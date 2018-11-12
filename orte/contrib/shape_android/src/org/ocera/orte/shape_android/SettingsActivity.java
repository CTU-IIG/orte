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

import android.content.Context;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.preference.PreferenceActivity;
import android.preference.EditTextPreference;
import android.text.format.Formatter;

/**
 * This activity show settings.
 * 
 * @author jiri hubacek <jiri.hubacek@gmail.com>
 * @version %I, %G
 * @since 1.2
 */
public class SettingsActivity extends PreferenceActivity
{
	/**
	 * Setting up workspace.
	 * 
	 * @since 1.2
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		addPreferencesFromResource(R.layout.activity_settings);
		
		WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
		EditTextPreference pref = (EditTextPreference) findPreference("prefManagers");

		//TODO should be done in respect to @strings
		pref.setSummary("Comma separated manager\'s IP addresses" + " (actual IP: "+Formatter.formatIpAddress(wifiManager.getConnectionInfo().getIpAddress())+")");
	}
}
