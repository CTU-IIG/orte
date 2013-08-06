package org.ocera.orte.demo;

import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.ReadLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.WriteLock;

import org.ocera.orte.DomainApp;
import org.ocera.orte.Subscription;
import org.ocera.orte.SubscriptionCallback;
import org.ocera.orte.types.MessageData;
import org.ocera.orte.types.NtpTime;
import org.ocera.orte.types.RecvInfo;
import org.ocera.orte.types.SubsProp;

public class HokuyoScanSubscribe extends SubscriptionCallback{

    public final static int IMMEDIATE    = 0x02;
    public final static int BEST_EFFORTS = 0x01;
    
	private Subscription sub;
	private HokuyoView view;
	private	HokuyoScanType hokuyomsg;
	private DomainApp domainApp;
	private SubsProp subProps;
	
	private boolean isCancelled = true;
	private final ReentrantReadWriteLock controlRrwl = new ReentrantReadWriteLock(true);
	private final ReadLock rcLock = controlRrwl.readLock();
	private final WriteLock wcLock = controlRrwl.writeLock();
	
	public HokuyoScanSubscribe(DomainApp domainApp, HokuyoView view) {
		this.view = view;
		this.domainApp = domainApp;
		
	    NtpTime deadline      = new NtpTime(10);
	    NtpTime minSeparation = new NtpTime(0);
	    
	    hokuyomsg = new HokuyoScanType(domainApp, "hokuyo_scan");
		
	    subProps = new SubsProp(hokuyomsg.getTopic(),
	    						"hokuyo_scan",                       		
	    						minSeparation,  
	    						deadline,
	    						IMMEDIATE,
	    						BEST_EFFORTS,
	    						0);
	}
	
	public void start() {
		wcLock.lock();
		try {
			isCancelled = false;
			sub = domainApp.createSubscription(subProps, hokuyomsg, this);
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
    	view.setData(((HokuyoScanType)msg).hokuyo);
	}
}