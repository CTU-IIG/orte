/*
 * CommonPubSubInfo.java
 *
 * Created on 10. květen 2005, 11:15
 */

package org.ocera.orte.types;

/**
 *
 * @author luky
 */
public class CommonPubSubInfo {
    
    /** Creates a new instance of CommonPubSubInfo */
    public CommonPubSubInfo() {
    }

    public String getTopic() {
        return this.topic;
    }
  
    public String getTypeName() {
        return this.typeName;
    }
    
    // variables
    private String topic;
    private String typeName;          
}
