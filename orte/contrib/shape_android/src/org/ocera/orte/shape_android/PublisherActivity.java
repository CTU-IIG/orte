/**
 * 
 *	This file is part of shape_android.
 *
 *  shape_android is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  shape_android is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with shape_android.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
package org.ocera.orte.shape_android;

import org.ocera.orte.DomainApp;
import org.ocera.orte.Manager;
import org.ocera.orte.types.NtpTime;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.os.Bundle;
import android.os.Handler;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.preference.PreferenceManager;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.ViewSwitcher;

/**
 * Class for object sent and received throw the ORTE middleware.
 * 
 * @author jiri hubacek <jiri.hubacek@gmail.com>
 * @version %I, %G
 * @since 1.2
 */
public class PublisherActivity extends Activity {
	//public final static Pattern IP_ADDRESS = Pattern.compile("((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))");

	private static final int REDRAW_INTERVAL = 50;
	private static final int STRENGTH_MAX = 5;
	private static final int MINSEPARATION_MAX = 5;
	private static final int RESULT_SETTINGS = 1;
	
	public static int SHAPE_WIDTH = 0;
	public static int SHAPE_HEIGHT = 0;
	
	private PowerManager powerManager;
	private WakeLock wakeLock;
	
	private WifiManager wifiManager;
	private WifiLock wifiLock;
	
	private Manager orteManager;
	private DomainApp appDomain;
	
	private PublisherView publisherView;
	private SubscriberView subscriberView;
	
	private ViewSwitcher switcher;
	private boolean actualIsPublisher;
	private SharedPreferences sp;
	
	private Handler handler = new Handler();
	private Runnable redraw = new Runnable()
	{
		@Override
		public void run()
		{
			publisherView.countShapes();
			publisherView.invalidate();
			handler.postDelayed(this, REDRAW_INTERVAL);
		}
	};
	
	/**
	 * Setting up main workspace.
	 * 
	 * @since 1.2
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_publisher);
		
		// count default shape size
		SHAPE_WIDTH = (int) (this.getWindowManager().getDefaultDisplay().getWidth() * 25 / BoxType.DESTINATION_WIDTH);
		SHAPE_HEIGHT = (int) (this.getWindowManager().getDefaultDisplay().getHeight() * 45 / BoxType.DESTINATION_HEIGHT);
		
		// From Robot_Demo project.
		wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
		wifiLock = (WifiLock) wifiManager.createWifiLock((
					(android.os.Build.VERSION.SDK_INT >= 12)
					? WifiManager.WIFI_MODE_FULL_HIGH_PERF
					: WifiManager.WIFI_MODE_FULL
				), getClass().getName());
		
		powerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
		wakeLock = (WakeLock) powerManager.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, getClass().getName() + " Dim");
		
		sp = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
		
		// Start ORTE.
		orteManager = new Manager(sp.getString("prefManagers", ""));
		appDomain = new DomainApp();
		
		this.switcher = (ViewSwitcher) findViewById(R.id.switcher);
		this.actualIsPublisher = true;
		
		this.publisherView = (PublisherView) findViewById(R.id.publisher_view);
		this.subscriberView = (SubscriberView) findViewById(R.id.subscriber_view);
		
		this.redraw.run();
		this.subscriberView.addElements(this.appDomain);
		for (SubscriberElement e : this.subscriberView.elements) {
			e.setScale(this.getWindowManager().getDefaultDisplay().getWidth(), this.getWindowManager().getDefaultDisplay().getHeight());
			e.setAllowScaling(sp.getBoolean("prefScaling", true));
		}
	}

	/**
	 * When pause activity, release WiFi lock and Wake lock.
	 * 
	 * @since 1.0
	 */
	@Override
	public void onPause()
	{
		super.onPause();
		
		wifiLock.release();
		wakeLock.release();
	}
	
	/**
	 * When resume activity, acquire WiFi lock and Wake lock.
	 * 
	 * @since 1.0
	 */
	@Override
	public void onResume()
	{
		super.onResume();
		
		wifiLock.acquire();
		wakeLock.acquire();
	}
	
	/**
	 * When switching application off.
	 * 
	 * @since 1.0
	 */
	@Override
	public void onDestroy()
	{
		super.onDestroy();

		for (PublisherShape shape : publisherView.shapes)
			shape.killMe();

		for (SubscriberElement element : subscriberView.elements)
			element.killMe();

		if (appDomain != null) {
			appDomain.destroy();
			appDomain = null;
		}
		if (orteManager != null) {
			orteManager.destroy();
			orteManager = null;
		}
	}
	
	/**
	 * When screen orientation is changed,
	 * scale recalculation is needed.
	 * 
	 * @since 1.0
	 */
	@Override
	public void onConfigurationChanged(Configuration newConfig)
	{
		super.onConfigurationChanged(newConfig);
		
		// count default shape size
		SHAPE_WIDTH = (int) (this.getWindowManager().getDefaultDisplay().getWidth() * 25 / BoxType.DESTINATION_WIDTH);
		SHAPE_HEIGHT = (int) (this.getWindowManager().getDefaultDisplay().getHeight() * 45 / BoxType.DESTINATION_HEIGHT);
				
		for (PublisherShape s : this.publisherView.shapes) {
			s.setScale(this.getWindowManager().getDefaultDisplay().getWidth(), this.getWindowManager().getDefaultDisplay().getHeight());
		}
		for (SubscriberElement e : this.subscriberView.elements) {
			e.setScale(this.getWindowManager().getDefaultDisplay().getWidth(), this.getWindowManager().getDefaultDisplay().getHeight());
		}
		
		//TODO When change rotation in Subscriber view,
		//		there is no change in Publisher view, which
		//		leads to misbehavior. For example, there
		//		is Publisher of Black color in Publisher view,
		//		and we are looking at it in Subscriber view,
		//		when rotation is changed, Publisher view remains
		//		in old rotation while Subscriber view is in new
		//		rotation. Therefore Black Publisher thinks that
		//		it's still in old rotation and the problem with
		//		leaving screen is here.
	}

	/**
	 * When returning from SettingsActivity, apply settings.
	 * 
	 * @since 1.2
	 */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
	    super.onActivityResult(requestCode, resultCode, data);
	
	    switch(requestCode) {
	    case RESULT_SETTINGS:
	    	boolean allowScaling = sp.getBoolean("prefScaling", true);

			for (PublisherShape s : this.publisherView.shapes) {
				s.box.allowScaling = allowScaling;
			}
	    	for (SubscriberElement e : this.subscriberView.elements) {
				e.setAllowScaling(allowScaling);
			}
	    	
	    	if (orteManager != null) orteManager.destroy();
			orteManager = new Manager(sp.getString("prefManagers", ""));
			
	    	break;
	    default:
	    }

    }
    
    /**
     * Creating menu.
     * 
     * @since 1.2
     */
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.publisher, menu);
		return true;
	}
	
	/**
	 * All created publishers are in menu.
	 * @return 
	 * 
	 * @since 1.0
	 */
	@Override
	public boolean onPrepareOptionsMenu(Menu menu)
	{
		menu.clear();
		
		if (this.actualIsPublisher) {
			getMenuInflater().inflate(R.menu.publisher, menu);
			for (PublisherShape shape : this.publisherView.shapes) {
				menu.add(0, this.publisherView.shapes.indexOf(shape), 20 + this.publisherView.shapes.indexOf(shape), "#" + this.publisherView.shapes.indexOf(shape) + " " + shape.getColorName() + " " + shape.getShapeName());
			}
		} else {
			getMenuInflater().inflate(R.menu.subscriber, menu);
		}
		
		return super.onPrepareOptionsMenu(menu);
	}
	
	/**
	 * Logic for menu selecting.
	 * 
	 * Contains switching between Publishers and Subscribers, adding
	 * new Publisher, setting Publishers, setting Subscribers, moving
	 * to SettingsActivity and HelpActivity.
	 * 
	 * @since 1.2
	 */
	@SuppressLint("NewApi")
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		final int id = item.getItemId();
		final Activity me = this;
		Intent intent;
		
		switch(id) {
		case R.id.action_switch:
			if (this.actualIsPublisher) {
				this.switcher.showNext();
				this.setTitle(R.string.title_activity_subscriber);
			} else {
				this.switcher.showPrevious();
				this.setTitle(R.string.title_activity_publisher);
			}
			this.actualIsPublisher = !this.actualIsPublisher;
			
			if (android.os.Build.VERSION.SDK_INT >= 11) this.invalidateOptionsMenu();
			
			return true;
		case R.id.action_new:
			String[] colors = new String[5];
			
			colors[0] = "Blue";
			colors[1] = "Green";
			colors[2] = "Red";
			colors[3] = "Black";
			colors[4] = "Yellow";
			
			AlertDialog.Builder builderNew = new AlertDialog.Builder(me);
			builderNew.setTitle(R.string.dialog_newPublisher)
			.setItems(colors, new DialogInterface.OnClickListener()
			{
				@Override
				public void onClick(DialogInterface dialog, int which)
				{
					// new publisher
					
					publisherView.addShape(which, appDomain);

					for (PublisherShape s : publisherView.shapes) {
						s.setScale(me.getWindowManager().getDefaultDisplay().getWidth(), me.getWindowManager().getDefaultDisplay().getHeight());
						s.box.allowScaling = sp.getBoolean("prefScaling", true);
					}
					
					if (android.os.Build.VERSION.SDK_INT >= 11) me.invalidateOptionsMenu();
				}
			})
			.setNegativeButton(R.string.dialog_cancel, new DialogInterface.OnClickListener()
			{
				@Override
				public void onClick(DialogInterface dialog, int which)
				{	
				}
			});
			
			AlertDialog dialogNew = builderNew.create();
			dialogNew.show();
			
			return true;
		case R.id.action_settings:
			intent = new Intent(this, SettingsActivity.class);
			startActivityForResult(intent, RESULT_SETTINGS);
			return true;
		case R.id.action_help:
			intent = new Intent(this, HelpActivity.class);
			startActivity(intent);
			return true;
		default:
			// For settings of Publisher/Subscriber elements.
			LinearLayout l = new LinearLayout(me);
			l.setOrientation(LinearLayout.VERTICAL);
			AlertDialog.Builder builderSettings = new AlertDialog.Builder(me);
			
			if (this.actualIsPublisher) {
				/*
				 * Parameters of Publisher shapes.
				 */
				final int shapeStrength = publisherView.shapes.get(id).getStrength();
				SeekBar seekStrength;
				
				/* seekBar */
				seekStrength = new SeekBar(me);
				seekStrength.setMax(STRENGTH_MAX);
				seekStrength.setProgress(shapeStrength);
				
				seekStrength.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
				{
					@Override
					public void onStopTrackingTouch(SeekBar seekBar) {
					}
					
					@Override
					public void onStartTrackingTouch(SeekBar seekBar) {
					}
					
					@Override
					public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser)
					{
						publisherView.shapes.get(id).setStrength(progress);
					}
				});
				
				l.addView(seekStrength);
				/* seekBar - end */
				
				builderSettings.setTitle("#" + id + " " + publisherView.shapes.get(id).getColorName() + " " + publisherView.shapes.get(id).getShapeName() + " strength:")
				.setView(l)
				.setPositiveButton(R.string.dialog_OK, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{}
				})
				.setNeutralButton(R.string.dialog_delete, new DialogInterface.OnClickListener()
				{	
					@Override
					public void onClick(DialogInterface dialog, int which) {
						publisherView.shapes.get(id).killMe();
						publisherView.shapes.remove(id);
						
						if (android.os.Build.VERSION.SDK_INT >= 11) me.invalidateOptionsMenu();
					}
				})
				.setNegativeButton(R.string.dialog_cancel, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{
						publisherView.shapes.get(id).setStrength(shapeStrength);
					}
				});
			
				AlertDialog dialogSettings = builderSettings.create();
				dialogSettings.show();
			} else {
				/*
				 * Parameters of Subscriber shapes.
				 */
				final int eId;
				SeekBar seekMinSeparation;
				
				switch (id) {
				case R.id.action_blue:
					eId = 0;
					break;
				case R.id.action_green:
					eId = 1;
					break;
				case R.id.action_red:
					eId = 2;
					break;
				case R.id.action_black:
					eId = 3;
					break;
				case R.id.action_yellow:
					eId = 4;
					break;
				default:
					eId = 0;
				}
				
				/* seekBar */
				seekMinSeparation = new SeekBar(me);
				seekMinSeparation.setMax(MINSEPARATION_MAX);
				seekMinSeparation.setProgress(0);
				subscriberView.elements.get(eId).setMinSeparation(new NtpTime(0));
				
				seekMinSeparation.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
				{
					@Override
					public void onStopTrackingTouch(SeekBar seekBar) {
					}
					
					@Override
					public void onStartTrackingTouch(SeekBar seekBar) {
					}
					
					@Override
					public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser)
					{
						subscriberView.elements.get(eId).setMinSeparation(new NtpTime(progress));
						subscriberView.elements.get(eId).setEnabled(true);
					}
				});
				
				l.addView(seekMinSeparation);
				/* seekBar - end */
				
				builderSettings.setTitle(PublisherShape.getColorName(eId) + " settings")
				.setView(l)
				.setPositiveButton(R.string.dialog_OK, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{}
				})
				.setNeutralButton(subscriberView.elements.get(eId).getEnabled()?R.string.dialog_delete:R.string.dialog_add, new DialogInterface.OnClickListener()
				{	
					@Override
					public void onClick(DialogInterface dialog, int which) {
						if (subscriberView.elements.get(eId).getEnabled()) {
							subscriberView.elements.get(eId).setEnabled(false);
						} else {
							subscriberView.elements.get(eId).setMinSeparation(new NtpTime(0));
							subscriberView.elements.get(eId).setEnabled(true);
						}
					}
				});
			
				AlertDialog dialogSettings = builderSettings.create();
				dialogSettings.show();
			}
		}
		
		return super.onOptionsItemSelected(item);
	}
}
