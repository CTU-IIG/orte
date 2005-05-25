/*
 * Events.java
 *
 * Created on April 27, 2005, 2:25 PM
 */

package org.ocera.orte.rtpseye;

import org.ocera.orte.types.*;

/**
 *
 * @author pokorl
 */
public class Events extends org.ocera.orte.types.DomainEvents {
    
    /** Creates a new instance of Events */
    public Events(RtpsTree rtpsTree, javax.swing.JLabel jLabelMessage) {
        super();
        this.rtpsTree = rtpsTree;
        this.jLabelMessage = jLabelMessage;
    }
    
    
    
    
// FOR TESTING PURPOSES    
    public long tempAppId;
    
    public void setTempAppId (long tempAppId) {
        this.tempAppId = tempAppId;
        return;
    }    
// FOR TESTING PURPOSES        
    
    
    public void onRegFail() 
    {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onRegFail()'..          *");
  	System.out.println(":j: *************************************************");
        rtpsTree.registrationIntoTreeFail();
        //jLabelMessage.setForeground(new java.awt.Color(255,255,255));
        jLabelMessage.setBackground(new java.awt.Color(255, 0, 0));
        jLabelMessage.setForeground(new java.awt.Color(255,0,0));        
        jLabelMessage.setText(" can't register RtpsEye to manager!");
    }

    public void onMgrNew(AppInfo appInfo) 
    {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onMgrNew()'..           *");
  	System.out.println(":j: *************************************************");
        // ///////////////////////////
        appInfo.printWithLegend();
        // ///////////////////////////
        try {
            
            rtpsTree.addNewMgr(appInfo);
            System.out.println("Events: onMgrNew called");
            return;
        }
        
        catch(NullPointerException e) {
            System.out.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            System.out.println("NullPointerException caught");
            //System.out.println("appInfo = " + appInfo);
            //appInfo.printWithLegend();
            return;
        }
    }
  
    public void onMgrDelete(AppInfo appInfo)
    {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onMgrDelete()'..        *");
  	System.out.println(":j: *************************************************");
        //
        rtpsTree.removeMgr(appInfo);
        return;
    }

    
    public void onAppRemoteNew(AppInfo appInfo)
    {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onAppRemoteNew()'..     *");
  	System.out.println(":j: *************************************************");
        //
  	appInfo.printWithLegend();
        try {
            
            rtpsTree.addNewApp(appInfo);
            System.out.println("Events: onAppRemoteNew called");            
            return;
        }
        
        catch(NullPointerException e) {
            System.out.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            System.out.println("NullPointerException caught");
            //System.out.println("appInfo = " + appInfo);
            //appInfo.printWithLegend();
            return;
        }
        
    }

    public void onAppDelete(AppInfo appInfo) 
    {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onAppDelete()'..        *");
  	System.out.println(":j: *************************************************");
        //        
        rtpsTree.removeApp(appInfo);
    }

    public void onPubRemoteNew(AppInfo appInfo, PubInfo pubInfo)
    {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onPubRemoteNew()'..     *");
  	System.out.println(":j: *************************************************");

        try {
            
            rtpsTree.addNewPub(appInfo, pubInfo);            
            return;
        }
        
        catch(NullPointerException e) {
            System.out.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            System.out.println("NullPointerException caught");
            //System.out.println("appInfo = " + appInfo);
            //appInfo.printWithLegend();
            return;
        }
        
    }

    public void onPubRemoteChanged(AppInfo appInfo, PubInfo pubInfo)
    {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onPubRemoteChanged()'.. *");
  	System.out.println(":j: *************************************************");
        // 
  	//appInfo.printWithLegend();	
        //pubInfo.printWithLegend();
    }

    public void onPubDelete(AppInfo appInfo, PubInfo pubInfo)
    {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onPubDelete()'..        *");
  	System.out.println(":j: *************************************************");
        // 
        rtpsTree.removePubFromTree(appInfo, pubInfo);
    }
  
  
    public void onSubRemoteNew(AppInfo appInfo, SubInfo subInfo)
    {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onSubRemoteNew()'..     *");
  	System.out.println(":j: *************************************************");
        // 
        try {
            
            rtpsTree.addNewSub(appInfo, subInfo);            
            return;
        }
        
        catch(NullPointerException e) {
            System.out.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            System.out.println("NullPointerException caught");
            //System.out.println("appInfo = " + appInfo);
            //appInfo.printWithLegend();
            return;
        }
        
    }

    public void onSubRemoteChanged(AppInfo appInfo, SubInfo subInfo)
    {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onSubRemoteChanged()'.. *");
  	System.out.println(":j: *************************************************");
        // 
  	//appInfo.printWithLegend();	
        //subInfo.printWithLegend();  
    }

    public void onSubDelete(AppInfo appInfo, SubInfo subInfo)
    {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onSubDelete()'..        *");
  	System.out.println(":j: *************************************************");
        // 
        rtpsTree.removeSubFromTree(appInfo, subInfo);                
    }
    
    // variables
    private javax.swing.JLabel jLabelMessage;
    private org.ocera.orte.rtpseye.RtpsTree rtpsTree;
    
}
