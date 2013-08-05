package org.ocera.orte.demo;

import org.ocera.orte.DomainApp;
import org.ocera.orte.JOrte;
import org.ocera.orte.Publication;
import org.ocera.orte.types.NtpTime;
import org.ocera.orte.types.PublProp;

public class MotionSpeedPublish implements Runnable {
	
	public static final int VMAX = 16000;
	public static final double r = 0.15;
	
	private short[] speed = new short[2];
	private boolean isCancelled = false;
	private float maxRange;
	private float[] accelData = new float[2];
	private SpeedMotionType speedmsg;
	private Publication pub;
	private Object lock = new Object();
	private Thread thread = null;
	private PublProp publProp;
	private DomainApp appDomain;
	
	public MotionSpeedPublish(float maxRange, DomainApp appDomain) {
		this.maxRange = maxRange;
		this.appDomain = appDomain;
		
	    NtpTime persistence = new NtpTime(3);
	    int strength = 100;
	    
	    speedmsg = new SpeedMotionType(appDomain,"motion_speed");
	    
	    publProp = new PublProp(speedmsg.getTopic(),
	    								 "motion_speed",                       		
	    								 persistence,
	    								 strength);
	    

	}
	
	public void start() {
		isCancelled = false;
	    pub = appDomain.createPublication(publProp, speedmsg);
	    thread = new Thread(this);
	    thread.start();
	}
	
	private void calculateSpeed(float[] mAccel) {

		double angle = 0, lenght, v, omega; 

		v = (double)mAccel[1]/maxRange;
		omega = (double)mAccel[0]/maxRange;
		lenght = Math.sqrt(Math.pow(v,2) + Math.pow(omega,2));
		if (lenght >= 1) {
			angle = Math.atan2(mAccel[1], mAccel[0]);	
			v = Math.sin(angle);
			omega = Math.cos(angle);
		}
		omega *= 2;
		 
		speed[0] = (short)(-(v + (v>0 ? -r*omega : r*omega))*VMAX);
		speed[1] = (short)(-(v - (v>0 ? -r*omega : r*omega))*VMAX);
	}
	
	public void cancel() {
		isCancelled = true;
		try {
			thread.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	    pub.destroy();
	}
	
	public boolean isCancelled() {
		return isCancelled;
	}
	
	@Override
	public void run() {
	    while(!isCancelled) {
	      synchronized(lock) {
	    	  calculateSpeed(accelData);
	    	  speedmsg.speed = this.speed;
	    	  
	    	  pub.send(speedmsg);
	      }
	      
    	  JOrte.sleepMs(100);
		}
	}
	
	public void setSpeed(float accelX, float accelY) {
		synchronized(lock) {
			this.accelData[0] = accelX;
			this.accelData[1] = accelY;
		}
	}
	
}