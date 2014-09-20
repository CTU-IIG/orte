/**
 * 
 *	This file is part of shape4a.
 *
 *  shape4a is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  shape4a is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with shape4a.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
package org.ocera.orte.shape4a;

import java.util.regex.Pattern;

import android.support.v7.app.ActionBarActivity;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.text.InputType;
import android.text.format.Formatter;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;

import org.ocera.orte.DomainApp;
import org.ocera.orte.Manager;

//TODO Better navigation (swipe sometimes miss drag).

/**
 * Main application activity, when application
 * launch, this activity is choosen.
 * 
 * @author jiri hubacek <jiri.hubacek@gmail.com>
 * @version %I%, %G%
 * @since 1.0
 */
public class MainActivity extends ActionBarActivity {
	public final static Pattern IP_ADDRESS = Pattern.compile("((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))");
	
	/**
	 * The {@link android.support.v4.view.PagerAdapter} that will provide
	 * fragments for each of the sections. We use a {@link FragmentPagerAdapter}
	 * derivative, which will keep every loaded fragment in memory. If this
	 * becomes too memory intensive, it may be best to switch to a
	 * {@link android.support.v4.app.FragmentStatePagerAdapter}.
	 */
	SectionsPagerAdapter mSectionsPagerAdapter;

	/**
	 * The {@link ViewPager} that will host the section contents.
	 */
	ViewPager mViewPager;

	private WifiManager wifiManager;
	private WifiLock wifiLock;
	
	private Manager orteManager;
	private DomainApp appDomain;
	
	private SettingsDialogFragment settingsDialogFragment = new SettingsDialogFragment();
	private HelpDialogFragment helpDialogFragment = new HelpDialogFragment();

	/**
	 * Setting up workspace.
	 * 
	 * @param savedInstanceState
	 * @since 1.0
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		// Create the adapter that will return a fragment for each of the three
		// primary sections of the activity.
		mSectionsPagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());

		// Set up the ViewPager with the sections adapter.
		mViewPager = (ViewPager) findViewById(R.id.pager);
		mViewPager.setAdapter(mSectionsPagerAdapter);
		
		// From Robot_Demo project.
		wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
		wifiLock = (WifiLock) wifiManager.createWifiLock((
					(android.os.Build.VERSION.SDK_INT >= 12)
					? WifiManager.WIFI_MODE_FULL_HIGH_PERF
					: WifiManager.WIFI_MODE_FULL
				), getClass().getName());
		
				
		// Start ORTE.
		orteManager = new Manager("");
		appDomain = new DomainApp();
		
	}

	/**
	 * When pause activity, release WiFi lock.
	 * 
	 * @since 1.0
	 */
	@Override
	public void onPause()
	{
		super.onPause();
		
		wifiLock.release();
	}
	
	/**
	 * When resume activity, acquire WiFi lock.
	 * 
	 * @since 1.0
	 */
	@Override
	public void onResume()
	{
		super.onResume();
		
		wifiLock.acquire();
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
	 * Adding elements to menu.
	 * 
	 * @since 1.0
	 */
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	/**
	 * When Settings or Help button is selected.
	 * 
	 * @since 1.0
	 */
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		switch (id) {
		case R.id.action_settings:
			//TODO Menu Settings (prefer own activity)
			// Ideas about settings:
			// - input for manager's ip adresses
			// - enable/disable scaling, destination screen size
			
			settingsDialogFragment.show(getSupportFragmentManager(), "dialog");
			return true;
		case R.id.action_help:
			//TODO Menu Help (prefer own activity)
			
			helpDialogFragment.show(getSupportFragmentManager(), "dialog");
			return true;
		default:
			// Nothing...
		}
		return super.onOptionsItemSelected(item);
	}

	/**
	 * A {@link FragmentPagerAdapter} that returns a fragment corresponding to
	 * one of the sections/tabs/pages.
	 */
	public class SectionsPagerAdapter extends FragmentPagerAdapter {

		public SectionsPagerAdapter(FragmentManager fm) {
			super(fm);
		}

		@Override
		public Fragment getItem(int position) {
			// getItem is called to instantiate the fragment for the given page.
			// Return a PlaceholderFragment (defined as a static inner class
			// below).
			if (position == 0) {
				PublisherFragment fragment = new PublisherFragment(appDomain);
				fragment.setHasOptionsMenu(true);
				return fragment;
			} else {
				SubscriberFragment fragment = new SubscriberFragment(appDomain);
				fragment.setHasOptionsMenu(true);
				return fragment;
			}
		}

		@Override
		public int getCount() {
			// Show 2 total pages - Publisher and Subscriber
			return 2;
		}
	}
	
	/**
	 * Very easy help/about.
	 * 
	 * @since 1.0
	 */
	public class HelpDialogFragment extends DialogFragment
	{
		/**
		 * Create dialog window.
		 * 
		 * @since 1.0
		 */
		@Override
		public Dialog onCreateDialog(Bundle savedInstanceState)
		{
			AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
			builder.setTitle("Help/About")
				.setMessage("Under construction\n" +
						"\n" +
						"written by jiri hubacek\n")
				.setPositiveButton(R.string.dialog_OK, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which)
					{}
				});
			
			return builder.create();
		}
	}
	
	/**
	 * Set up IP addresses of other managers.
	 * 
	 * @since 1.0
	 */
	public class SettingsDialogFragment extends DialogFragment
	{
		private EditText om;
		
		/**
		 * Create dialog window.
		 * 
		 * @since 1.0
		 */
		@Override
		public Dialog onCreateDialog(Bundle savedInstanceState)
		{
			// Strings should be done better way, like
			// centralized in strings.xml
			
			AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
			builder.setTitle("Your IP is " + Formatter.formatIpAddress(wifiManager.getConnectionInfo().getIpAddress()) + ",\nadd other managers:")
				.setView(text())
				.setPositiveButton(R.string.dialog_OK, new DialogInterface.OnClickListener() {
					
					/**
					 * When IP addresses of managers changes,
					 * destroy manager and create new one.
					 * 
					 * @since 1.0
					 */
					@Override
					public void onClick(DialogInterface dialog, int which)
					{
						if (IP_ADDRESS.matcher(om.getText()).matches()) {
							if (orteManager != null) orteManager.destroy();
							
							String managers[] = new String[1];
							managers[0] = om.getText().toString();
							
							orteManager = new Manager(managers);
						}
					}
				})
				.setNegativeButton(R.string.dialog_cancel, new DialogInterface.OnClickListener() {
					
					@Override
					public void onClick(DialogInterface dialog, int which) {
					}
				});
			
			return builder.create();
		}
		
		/**
		 * SeekBar for setting-up strength of publishers.
		 * 
		 * @return Layout of SeekBar.
		 * @since 1.0
		 */
		private View text(){
			LinearLayout l = new LinearLayout(getActivity());
			l.setOrientation(LinearLayout.VERTICAL);
			
			om = new EditText(getActivity());
			om.setInputType(InputType.TYPE_CLASS_PHONE);
			om.setText("192.168.1.108");
			
			l.addView(om);
			return l;
		}
	}
	
}
