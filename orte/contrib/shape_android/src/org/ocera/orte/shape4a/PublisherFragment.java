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

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.SeekBar;

import org.ocera.orte.DomainApp;

/**
 * Fragment containing publishing workspace.
 * 
 * @author jiri hubacek <jiri.hubacek@gmail.com>
 * @version %I%, %G%
 * @since 1.0
 */
public class PublisherFragment extends Fragment {
	private static final int REDRAW_INTERVAL = 50;
	private static final int STRENGTH_MAX = 5;
	
	private View rootView;
	private PublisherView publisherView;
	
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
	
	NewDialogFragment newDialogFragment = new NewDialogFragment();
	PublisherSettingsDialogFragment publisherSettingsDialogFragment = new PublisherSettingsDialogFragment();
	
	private DomainApp appDomain;
	
	/**
	 * Constructor just stores appDomain variable.
	 * 
	 * @param appDomain
	 * @since 1.0
	 */
	public PublisherFragment(DomainApp appDomain)
	{
		this.appDomain = appDomain;
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
		for (PublisherShape s : this.publisherView.shapes) {
			s.setScale(getActivity().getWindowManager().getDefaultDisplay().getWidth(), getActivity().getWindowManager().getDefaultDisplay().getHeight());
		}
	}
	
	/**
	 * Setting up workspace, adding publisherView
	 * and timer (handler) for drawing objects.
	 * 
	 * @since 1.0
	 */
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
	{
		this.rootView = inflater.inflate(R.layout.fragment_publisher, container, false);
		this.publisherView = (PublisherView) this.rootView.findViewById(R.id.publisher_view);
		this.handler.postDelayed(redraw, REDRAW_INTERVAL);
		
		return this.rootView;
	}
	
	/**
	 * Creating menu.
	 * 
	 * @since 1.0
	 */
	@Override
	public void onCreateOptionsMenu(Menu menu, MenuInflater inflater)
	{
		inflater.inflate(R.menu.publisher, menu);
	}
	
	/**
	 * All created publishers are in menu.
	 * 
	 * @since 1.0
	 */
	@Override
	public void onPrepareOptionsMenu(Menu  menu)
	{
		for (PublisherShape shape : publisherView.shapes) {
			menu.add(0, publisherView.shapes.indexOf(shape), 20 + publisherView.shapes.indexOf(shape), "#" + publisherView.shapes.indexOf(shape) + " " + shape.getColorName() + " " + shape.getShapeName());
		}
	}
	
	/**
	 * Select settings of publishers
	 * or add new publisher.
	 * 
	 * @since 1.0
	 */
	@Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		
		if (id >= 0 && id < publisherView.shapes.size()) {
			publisherSettingsDialogFragment.setId(id);
			publisherSettingsDialogFragment.show(getFragmentManager(), "dialog");
		} else {
			switch (id) {
			case R.id.action_new:
				newDialogFragment.show(getFragmentManager(), "dialog");
				break;
			}
		}
		return super.onOptionsItemSelected(item);
	}
	
	/**
	 * Dialog for adding new publisher
	 * throw action bar button +.
	 * 
	 * @since 1.0
	 */
	public class NewDialogFragment extends DialogFragment
	{
		/*
		 * Selection of colors is badly solved,
		 * it should be altered..
		 * 
		 */
		private String[] colors = new String[5];
		
		/**
		 * In constructor, make array
		 * of colors.
		 * 
		 * @since 1.0
		 */
		public NewDialogFragment()
		{
			// Texts should be done better way,
			// like centralized in strings.xml
			
			colors[0] = "Blue";
			colors[1] = "Green";
			colors[2] = "Red";
			colors[3] = "Black";
			colors[4] = "Yellow";
		}
		
		/**
		 * Creating dialog for new publisher.
		 * 
		 * @since 1.0
		 */
		@Override
		public Dialog onCreateDialog(Bundle savedInstanceState)
		{
			AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
			builder.setTitle(R.string.dialog_newPublisher)
				.setItems(colors, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{
						// new publisher
						
						publisherView.addShape(which, appDomain);

						for (PublisherShape s : publisherView.shapes) {
							s.setScale(getActivity().getWindowManager().getDefaultDisplay().getWidth(), getActivity().getWindowManager().getDefaultDisplay().getHeight());
						}
						
						getActivity().supportInvalidateOptionsMenu();
					}
				})
				.setNegativeButton(R.string.dialog_cancel, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{	
					}
				});
			return builder.create();
			
		}
	}
	
	/**
	 * When click on shape in Action Bar menu.
	 * 
	 * @since 1.0
	 */
	public class PublisherSettingsDialogFragment extends DialogFragment
	{
		private int shapeId;
		private int shapeStrength;
		private SeekBar seekStrength;
		
		/**
		 * No constructor needed.
		 * 
		 * @since 1.0
		 */
		public PublisherSettingsDialogFragment()
		{}
		
		/**
		 * Set wanted publisher's dialog.
		 * 
		 * @param i Index of publisher.
		 * @since 1.0
		 */
		public void setId(int i)
		{
			this.shapeId = i;
			this.shapeStrength = publisherView.shapes.get(this.shapeId).getStrength();
		}
		
		/**
		 * Creating dialog for publisher's settings.
		 * 
		 * @since 1.0
		 */
		@Override
		public Dialog onCreateDialog(Bundle savedInstanceState)
		{
			AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
			builder.setTitle("#" + this.shapeId + " " + publisherView.shapes.get(this.shapeId).getColorName() + " " + publisherView.shapes.get(this.shapeId).getShapeName() + " strength:")
				.setView(seekBar())
				.setPositiveButton(R.string.dialog_OK, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{
						publisherView.shapes.get(shapeId).setStrength(shapeStrength);
					}
				})
				.setNeutralButton(R.string.dialog_delete, new DialogInterface.OnClickListener()
				{	
					@Override
					public void onClick(DialogInterface dialog, int which) {
						publisherView.shapes.get(shapeId).killMe();
						publisherView.shapes.remove(shapeId);
						getActivity().supportInvalidateOptionsMenu();
					}
				})
				.setNegativeButton(R.string.dialog_cancel, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{	
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
		private View seekBar(){
			LinearLayout l = new LinearLayout(getActivity());
			l.setOrientation(LinearLayout.VERTICAL);
			
			seekStrength = new SeekBar(getActivity());
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
					shapeStrength = progress;
				}
			});
			
			l.addView(seekStrength);
			return l;
		}
	}
}
