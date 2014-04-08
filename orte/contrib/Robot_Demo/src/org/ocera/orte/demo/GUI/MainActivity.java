package org.ocera.orte.demo.GUI;

import org.ocera.orte.DomainApp;
import org.ocera.orte.Manager;
//import org.ocera.orte.demo.R;
import org.ocera.orte.demo.Publishers.CraneCmdPublish;
import org.ocera.orte.demo.Publishers.MagnetCmdPublish;
import org.ocera.orte.demo.Publishers.MotionSpeedPublish;
import org.ocera.orte.demo.Subscribers.HokuyoScanSubscribe;
import org.ocera.orte.demo.Subscribers.MotionSpeedSubscribe;
import org.ocera.orte.demo.Subscribers.PwrVoltageSubscribe;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnDismissListener;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.text.format.Formatter;
import android.view.GestureDetector;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
 
public class MainActivity extends Activity {
	private AlertDialog voltageDialog = null;
	private AlertDialog aboutDialog = null;
	private AlertDialog managersDialog = null;
	private EditText managersField = null;
	private TextView ownIP = null;
	private NetworkInfo wifiInfoTask = null; 
	static EditText voltage33 = null;
	static EditText voltage50 = null;
	static EditText voltage80 = null;
	static EditText voltageBAT = null;
	static Handler dialogHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			double[] values = msg.getData().getDoubleArray("voltages");
			if (values != null) {
				if (voltage33 != null) {
					voltage33.setText(Double.toString(values[0]));
				}
				if (voltage50 != null) {
					voltage50.setText(Double.toString(values[1]));
				}
				if (voltage80 != null) {
					voltage80.setText(Double.toString(values[2]));
				}
				if (voltageBAT != null) {
					voltageBAT.setText(Double.toString(values[3]));
				}
			}
		}
    };
	
	private Manager manager = null;
    private String mgrs = null;
	private MotionSpeedPublish motion_speed_publ = null;
	private MotionSpeedSubscribe motion_speed_subs = null;
	private HokuyoScanSubscribe hokuyo_scan = null;
	private PwrVoltageSubscribe pwr_voltage = null;
	private MagnetCmdPublish magnet_cmd = null;
	private CraneCmdPublish crane_cmd = null;
    private SensorManager mSensorManager = null;
    private Sensor mGravity = null;
    private SensorEventListener accel = null;
    private PowerManager mPowerManager = null;
    private WifiManager mWifiManager = null;
    private WakeLock mWakeLock = null;
    private WakeLock mDimLock = null;
    private WifiLock mWifiLock = null;
    private DomainApp appDomain = null;
    private HokuyoView hokuyo_view = null;
    private SharedPreferences prefs = null;
    private GestureDetector gDetector = null;

    static {
    	System.loadLibrary("jorte");     
    }

    @Override
    protected void onPause() {
        super.onPause();
        
        if (motion_speed_publ != null && !motion_speed_publ.isCancelled()) {
        	motion_speed_publ.cancel();
            mSensorManager.unregisterListener(accel);
            this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
            mDimLock.release();
            mWifiLock.release();
        }
        
        if (motion_speed_subs != null && !motion_speed_subs.isCancelled()) {
        	hokuyo_view.runMotion(false);
        	motion_speed_subs.cancel();
        	hokuyo_view.invalidate();
            mWakeLock.release();
            mWifiLock.release();
        }

        if (hokuyo_scan != null && !hokuyo_scan.isCancelled()) {
			hokuyo_view.run(false);
			hokuyo_scan.cancel();
			hokuyo_view.invalidate();
	        mWakeLock.release();
	        mWifiLock.release();
        }
    }
    
    @Override
    protected void onDestroy() {
    	super.onDestroy();

        if (pwr_voltage != null && !pwr_voltage.isCancelled()) {
        	if (voltageDialog.isShowing())
        		voltageDialog.dismiss();
        }
    	
    	if (crane_cmd != null && !crane_cmd.isCancelled()) {
    		crane_cmd.cancel();
    	}
    	
    	if (magnet_cmd != null && !crane_cmd.isCancelled()) {
    		magnet_cmd.cancel();
    	}
    	
        if (appDomain != null) {
        	appDomain.destroy();
        	appDomain = null;
        }
        
        if (manager != null) {
        	manager.destroy();
        	manager = null;
        }
    }
	
	@Override
    public void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
        setContentView(R.layout.hokuyo_view);
        
        mPowerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
        mWakeLock = mPowerManager.newWakeLock(
        		PowerManager.SCREEN_BRIGHT_WAKE_LOCK,
        		getClass().getName() + " Bright");
        mDimLock = mPowerManager.newWakeLock(
        		PowerManager.SCREEN_DIM_WAKE_LOCK,
        		getClass().getName() + " Dim");

        mWifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
        mWifiLock = mWifiManager.createWifiLock(
        		android.os.Build.VERSION.SDK_INT >= 12
        		? WifiManager.WIFI_MODE_FULL_HIGH_PERF
        		: WifiManager.WIFI_MODE_FULL, getClass().getName());
        
        prefs = getSharedPreferences("prefs", 0);
        mgrs = prefs.getString("managers", "10.1.1.1");
        
		AlertDialog.Builder voltageBuilder = new AlertDialog.Builder(this);
		LayoutInflater inflater = getLayoutInflater();
		View voltageView = inflater.inflate(R.layout.status_dialog, null);
		voltageBuilder.setView(voltageView);
		voltageBuilder.setPositiveButton("OK",null);
		voltageBuilder.setTitle("Voltages");
		voltageDialog = voltageBuilder.create();
		voltageDialog.setOnDismissListener(new OnDismissListener(){
			@Override
			public void onDismiss(DialogInterface arg0) {
				voltageDialog.dismiss();
				pwr_voltage.cancel();
				mDimLock.release();
			}
		});
		voltage33 = (EditText)voltageView.findViewById(R.id.editText1);
		voltage50 = (EditText)voltageView.findViewById(R.id.editText2);
		voltage80 = (EditText)voltageView.findViewById(R.id.editText3);
		voltageBAT = (EditText)voltageView.findViewById(R.id.editText4);
		
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		View aboutView = inflater.inflate(R.layout.about_dialog, null);
		builder.setView(aboutView);
		builder.setPositiveButton("OK", null);
		aboutDialog = builder.create();
		
		AlertDialog.Builder managersBuilder = new AlertDialog.Builder(this);
		View managersView = inflater.inflate(R.layout.managers_view, null);
		ownIP = (TextView)managersView.findViewById(R.id.ownIP);
		managersField = (EditText)managersView.findViewById(R.id.managers);
		managersField.setText(mgrs);
		
		managersBuilder.setView(managersView);
		managersBuilder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				SharedPreferences.Editor editor = prefs.edit();
				mgrs = managersField.getText().toString();
				editor.putString("managers", mgrs);
				editor.commit();
				
				if (manager != null)
					manager.destroy();
				manager = new Manager(mgrs);
				
				managersDialog.dismiss();
			}
		});
		managersBuilder.setNeutralButton("Cancel", null);
		managersBuilder.setTitle("Set fellow managers");
		managersDialog = managersBuilder.create();
		managersDialog.setOnDismissListener(new OnDismissListener() {
			@Override
			public void onDismiss(DialogInterface arg0) {
				managersField.setText(mgrs);
				wifiInfoTask.cancel(false);
			}
		});
		
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mGravity = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        
        hokuyo_view = (HokuyoView) findViewById(R.id.hokuyo_view);
        gDetector = new GestureDetector(hokuyo_view.getContext(), new HokuyoGestures());
        hokuyo_view.setOnTouchListener(new View.OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				return gDetector.onTouchEvent(event);
			}
        	
        });

        manager = new Manager(mgrs);

        appDomain = new DomainApp();
        magnet_cmd = new MagnetCmdPublish(appDomain);
        magnet_cmd.start();
        crane_cmd = new CraneCmdPublish(appDomain);
        crane_cmd.start();
    }
    
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.activity_main, menu);

		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected (MenuItem item) {
		if (item.getTitle().equals("Crane: up")) {
			crane_cmd.send(0x190);
			item.setTitle("Crane: down");
		}
		else if (item.getTitle().equals("Crane: down")) {
			crane_cmd.send(0x100);
			item.setTitle("Crane: up");
		}
		else if (item.getTitle().equals("Magnet")) {
			if (!item.isChecked()) {
				magnet_cmd.send(1);
				item.setChecked(true);
			}
			else {
				magnet_cmd.send(0);
				item.setChecked(false);	
			}
		}
		else if (item.getTitle().equals("Voltage monitor")) {
			mDimLock.acquire();
			if (pwr_voltage == null)
				pwr_voltage = new PwrVoltageSubscribe(appDomain, dialogHandler);
			pwr_voltage.start();
			voltageDialog.show();
		}
		else if (item.getTitle().equals("Fellow managers")) {
			wifiInfoTask = new NetworkInfo();

			managersDialog.show();
			wifiInfoTask.execute();
		}
		else if (item.getTitle().equals("About")) {
			aboutDialog.show();
		}
		else if (item.getTitle().equals("Exit")) {
			finish();
		}
		
		return true;
	}
	
    private class HandleAccelerometer implements SensorEventListener {

    	@Override
    	public void onAccuracyChanged(Sensor sensor, int accuracy) {}

    	@Override
    	public void onSensorChanged(SensorEvent event) {
    		 if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
    			 if (motion_speed_publ != null)
    				 motion_speed_publ.setSpeed(event.values[0], event.values[1]);
    		 }
    	}
    }
    
//    hokuyo_view.setOnTouchListener(new View.OnTouchListener() {
    private class HokuyoGestures extends GestureDetector.SimpleOnGestureListener {

		@Override
		public boolean onSingleTapConfirmed(MotionEvent event) {
			// urceni orientace HokuyoView a poctu pixelu, ktere zabira hokuyo
			double norm = (double)hokuyo_view.getWidth()/(2*HokuyoView.COSINUS);
			boolean isVertical = true;
			if (norm > hokuyo_view.getHeight()) {
				norm = HokuyoView.COSINUS*hokuyo_view.getHeight()*2;
				norm = (hokuyo_view.getWidth() - norm)/2;
				isVertical = false;
			}
			else
				norm = hokuyo_view.getHeight() - norm;
			
			// urceni poctu pixelu, ktere zabira ctverec monitoru rychlosti
			double normSpeed = ((double)(isVertical ? hokuyo_view.getWidth() : hokuyo_view.getHeight())) * 0.375;
			
			if (event.getYPrecision()*event.getY() < normSpeed &&
					event.getXPrecision()*event.getX() < normSpeed) {
				if(!hokuyo_view.isRunningMotion()) {
					mWakeLock.acquire();
					mWifiLock.acquire();
					if (motion_speed_subs == null)
						motion_speed_subs = new MotionSpeedSubscribe(appDomain, hokuyo_view);
					motion_speed_subs.start();
					hokuyo_view.runMotion(true);
					hokuyo_view.invalidate();
					Toast.makeText(getApplicationContext(), "Speed Monitor: ON", Toast.LENGTH_SHORT).show();
				}
				else {
					hokuyo_view.runMotion(false);
					motion_speed_subs.cancel();
					hokuyo_view.invalidate();
					mWakeLock.release();
					mWifiLock.release();
					Toast.makeText(getApplicationContext(), "Speed Monitor: OFF", Toast.LENGTH_SHORT).show();
				}
				return true;
			}
			
			if ((isVertical && event.getYPrecision()*event.getY() > norm) ||
					(!isVertical && event.getXPrecision()*event.getX() > norm &&
							event.getXPrecision()*event.getX() < hokuyo_view.getWidth() - norm)) {
				if (!hokuyo_view.isRunning()) {
					mWakeLock.acquire();
					mWifiLock.acquire();
					hokuyo_view.setData(new int[681]);
					if (hokuyo_scan == null)
						hokuyo_scan = new HokuyoScanSubscribe(appDomain, hokuyo_view);
					hokuyo_scan.start();
					hokuyo_view.run(true);
					hokuyo_view.invalidate();
					Toast.makeText(getApplicationContext(), "Hokuyo LRF: ON", Toast.LENGTH_SHORT).show();
				}
				else {
					hokuyo_view.run(false);
					hokuyo_scan.cancel();
					hokuyo_view.invalidate();
					mWakeLock.release();
					mWifiLock.release();
					Toast.makeText(getApplicationContext(), "Hokuyo LRF: OFF", Toast.LENGTH_SHORT).show();
				}
				return true;
			}
			
			return false;
		}

		@Override
		public boolean onDown(MotionEvent e) {
			return true;
		}

		@Override
		public void onLongPress(MotionEvent event) {
			// urceni poctu pixelu, ktere zabira ctverec monitoru rychlosti
			double normSpeed = ((double)(hokuyo_view.getHeight() < hokuyo_view.getWidth() ? hokuyo_view.getHeight() : hokuyo_view.getWidth())) * 0.375;
			
			if (event.getYPrecision()*event.getY() < normSpeed &&
					event.getXPrecision()*event.getX() < normSpeed) {
				if (motion_speed_publ == null || motion_speed_publ.isCancelled()) {
					mDimLock.acquire();
					mWifiLock.acquire();
					accel = new HandleAccelerometer();
					mSensorManager.registerListener(accel, mGravity, SensorManager.SENSOR_DELAY_GAME);
					if (motion_speed_publ == null)
						motion_speed_publ = new MotionSpeedPublish(appDomain);
					motion_speed_publ.start();
					setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
					Toast.makeText(getApplicationContext(), "Motion Control: ON", Toast.LENGTH_SHORT).show();
				}
				else {
					mSensorManager.unregisterListener(accel);
					motion_speed_publ.cancel();
					setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
					mDimLock.release();
					mWifiLock.release();
					Toast.makeText(getApplicationContext(), "Motion Control: OFF", Toast.LENGTH_SHORT).show();
				}
			}
		}
    }
    
    private class NetworkInfo extends AsyncTask<Void, String, Void> {
    	@Override
    	protected Void doInBackground(Void... arg0) {
    		while (!isCancelled()) {
		        WifiInfo wifiInfo = mWifiManager.getConnectionInfo();
		        int ip = wifiInfo.getIpAddress();
		        String ipAddress = Formatter.formatIpAddress(ip);
		        String ssid = wifiInfo.getSSID();
	
		        publishProgress("\nOwn IP Address: " + ipAddress + "\nNetwork SSID: " + ssid);
		        
		        try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
    		}
    		
    		return null;
    	}

    	@Override
    	protected void onProgressUpdate(String... values) {
    		super.onProgressUpdate(values);
    		
    		ownIP.setText(values[0]);
    	}
    }
}
