package org.ocera.orte.demo;

import org.ocera.orte.DomainApp;
import org.ocera.orte.Manager;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.view.Menu;
import android.view.MenuItem;
 
public class MainActivity extends Activity {
	
	private Manager manager = null;
    private String[] mgrs = {"192.168.1.5","192.168.1.8"};
	private MotionSpeedPublish motion_speed = null;
	private HokuyoScanSubscribe hokuyo_scan = null;
    private SensorManager mSensorManager = null;
    private Sensor mGravity = null;
    private SensorEventListener accel = null;
    private PowerManager mPowerManager = null;
    private WakeLock mWakeLock = null;
    private DomainApp appDomain = null;
    private HokuyoView hokuyo_view = null;
    private MenuItem speed_item = null;
    private MenuItem hokuyo_item = null;

    static {
    	System.loadLibrary("jorte");     
    }

    @Override
    protected void onResume() {
        super.onResume();

        mWakeLock.acquire();
    }

    @Override
    protected void onPause() {
        super.onPause();
        
        if (motion_speed != null && !motion_speed.isCancelled()) {
        	motion_speed.cancel();
            mSensorManager.unregisterListener(accel);
        	speed_item.setTitle("Start speed");
        }

        if (hokuyo_scan != null && !hokuyo_scan.isCancelled()) {
			hokuyo_view.run(false);
			hokuyo_scan.cancel();
			hokuyo_item.setTitle("Start hokuyo");
        }
        
        mWakeLock.release();
    }
    
    @Override
    protected void onDestroy() {
    	super.onDestroy();
    	
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
        
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mGravity = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        
        mPowerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
        mWakeLock = mPowerManager.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK, getClass().getName());
        
        hokuyo_view = (HokuyoView) findViewById(R.id.hokuyo_view);
        
        manager = new Manager(mgrs);

        appDomain = new DomainApp();
    }
    
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.activity_main, menu);

		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected (MenuItem item) {
		System.out.println(item.getTitle());
		
		if(item.getTitle().equals("Start speed")) {
			accel = new HandleAccelerometer();
			mSensorManager.registerListener(accel, mGravity, SensorManager.SENSOR_DELAY_UI);
			if (motion_speed == null)
				motion_speed = new MotionSpeedPublish(mGravity.getMaximumRange(),appDomain);
			motion_speed.start();
			speed_item = item;
			item.setTitle("Stop speed");
		}
		else if (item.getTitle().equals("Stop speed")) {
			mSensorManager.unregisterListener(accel);
			motion_speed.cancel();
			item.setTitle("Start speed");
		}
		else if (item.getTitle().equals("Start hokuyo")) {
			if (hokuyo_scan == null)
				hokuyo_scan = new HokuyoScanSubscribe(appDomain, hokuyo_view);
			hokuyo_scan.start();
			hokuyo_view.run(true);
			hokuyo_item = item;
			item.setTitle("Stop hokuyo");
		}
		else if (item.getTitle().equals("Stop hokuyo")) {
			hokuyo_view.run(false);
			hokuyo_scan.cancel();
			hokuyo_view.invalidate();
			item.setTitle("Start hokuyo");
		}
		else if (item.getTitle().equals("Lift up")) {
			
		}
		else if (item.getTitle().equals("Lift down")) {
			
		}
		else if (item.getTitle().equals("Magnet on")) {
			
		}
		else if (item.getTitle().equals("Magnet off")) {
			
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
    			 if (motion_speed != null)
    				 motion_speed.setSpeed(event.values[0], event.values[1]);
    		 }
    	}
    }
}