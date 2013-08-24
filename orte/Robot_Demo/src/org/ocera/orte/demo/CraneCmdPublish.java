package org.ocera.orte.demo;

import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.ReadLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.WriteLock;

import org.ocera.orte.DomainApp;
import org.ocera.orte.Publication;
import org.ocera.orte.types.NtpTime;
import org.ocera.orte.types.PublProp;

public class CraneCmdPublish {
	
	private boolean isCancelled = true;
	private CraneCmdType liftmsg;
	private Publication pub;
	private PublProp publProp;
	private DomainApp appDomain;
	
	private final ReentrantReadWriteLock controlRrwl = new ReentrantReadWriteLock(true);
	private final ReadLock rcLock = controlRrwl.readLock();
	private final WriteLock wcLock = controlRrwl.writeLock();
	
	public CraneCmdPublish(DomainApp appDomain) {
		this.appDomain = appDomain;
		
	    NtpTime persistence = new NtpTime(3);
	    int strength = 100;
	    
	    liftmsg = new CraneCmdType(appDomain,"crane_cmd");
	    
	    publProp = new PublProp(liftmsg.getTopic(),
	    								 "crane_cmd",                       		
	    								 persistence,
	    								 strength);
	}
	
	public void start() {
		wcLock.lock();
		try {
			isCancelled = false;
		    pub = appDomain.createPublication(publProp, liftmsg);
		}
		finally {
			wcLock.unlock();
		}
	}
	
	public void cancel() {
		wcLock.lock();
		try {
			isCancelled = true;
			pub.destroy();
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
	
	public void send(short magnet) {
		liftmsg.position = magnet;
		pub.send(liftmsg);
	}
}
