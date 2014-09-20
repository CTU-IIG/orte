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
import android.support.v4.app.DialogFragment;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.SeekBar;

import org.ocera.orte.DomainApp;
import org.ocera.orte.types.NtpTime;

/**
 * Fragment containing subscribing workspace.
 * 
 * @author jiri hubacek <jiri.hubacek@gmail.com>
 * @version %I%, %G%
 * @since 1.0
 */
public class SubscriberFragment extends Fragment {
	private static final int MINSEPARATION_MAX = 5;
	
	private View rootView;
	private SubscriberView subscriberView;
	private DomainApp appDomain;
	
	private SubscriberSettingsDialogFragment subscriberSettingsDialogFragment = new SubscriberSettingsDialogFragment();
	
	/**
	 * Constructor just stores appDomain variable.
	 * 
	 * @param appDomain
	 * @since 1.0
	 */
	public SubscriberFragment(DomainApp appDomain)
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
		for (SubscriberElement e : this.subscriberView.elements) {
			e.setScale(getActivity().getWindowManager().getDefaultDisplay().getWidth(), getActivity().getWindowManager().getDefaultDisplay().getHeight());
		}
	}
	
	/**
	 * Setting up workspace, adding publisherView
	 * and timer (handler) for drawing objects.
	 * 
	 * @since 1.0
	 */
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		
		this.rootView = inflater.inflate(R.layout.fragment_subscriber, container, false);
		this.subscriberView = (SubscriberView) this.rootView.findViewById(R.id.subscriber_view);
		this.subscriberView.addElements(this.appDomain);
		for (SubscriberElement e : this.subscriberView.elements) {
			e.setScale(getActivity().getWindowManager().getDefaultDisplay().getWidth(), getActivity().getWindowManager().getDefaultDisplay().getHeight());
		}
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
		inflater.inflate(R.menu.subscriber, menu);
	}
	
	/**
	 * Setting up subscriber objects.
	 * 
	 * @since 1.0
	 */
	@Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
		int id = 0;
		switch (item.getItemId()) {
		case R.id.action_blue:
			id = 0;
			break;
		case R.id.action_green:
			id = 1;
			break;
		case R.id.action_red:
			id = 2;
			break;
		case R.id.action_black:
			id = 3;
			break;
		case R.id.action_yellow:
			id = 4;
			break;
		default:
		}
		
		subscriberSettingsDialogFragment.setId(id);
		subscriberSettingsDialogFragment.show(getFragmentManager(), "dialog");
		
		return super.onOptionsItemSelected(item);
	}
	
	/**
	 * When click on shape in Action Bar menu.
	 * 
	 * @since 1.0
	 */
	public class SubscriberSettingsDialogFragment extends DialogFragment
	{
		private int eId, ms;
		private NtpTime minSeparation;
		private SeekBar seekMinSeparation;
		
		/**
		 * Set wanted publisher's dialog.
		 * 
		 * @param i Index of publisher.
		 * @since 1.0
		 */
		public void setId(int i)
		{
			this.eId = i;
			this.minSeparation = subscriberView.elements.get(this.eId).getMinSeparation();
			this.ms = 0;
		}
		
		/**
		 * Creating dialog for subscriber's settings.
		 * 
		 * @since 1.0
		 */
		@Override
		public Dialog onCreateDialog(Bundle savedInstanceState)
		{
			AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
			builder.setTitle(PublisherShape.getColorName(this.eId) + " settings")
				.setView(seekBar())
				.setPositiveButton(R.string.dialog_OK, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{
						minSeparation = new NtpTime(ms);
						subscriberView.elements.get(eId).setMinSeparation(minSeparation);
						subscriberView.elements.get(eId).setEnabled(true);
					}
				})
				.setNeutralButton(subscriberView.elements.get(eId).getEnabled()?R.string.dialog_delete:R.string.dialog_add, new DialogInterface.OnClickListener()
				{	
					@Override
					public void onClick(DialogInterface dialog, int which) {
						if (subscriberView.elements.get(eId).getEnabled()) {
							subscriberView.elements.get(eId).setEnabled(false);
						} else {
							minSeparation = new NtpTime(ms);
							subscriberView.elements.get(eId).setMinSeparation(minSeparation);
							subscriberView.elements.get(eId).setEnabled(true);
						}
					}
				})
				.setNegativeButton(R.string.dialog_cancel, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{}
				});
			return builder.create();
		}
		
		/**
		 * SeekBar for setting-up min. separation of subscribers.
		 * 
		 * @return Layout of SeekBar.
		 * @since 1.0
		 */
		private View seekBar(){
			LinearLayout l = new LinearLayout(getActivity());
			l.setOrientation(LinearLayout.VERTICAL);
			
			seekMinSeparation = new SeekBar(getActivity());
			seekMinSeparation.setMax(MINSEPARATION_MAX);
			seekMinSeparation.setProgress(0);
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
					ms = progress;
				}
			});
			
			l.addView(seekMinSeparation);
			return l;
		}
	}
}
