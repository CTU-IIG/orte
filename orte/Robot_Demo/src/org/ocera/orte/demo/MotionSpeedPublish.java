package org.ocera.orte.demo;

import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.ReadLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.WriteLock;

import org.ocera.orte.DomainApp;
import org.ocera.orte.Publication;
import org.ocera.orte.types.NtpTime;
import org.ocera.orte.types.PublProp;

public class MotionSpeedPublish implements Runnable {
	
	public static final int VMAX = 16000;
	public static final double r = 0.15;
	public final float alpha = 0.8f;
	
	private short[] speed = new short[2]; 
	private boolean isCancelled = true;
	private float a2sRatio = 9.81f;
	private float[] accelData = new float[2];
	private float[] accelNew = new float[2];
	private SpeedMotionType speedmsg;
	private Publication pub;
	private Thread thread = null;
	private PublProp publProp;
	private DomainApp appDomain;
	
	private final ReentrantLock dataLock = new ReentrantLock();
	private final ReentrantReadWriteLock controlRrwl = new ReentrantReadWriteLock(true);
	private final ReadLock rcLock = controlRrwl.readLock();
	private final WriteLock wcLock = controlRrwl.writeLock();
	
	public MotionSpeedPublish(DomainApp appDomain) {
		this.appDomain = appDomain;
		
	    NtpTime persistence = new NtpTime(3);
	    int strength = 100;
	    
	    speedmsg = new SpeedMotionType(appDomain,"motion_speed");
	    
	    publProp = new PublProp(speedmsg.getTopic(),
	    								 "motion_speed",                       		
	    								 persistence,
	    								 strength);
	}
	
	private void calculateSpeed(float[] mAccel) {

		double angle = 0, length, v, omega;

		v = (double)mAccel[1]/a2sRatio;
		omega = (double)mAccel[0]/a2sRatio;
		length = Math.sqrt(Math.pow(v,2) + Math.pow(omega,2));
		if (length >= 1) {
			angle = Math.atan2(mAccel[1], mAccel[0]);	
			v = Math.sin(angle);
			omega = Math.cos(angle);
		}
		omega *= 2;
		 
		speed[0] = (short)(-(v + (v>0 ? -r*omega : r*omega))*VMAX);
		speed[1] = (short)(-(v - (v>0 ? -r*omega : r*omega))*VMAX);
	}
	
	public void start() {
		wcLock.lock();
		try {
			isCancelled = false;
		    pub = appDomain.createPublication(publProp, speedmsg);
		    thread = new Thread(this);
		    thread.start();
		}
		finally {
			wcLock.unlock();
		}
	}
	
	public void cancel() {
		wcLock.lock();
		try {
			isCancelled = true;
			rcLock.lock();
		} finally {
			wcLock.unlock();
		}
		try {
			thread.join();
		    pub.destroy();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} finally {
			rcLock.unlock();
		}
	}
	
	public boolean isCancelled() {
		rcLock.lock();
		try {
			return isCancelled;
		} finally {
			rcLock.unlock();
		}
	}
	
	@Override
	public void run() {
		while(true) {
			rcLock.lock();
			try {
			    if (!isCancelled) {
			      dataLock.lock();
			      try {
			    	  accelData[0] = alpha * accelData[0] + (1 - alpha) * accelNew[0];
			    	  accelData[1] = alpha * accelData[1] + (1 - alpha) * accelNew[1];

			    	  calculateSpeed(accelData);
			      }
			      finally {
			    	  dataLock.unlock();
			      }
		    	  speedmsg.speed = this.speed;  
			      pub.send(speedmsg);
				}
			    else
			    	break;
			} finally {
				rcLock.unlock();
			}
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	public void setSpeed(float accelX, float accelY) {
		if (dataLock.tryLock()) {
			try {
				this.accelNew[0] = accelX;
				this.accelNew[1] = accelY;
			}
			finally {
				dataLock.unlock();
			}
		}
	}
}
