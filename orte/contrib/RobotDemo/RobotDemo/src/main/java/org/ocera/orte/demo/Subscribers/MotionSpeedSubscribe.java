package org.ocera.orte.demo.Subscribers;

import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.ReadLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.WriteLock;

import org.ocera.orte.DomainApp;
import org.ocera.orte.Subscription;
import org.ocera.orte.SubscriptionCallback;
import org.ocera.orte.demo.DataTypes.SpeedMotionType;
import org.ocera.orte.demo.GUI.HokuyoView;
import org.ocera.orte.types.MessageData;
import org.ocera.orte.types.NtpTime;
import org.ocera.orte.types.RecvInfo;
import org.ocera.orte.types.SubsProp;
import org.ocera.orte.types.ORTEConstant;

public class MotionSpeedSubscribe extends SubscriptionCallback{

	private Subscription sub;
	private HokuyoView view;
	private	SpeedMotionType speedmsg;
	private DomainApp domainApp;
	private SubsProp subProps;
	
	private boolean isCancelled = true;
	private final ReentrantReadWriteLock controlRrwl = new ReentrantReadWriteLock(true);
	private final ReadLock rcLock = controlRrwl.readLock();
	private final WriteLock wcLock = controlRrwl.writeLock();
	
	public MotionSpeedSubscribe(DomainApp domainApp, HokuyoView view) {
		this.view = view;
		this.domainApp = domainApp;
		
	    NtpTime deadline      = new NtpTime(0,1288490189);
	    NtpTime minSeparation = new NtpTime(0);
	    
	    speedmsg = new SpeedMotionType(domainApp, "motion_speed");
		
	    subProps = new SubsProp(speedmsg.getTopic(),
	    						"motion_speed",                       		
	    						minSeparation,  
	    						deadline,
	    						ORTEConstant.IMMEDIATE,
	    						ORTEConstant.BEST_EFFORTS,
	    						0);
	}
	
	public void start() {
		wcLock.lock();
		try {
			isCancelled = false;
			sub = domainApp.createSubscription(subProps, speedmsg, this);
		} finally {
			wcLock.unlock();
		}
	}
	
	public void cancel() {
		wcLock.lock();
		try {
			isCancelled = true;
			sub.destroy();
		} finally {
			wcLock.unlock();
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
	
    public void callback(RecvInfo info, MessageData msg) {
      if (info.getRecvStatus() == ORTEConstant.NEW_DATA)
        view.setDataMotion(((SpeedMotionType)msg).speed);
    }
}
