/*
 * PropsTable.java
 *
 * Created on 5. květen 2005, 0:04
 */

package org.ocera.orte.rtpseye;

import javax.swing.table.AbstractTableModel;
import javax.swing.table.*;
import org.ocera.orte.types.AppInfo;
import java.util.Vector;
/**
 *
 * @author luky
 */
public class AppInfoTableModel extends AbstractTableModel {
    
    
    //protected SimpleModel tableData;
    
    
    /** Creates a new instance of PropsTable */
    public AppInfoTableModel() {
    }
       
                
    public void setValueAtCurrentColumn(Object value, int row) {

        tableData[row][Constant.CURRENT_VALUE] = value;  
        fireTableDataChanged();
        return;
        
    }
    
   public void fillTableByAppInfo(AppInfo appInfo) {

       int row = 0;                

       tableData = tableWithFirstColumn;
       
       // hostId         
       setValueAtCurrentColumn(
               RtpsTree.ip2string(appInfo.getHostId()),row++);

       // appId
       setValueAtCurrentColumn(
               RtpsTree.appId4NodeName(appInfo.getAppId()),row++);
       
       // unicast IP Address
       setValueAtCurrentColumn(
               RtpsTree.ip2string(appInfo.getIpAddressTemp()),row++);
       
       // unicast port
       setValueAtCurrentColumn(String.valueOf(appInfo.getUserdataUniPort()),row++);
              
       // multicast IP Address
       setValueAtCurrentColumn(
               RtpsTree.ip2string(appInfo.getMettaTrafficMulticastIPAddressTemp()),row++);
       
       // multicast port
       setValueAtCurrentColumn(String.valueOf(appInfo.getMetatrafficUniPort()),row++);
              
       // version
       setValueAtCurrentColumn(appInfo.getProtocolVersion().toString(),row++);
               
       // vendorId
       setValueAtCurrentColumn(appInfo.getVendorId().toString(),row++);
               
       return;
   }


    public void clearTable() {
        tableData = emptyTable;
        fireTableDataChanged();
        return;
    }
            
   
   public void setColumnIdentifiers(Vector setColumnIdentifiers) {              
        /* TODO */
   }
      
    public int getRowCount() {
    
        return tableData.length;
    }
    
    public int getColumnCount() {
    
        return tableData[0].length;
    }
    
    public Object getValueAt(int row, int column) {
        
        return tableData[row][column];   
    }

    public void setValueAt(Object value,int row, int column) {
        
        tableData[row][column] = value;  
        fireTableDataChanged();
        return;
    }
    
    Object[][] tableData;
    
    Object[][] tableWithFirstColumn = new Object [][] {
                {"Host Id (IP Address)", null},
                {"Application Id", null},
                {"Unicast IP Address", null},
                {"Userdata Unicast Port", null},
                {"Mettatraffic Multicast IP Address", null},
                {"Mettatraffic Unicast Port", null},
                {"RTPS Protocol Version", null},
                {"Vendor Id", null},
                {null, null}
    };
    
    Object[][] emptyTable = new Object [][] {
                {null, null},
                {null, null},
                {null, null},
                {null, null},
                {null, null},
                {null, null},
                {null, null},
                {null, null},
                {null, null}
            };
   
    
}
