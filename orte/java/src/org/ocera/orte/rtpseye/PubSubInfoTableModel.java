/*
 * PubSubInfoTableModel.java
 *
 * Created on 10. květen 2005, 15:15
 */

package org.ocera.orte.rtpseye;

import javax.swing.table.AbstractTableModel;
import javax.swing.table.*;
import org.ocera.orte.types.AppInfo;
import java.util.Vector;
import org.ocera.orte.types.CommonPubSubInfo;
/**
 *
 * @author luky
 */
public class PubSubInfoTableModel extends AbstractTableModel {
    
    /** Creates a new instance of PubSubInfoTableModel */
    public PubSubInfoTableModel() {
                
    }
    
            
    public void setValueAtCurrentColumn(Object value, int row) {

        tableData[row][Constant.CURRENT_VALUE] = value;  
        fireTableDataChanged();
        return;
        
    }
    
   public void fillTableByPSInfo(CommonPubSubInfo info) {

       int row = 0;                
       tableData = tableWithFirstColumn;                
       // type 
       setValueAtCurrentColumn(type,row++);
       // type name

       setValueAtCurrentColumn(info.getTypeName(),row++);
       // topic
       setValueAtCurrentColumn(info.getTopic(),row++);

       return;
   }


   public void clearTable() {
       tableData = emptyTable;
       fireTableDataChanged();
       return;
   }
            
   public void setType(String type) {
       this.type = type;
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
    
    // variables    
    private String type;
    
    Object[][] tableData;
    
    Object[][] tableWithFirstColumn = new Object [][] {
                {"type", null},
                {"type name", null},
                {"topic", null},                        
                {null, null}
    };

    Object[][] emptyTable = new Object [][] {
                {null, null},
                {null, null},
                {null, null},
                {null, null}
    };
    
    
}

