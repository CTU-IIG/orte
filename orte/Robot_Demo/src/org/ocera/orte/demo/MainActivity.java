package org.ocera.orte.demo;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import org.ocera.orte.DomainApp;
import org.ocera.orte.JOrte;
import org.ocera.orte.Publication;
import org.ocera.orte.R;
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

	Button start = null;
	Button stop = null;
	Button start_pub = null;
	Button stop_pub = null;
	TextView textview = null;
	Process manager = null;
	Thread publisher = null;
	boolean tContinue = true;
	
    private SensorManager mSensorManager;
    private PowerManager mPowerManager;
    private WakeLock mWakeLock;
    private Sensor mGravity;
    
    private AsyncTask<Void, Short, Void> accel;
    
    private short mLeft;
    private short mRight;
    
    private Object lock = new Object();
    
     static 
    {
      System.loadLibrary("jorte");     
    }
    
    private class HandleAccelerometer extends AsyncTask<Void, Short, Void> implements SensorEventListener {
        
    	public static final int VMAX = 16000;
    	//private int count = 0;
        
		@Override
		protected Void doInBackground(Void... params) {
			return null;
		}

		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {
		}

		@Override
		public void onSensorChanged(SensorEvent event) {
			 if (event.sensor.getType() != Sensor.TYPE_ACCELEROMETER)
	             return;
			 
			 //if (count > 1) {
				 calculateSpeed(event.values[0],event.values[1]);
				 //count = 0;
			 //}
			 //else
				 //count++;
		}
		
		private void calculateSpeed(float mAccelX, float mAccelY) {
	    	short right = 0;
	    	short left = 0;
	    	
			double r=0.15;
			double angle = 0, lenght, v, omega; 

			v = (double)mAccelY/mGravity.getMaximumRange();
			omega = (double)mAccelX/mGravity.getMaximumRange();
			lenght = Math.sqrt(Math.pow(v,2) + Math.pow(omega,2));
			/* if lenght si more than 1 is used unit circle */
			if (lenght >= 1) {
				/* computing new speed and omega */
				angle = Math.atan2(mAccelY, mAccelX);	
				v = Math.sin(angle);
				omega = Math.cos(angle);
			}
			omega *= 2;

			right = (short)(-((v + r*omega))*VMAX);
			left = (short)(-(v - r*omega)*VMAX);
			
			publishProgress(right, left);
		}
		
		@Override
		protected void onProgressUpdate(Short... input) {
			synchronized(lock) {
				mRight = input[0];
				mLeft = input[1];
				textview.setText("Values: left = " + input[1] + ", right = " + input[0]);
			}
		}
	}
	
	public void copyManager() {
		BufferedInputStream bis = null;
		BufferedOutputStream bos = null;
		
		String path = getFilesDir().getAbsolutePath() + "/ortemanager";
		
		if (!new File(path).exists()) {
			try {
				bis = new BufferedInputStream(getAssets().open("ortemanager"));
				bos = new BufferedOutputStream(new FileOutputStream(path));
				
				int delka;
				byte[] buffer = new byte[4096];
				
				while((delka = bis.read(buffer, 0, 4096)) > 0) {
					bos.write(buffer, 0, delka);
				}
				
				bos.flush();
				bos.close();
				bis.close();
				
				Runtime.getRuntime().exec("chmod 744 " + path);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
    @Override
    protected void onResume() {
        super.onResume();

        accel = new HandleAccelerometer().execute();
        mSensorManager.registerListener((SensorEventListener)accel, mGravity, SensorManager.SENSOR_DELAY_UI);
        
        mWakeLock.acquire();
    }

    @Override
    protected void onPause() {
        super.onPause();
        
        mSensorManager.unregisterListener((SensorEventListener)accel);
        accel.cancel(true);
        accel = null;
        
        mWakeLock.release();
    }
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
		textview = (TextView)findViewById(R.id.textView1);
	   	
		start = (Button)findViewById(R.id.button1);
		stop = (Button)findViewById(R.id.button2);
		stop.setEnabled(false);
		start_pub = (Button)findViewById(R.id.button3);
		start_pub.setEnabled(false);
		stop_pub = (Button)findViewById(R.id.button4);
		stop_pub.setEnabled(false);
    	
        // Get an instance of the SensorManager
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        // Get an instance of the PowerManager
        mPowerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
        
        // Create a bright wake lock
        mWakeLock = mPowerManager.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK, getClass().getName());
    	
        mGravity = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        
        copyManager();

		start.setOnClickListener(new View.OnClickListener() {
		    String[] orte = {getFilesDir().getAbsolutePath() + "/ortemanager","-p 192.168.1.5:192.168.1.8"};
			
		    @Override
			public void onClick(View v) {
				start.setEnabled(false);
				stop.setEnabled(true);
				try {
					manager = Runtime.getRuntime().exec(orte);
				} catch (IOException e) {
					e.printStackTrace();
				}
				start_pub.setEnabled(true);
			}
		});

		stop.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				start.setEnabled(true);
				stop.setEnabled(false);
				start_pub.setEnabled(false);
				stop_pub.setEnabled(false);
				manager.destroy();
			}
		});
		
		start_pub.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				start_pub.setEnabled(false);
				stop_pub.setEnabled(true);
				tContinue = true;
				
				publisher = new Thread(new Runnable() {
					
					@Override
					public void run() {
					    NtpTime persistence = new NtpTime(3);
					    int strength = 1;
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
					    
					    while(tContinue)
						{
					      synchronized(lock) {
					    	  hellomsg.left = mLeft;
					    	  hellomsg.right = mRight;
					      }
				    	  pub.send(hellomsg); 
				    	  System.out.println("<<  data to send: " + hellomsg);
				    	  JOrte.sleepMs(100);
				    	  System.out.println(" ");
						}
					    
					    pub = null;
					    System.gc();
					    appDomain = null;
					    System.gc();
					}
				});
				publisher.start();
			}
		});
		
		stop_pub.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				start_pub.setEnabled(true);
				stop_pub.setEnabled(false);
				tContinue = false;
			}
		});
		

    }
}