/*
 * RtpsTreeNode.java
 *
 * Created on 6. květen 2005, 10:54
 */

package org.ocera.orte.rtpseye;

import org.ocera.orte.types.*;
/**
 *
 * @author luky
 */
public class NodeInfo {
    
    
    /** Creates a new instance of RtpsTreeNode */
    public NodeInfo() {

            appInfo = new AppInfo();
            psInfoArray  = new CommonPubSubInfo[Constant.DEFAULT_DEPTH];
    }
    
           
    /** 
     * Returns current tree node's AppInfo
     * @return appInfo 
     */
    public AppInfo getAppInfo() {
        return this.appInfo;
    }
    
    /** 
     * Set tree node's appInfo
     * @param appInfo 
     * @return void
     */
    public void setAppInfo(AppInfo appInfo) {
        this.appInfo = appInfo;
        return;
    }

    
    /** 
     * Returns current tree node's Pub/SubInfo (if instance exists)
     * @return pub/subInfo 
     */
    public CommonPubSubInfo getPSInfo(int index) {
        if(index >= psInfoArray.length || index < 0) return null;
        return this.psInfoArray[index];
    }

    

    public void addNextPSInfo(CommonPubSubInfo newPSInfo) {
        
        for(int i = 0; i < this.psInfoArray.length; i++){            
            // find empty cell
            if(this.psInfoArray[i] == null) {
                offset = i;
                break;
            }             
        }
        // not enough cells in array
        if(offset == 0) {
            CommonPubSubInfo[] arrayTemp = 
                    new CommonPubSubInfo[this.psInfoArray.length + 1];
            // copy all from old array
            for(int i = 0; i < this.psInfoArray.length; i++) {
                arrayTemp[i] = this.psInfoArray[i];
            }                        
            arrayTemp[offset] = newPSInfo;
            this.psInfoArray = arrayTemp;
        }
        else this.psInfoArray[offset] = newPSInfo;
        return;        
    }
    
    
    
    public String toString() {
        if(appInfo == null) return("null");
        return(" ok ");
    }
    

    /** 
     * Set tree node's pub/subInfo
     * @param commonPubSubInfo 
     * @param index - position in psInfo array
     * @return void
     */
    public void setPSInfo(CommonPubSubInfo psInfo, int index) {
        this.psInfoArray[index] = psInfo;
        return;
    }
            
    // variables
    private org.ocera.orte.types.AppInfo appInfo;
    private org.ocera.orte.types.CommonPubSubInfo[] psInfoArray;
    int offset;
    // constants
    
    
}
