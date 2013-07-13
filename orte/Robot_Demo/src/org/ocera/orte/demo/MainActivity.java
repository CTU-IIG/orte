package org.ocera.orte.demo;

import org.ocera.orte.DomainApp;
import org.ocera.orte.JOrte;
import org.ocera.orte.Manager;
import org.ocera.orte.Publication;
import org.ocera.orte.types.DomainProp;
import org.ocera.orte.types.NtpTime;
import org.ocera.orte.types.ORTEConstant;
import org.ocera.orte.types.PublProp;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
 
public class MainActivity extends Activity {
	
	private Button start_mgr = null;
	private Button stop_mgr = null;
	private Button start_pub = null;
	private Button stop_pub = null;
	private TextView textview = null;
	
	private Manager manager = null;
	private AsyncTask<Void, Short, Void> publisher = null;
	
    private SensorManager mSensorManager = null;
    private Sensor mGravity = null;
    private SensorEventListener accel = null;
    private float accelData[] = new float[2];
    private Object lock = new Object();
    
    private PowerManager mPowerManager = null;
    private WakeLock mWakeLock = null;

    static {
    	System.loadLibrary("jorte");     
    }
    
    private class ComputeOnBackground extends AsyncTask<Void, Short, Void> {
    	
    	public static final int VMAX = 16000;
    	
    	private short[] calculateSpeed(float[] mAccel) {
    		short[] speed = new short[2];

			double r=0.15;
			double angle = 0, lenght, v, omega; 

			v = (double)mAccel[1]/mGravity.getMaximumRange();
			omega = (double)mAccel[0]/mGravity.getMaximumRange();
			lenght = Math.sqrt(Math.pow(v,2) + Math.pow(omega,2));
			/* if length is more than 1 is used unit circle */
			if (lenght >= 1) {
				/* computing new speed and omega */
				angle = Math.atan2(mAccel[1], mAccel[0]);	
				v = Math.sin(angle);
				omega = Math.cos(angle);
			}
			omega *= 2;
    		 
			speed[0] = (short)(-((v + r*omega))*VMAX);
			speed[1] = (short)(-(v - r*omega)*VMAX);
			
			return speed;
    	}

		@Override
		protected Void doInBackground(Void... arg0) {
		    NtpTime persistence = new NtpTime(3);
		    int strength = 100;
		    MyEvents events = new MyEvents();
		    
		    DomainApp appDomain = new DomainApp(ORTEConstant.ORTE_DEFAULT_DOMAIN,
                    DomainProp.defaultPropsCreate(),
                    events,
                    false);	
		    
		    HelloMsg hellomsg = new HelloMsg(appDomain,"motion_speed");
		    
		    PublProp publProp = new PublProp(hellomsg.getTopic(),
                     "motion_speed",                       		
                    persistence,
					 strength);
		    
		    Publication pub = appDomain.createPublication(publProp,
                    hellomsg);
		    
		    while(!isCancelled())
			{
		      synchronized(lock) {
		    	  short speed[] = calculateSpeed(accelData);
		    	  
		    	  hellomsg.left = speed[0];
		    	  hellomsg.right = speed[1];
		    	  pub.send(hellomsg);
		    	  publishProgress(speed[0],speed[1]);
		      }
	    	  JOrte.sleepMs(100);
			}
		    
		    pub = null;
		    System.gc();
		    appDomain = null;
		    System.gc();
		    
			return null;
		}
		
		@Override
		protected void onProgressUpdate(Short... input) {
			textview.setText("Values: left = " + input[0] + ", right = " + input[1]);
		}
	}
    
    private class HandleAccelerometer implements SensorEventListener {

		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {}

		@Override
		public void onSensorChanged(SensorEvent event) {
			 if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
				 synchronized(lock) {
					 accelData[0] = event.values[0];
					 accelData[1] = event.values[1];
				 }
			 }
		}
	}
	
    @Override
    protected void onResume() {
        super.onResume();
        
        mWakeLock.acquire();
    }

    @Override
    protected void onPause() {
        super.onPause();
        
        if (publisher != null) {
        	publisher.cancel(false);
        	publisher = null;
        }
        
        if (accel != null) {
        	mSensorManager.unregisterListener(accel);
        	accel = null;
        }
        
        manager = null;
        
        mWakeLock.release();
        
        System.gc();
    }
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
		textview = (TextView)findViewById(R.id.textView1);
	   	
		start_mgr = (Button)findViewById(R.id.button1);
		stop_mgr = (Button)findViewById(R.id.button2);
		stop_mgr.setEnabled(false);
		start_pub = (Button)findViewById(R.id.button3);
		start_pub.setEnabled(false);
		stop_pub = (Button)findViewById(R.id.button4);
		stop_pub.setEnabled(false);
    	
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mGravity = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        
        mPowerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
        mWakeLock = mPowerManager.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK, getClass().getName());

        start_mgr.setOnClickListener(new View.OnClickListener() {
		    String[] mgrs = {"192.168.1.5","192.168.1.8"};
			
		    @Override
			public void onClick(View v) {
				start_mgr.setEnabled(false);
				stop_mgr.setEnabled(true);
				start_pub.setEnabled(true);
				
				manager = new Manager(mgrs);
			}
		});

		stop_mgr.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				start_mgr.setEnabled(true);
				stop_mgr.setEnabled(false);
				start_pub.setEnabled(false);
				stop_pub.setEnabled(false);
				
				manager = null;
				System.gc();
			}
		});
		
		start_pub.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				start_pub.setEnabled(false);
				stop_pub.setEnabled(true);

				accel = new HandleAccelerometer();
				mSensorManager.registerListener(accel, mGravity, SensorManager.SENSOR_DELAY_UI);
				publisher = new ComputeOnBackground().execute();
			}
		});
		
		stop_pub.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				start_pub.setEnabled(true);
				stop_pub.setEnabled(false);

				publisher.cancel(false);
				mSensorManager.unregisterListener(accel);
				publisher = null;
				accel = null;
				System.gc();
			}
		});
    }
}