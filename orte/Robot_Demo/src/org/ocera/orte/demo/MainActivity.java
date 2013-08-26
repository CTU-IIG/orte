package org.ocera.orte.demo;

import org.ocera.orte.DomainApp;
import org.ocera.orte.Manager;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.ActivityInfo;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
 
public class MainActivity extends Activity {
	private Dialog voltageDialog = null;
	private AlertDialog aboutDialog = null;
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
    private String[] mgrs = {"192.168.1.5","192.168.1.8","192.168.1.29",
    		"10.1.1.1","10.1.1.2","10.1.1.3","10.1.1.4","10.1.1.5","10.1.1.6","10.1.1.7","10.1.1.8","10.1.1.9","10.1.1.10",
    		"10.1.1.11","10.1.1.12","10.1.1.13","10.1.1.14","10.1.1.15","10.1.1.16","10.1.1.17","10.1.1.18","10.1.1.19","10.1.1.20"};
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
    private WifiLock mWifiLock = null;
    private DomainApp appDomain = null;
    private HokuyoView hokuyo_view = null;
    private MenuItem speed_publ_item = null;
    private MenuItem speed_subs_item = null;
    private MenuItem hokuyo_item = null;

    static {
    	System.loadLibrary("jorte");     
    }

    @Override
    protected void onResume() {
        super.onResume();

        mWakeLock.acquire();
        mWifiLock.acquire();
    }

    @Override
    protected void onPause() {
        super.onPause();
        
        if (motion_speed_publ != null && !motion_speed_publ.isCancelled()) {
        	motion_speed_publ.cancel();
            mSensorManager.unregisterListener(accel);
            this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
        	speed_publ_item.setTitle("Start motion control");
        }
        
        if (motion_speed_subs != null && !motion_speed_subs.isCancelled()) {
        	hokuyo_view.runMotion(false);
        	motion_speed_subs.cancel();
        	hokuyo_view.invalidate();
        	speed_subs_item.setTitle("Start motion monitor");
        }

        if (hokuyo_scan != null && !hokuyo_scan.isCancelled()) {
			hokuyo_view.run(false);
			hokuyo_scan.cancel();
			hokuyo_view.invalidate();
			hokuyo_item.setTitle("Start LRF");
        }

        if (pwr_voltage != null && !pwr_voltage.isCancelled()) {
        	if (voltageDialog.isShowing())
        		voltageDialog.dismiss();
			pwr_voltage.cancel();
        }
        
        mWakeLock.release();
        mWifiLock.release();
    }
    
    @Override
    protected void onDestroy() {
    	super.onDestroy();
    	
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
	
	@SuppressWarnings("deprecation")
	@Override
    public void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
        setContentView(R.layout.hokuyo_view);
        
		AlertDialog.Builder voltageBuilder = new AlertDialog.Builder(this);
		LayoutInflater inflater = getLayoutInflater();
		View voltageView = inflater.inflate(R.layout.status_dialog, null);
		voltageBuilder.setCancelable(false);
		voltageBuilder.setView(voltageView);
		voltageBuilder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				voltageDialog.dismiss();
				pwr_voltage.cancel();
			}
		});
		voltageBuilder.setTitle("Voltages");
		voltageDialog = voltageBuilder.create();
		voltage33 = (EditText)voltageView.findViewById(R.id.editText1);
		voltage50 = (EditText)voltageView.findViewById(R.id.editText2);
		voltage80 = (EditText)voltageView.findViewById(R.id.editText3);
		voltageBAT = (EditText)voltageView.findViewById(R.id.editText4);
		
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		View aboutView = inflater.inflate(R.layout.about_dialog, null);
		builder.setView(aboutView);
		builder.setPositiveButton("OK", null);
		aboutDialog = builder.create();
		
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mGravity = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        
        mPowerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
        mWakeLock = mPowerManager.newWakeLock(
        		PowerManager.SCREEN_BRIGHT_WAKE_LOCK
        		| PowerManager.ACQUIRE_CAUSES_WAKEUP
        		| PowerManager.ON_AFTER_RELEASE,
        		getClass().getName());
        
        mWifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
        mWifiLock = mWifiManager.createWifiLock(WifiManager.WIFI_MODE_FULL_HIGH_PERF, getClass().getName());
        
        hokuyo_view = (HokuyoView) findViewById(R.id.hokuyo_view);
        
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
		System.out.println(item.getTitle());
		
		if(item.getTitle().equals("Start motion control")) {
			accel = new HandleAccelerometer();
			mSensorManager.registerListener(accel, mGravity, SensorManager.SENSOR_DELAY_GAME);
			if (motion_speed_publ == null)
				motion_speed_publ = new MotionSpeedPublish(mGravity.getMaximumRange(),appDomain);
			motion_speed_publ.start();
			speed_publ_item = item;
			this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
			item.setTitle("Stop motion control");
		}
		else if (item.getTitle().equals("Stop motion control")) {
			mSensorManager.unregisterListener(accel);
			motion_speed_publ.cancel();
			this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
			item.setTitle("Start motion control");
		}
		else if (item.getTitle().equals("Start motion monitor")) {
			if (motion_speed_subs == null)
				motion_speed_subs = new MotionSpeedSubscribe(appDomain, hokuyo_view);
			motion_speed_subs.start();
			hokuyo_view.runMotion(true);
			hokuyo_view.invalidate();
			speed_subs_item = item;
			item.setTitle("Stop motion monitor");
		}
		else if (item.getTitle().equals("Stop motion monitor")) {
			hokuyo_view.runMotion(false);
			motion_speed_subs.cancel();
			hokuyo_view.invalidate();
			item.setTitle("Start motion monitor");
		}
		else if (item.getTitle().equals("Start LRF")) {
			if (hokuyo_scan == null)
				hokuyo_scan = new HokuyoScanSubscribe(appDomain, hokuyo_view);
			hokuyo_scan.start();
			hokuyo_view.run(true);
			hokuyo_view.invalidate();
			hokuyo_item = item;
			item.setTitle("Stop LRF");
		}
		else if (item.getTitle().equals("Stop LRF")) {
			hokuyo_view.run(false);
			hokuyo_scan.cancel();
			hokuyo_view.invalidate();
			item.setTitle("Start LRF");
		}
		else if (item.getTitle().equals("Crane up")) {
			crane_cmd.send(0x100);
			item.setTitle("Crane down");
		}
		else if (item.getTitle().equals("Crane down")) {
			crane_cmd.send(0x190);
			item.setTitle("Crane up");
		}
		else if (item.getTitle().equals("Magnet on")) {
			magnet_cmd.send(1);
			item.setTitle("Magnet off");			
		}
		else if (item.getTitle().equals("Magnet off")) {
			magnet_cmd.send(0);
			item.setTitle("Magnet on");			
		}
		else if (item.getTitle().equals("Voltage monitor")) {
			if (pwr_voltage == null)
				pwr_voltage = new PwrVoltageSubscribe(appDomain, dialogHandler);
			pwr_voltage.start();
			voltageDialog.show();
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
}
