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
import org.ocera.orte.types.ORTEConstant;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

public class PwrVoltageSubscribe extends SubscriptionCallback{

    static Handler handler;
    
	private Subscription sub;
	private	PwrVoltageType voltagemsg;
	private DomainApp domainApp;
	private SubsProp subProps;
	
	private boolean isCancelled = true;
	private final ReentrantReadWriteLock controlRrwl = new ReentrantReadWriteLock(true);
	private final ReadLock rcLock = controlRrwl.readLock();
	private final WriteLock wcLock = controlRrwl.writeLock();
	
	public PwrVoltageSubscribe(DomainApp domainApp, Handler dialogHandler) {
		handler = dialogHandler;
		this.domainApp = domainApp;
		
	    NtpTime deadline      = new NtpTime(0,1288490189);
	    NtpTime minSeparation = new NtpTime(1);
	    
	    voltagemsg = new PwrVoltageType(domainApp, "pwr_voltage");
		
	    subProps = new SubsProp(voltagemsg.getTopic(),
	    						"pwr_voltage",                       		
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
			sub = domainApp.createSubscription(subProps, voltagemsg, this);
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
      if (info.getRecvStatus() == ORTEConstant.NEW_DATA) {
        Message message = handler.obtainMessage();
        Bundle bundle = new Bundle();

        bundle.putDoubleArray("voltages", ((PwrVoltageType)msg).voltage.clone());
        message.setData(bundle);
        handler.sendMessage(message);
      }
    }
}
