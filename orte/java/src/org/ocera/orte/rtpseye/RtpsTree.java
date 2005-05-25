/*
 * OrteTree.java
 *
 * Created on April 27, 2005, 1:09 PM
 */

package org.ocera.orte.rtpseye;

import java.awt.*;
import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.JTable;
import javax.swing.tree.DefaultMutableTreeNode;
import org.ocera.orte.types.AppInfo;
import org.ocera.orte.types.PubInfo;
import org.ocera.orte.types.SubInfo;
import org.ocera.orte.rtpseye.AppInfoTableModel;

/**
 *
 * @author pokorl
 */
public class RtpsTree {
    

    // variables    
    private javax.swing.JTree tree;
    private javax.swing.tree.DefaultTreeModel treeModel;    
    private javax.swing.tree.DefaultMutableTreeNode root;
    //
    private javax.swing.JTable jTableAppInfo;
    private org.ocera.orte.rtpseye.AppInfoTableModel appInfoTableModel;
    //
    private javax.swing.JTable jTablePSInfo;
    private org.ocera.orte.rtpseye.PubSubInfoTableModel psInfoTableModel;
    //
    private org.ocera.orte.rtpseye.NodeInfo[][] nodeInfoMatrix;    
    //    
    private boolean flag_runOnlyOnes;
    private int rowIndex;
    private int columnIndex;
    private int depthIndex;
            
    private int controlCount;
        
    /** Creates a new instance of RtpsTree */
    public RtpsTree(javax.swing.JTree tree, 
                    javax.swing.JTable jTableAppInfo, 
                    javax.swing.JTable jTablePSInfo) {
        // tree
        this.tree = tree;
        this.treeModel = (DefaultTreeModel) tree.getModel();         
        this.root = (DefaultMutableTreeNode) this.treeModel.getRoot(); 
        // tables
        this.jTableAppInfo = jTableAppInfo;
        appInfoTableModel = new AppInfoTableModel();
        this.jTablePSInfo = jTablePSInfo;
        psInfoTableModel = new PubSubInfoTableModel();
        //
        flag_runOnlyOnes = false;
        controlCount = 0;
        nodeInfoMatrix = 
            new NodeInfo[Constant.DEFAULT_ROW_COUNT][Constant.DEFAULT_COLUMN_COUNT];                                       
    }

    public void clearBothTables() {
        appInfoTableModel.clearTable();
        psInfoTableModel.clearTable();        
    }
    
    
    public void nodeSelected(javax.swing.event.TreeSelectionEvent event) {
        
        this.treeModel = (DefaultTreeModel) tree.getModel();         
        // get selected path
        TreePath pathToSelectedNode = event.getPath();
        // get selected node
        DefaultMutableTreeNode selectedNode = (DefaultMutableTreeNode) 
                pathToSelectedNode.getLastPathComponent();
        String nodeName = selectedNode.toString();
        
        // root node selected
        if(selectedNode == treeModel.getRoot()) {            
            clearBothTables();  
            return; 
        }            
        
        // get parent node of selected node
        DefaultMutableTreeNode parent = (DefaultMutableTreeNode)
                selectedNode.getParent();        
        
        // hostId node selected              
        if(parent == treeModel.getRoot()) {
            clearBothTables();
            return;
        }                                        
        
        // appId node selected        
        if(nodeName.startsWith("mgr") || nodeName.startsWith("app")) {        
            clearBothTables();
            this.columnIndex = this.treeModel.getIndexOfChild(parent, selectedNode);
            this.rowIndex = this.treeModel.getIndexOfChild(parent.getParent(), parent);
            NodeInfo nodeInfo = nodeInfoMatrix[rowIndex][columnIndex];                
            appInfoTableModel.fillTableByAppInfo(nodeInfo.getAppInfo());
            /* TODO vymyslet, jak nastavit spravne jmena sloupcu (columnIdentifiers) */
            jTableAppInfo.setModel(appInfoTableModel);             
            return;
        }
        
        // pub/sub node selected
        this.rowIndex = this.treeModel.getIndexOfChild(treeModel.getRoot(), parent.getParent());            
        this.depthIndex = this.treeModel.getIndexOfChild(parent, selectedNode);
        this.columnIndex = this.treeModel.getIndexOfChild(parent.getParent(), parent);                        
        
        NodeInfo nodeInfo = nodeInfoMatrix[rowIndex][columnIndex];                
        appInfoTableModel.fillTableByAppInfo(nodeInfo.getAppInfo());        
        jTableAppInfo.setModel(appInfoTableModel);
        System.out.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
        System.out.println("node selected: row: " + rowIndex + " column: " + columnIndex + " depth: " + depthIndex);
                
        if(nodeName.startsWith("P")) psInfoTableModel.setType("publication");
        if(nodeName.startsWith("S")) psInfoTableModel.setType("subscription");
        psInfoTableModel.fillTableByPSInfo(nodeInfo.getPSInfo(this.depthIndex));                
        jTablePSInfo.setModel(psInfoTableModel);
        return;        
    }
    
                
    /** 
     * Registration of RTPSLook failed. Change root node "registration fault".
     */        
    public void registrationIntoTreeFail() {
        
        treeModel.setRoot(new DefaultMutableTreeNode("not available"));        
        return;
    }            

    
    /** 
     * Creates a new manager's node in a tree
     * @param appInfo AppInfo instance
     * @return void
     */    
    public void addNewMgr(AppInfo appInfo) {
                                        
        System.out.println("addMgrNew() called");
        createRtpsTree(appInfo,null,null);
        return;
    }

    
    /** 
     * Creates a new manager's node in a tree
     * @param appInfo AppInfo instance
     * @return void
     */    
    public void removeMgr(AppInfo appInfo) {
                                               
        System.out.println("remMgr() called");
        
        DefaultMutableTreeNode hostIdNode = null;
        DefaultMutableTreeNode appIdNode = null;
        
        // hostId
        String nodeName = String.valueOf(ip2string(appInfo.getHostId()));
        int index = ManageTree.getNodeIndexByName(this.tree,this.root,nodeName);        
        if(index < 0) {
            System.out.println(":: rem Mgr [bad  hostId]");
            return;
        }
        hostIdNode = (DefaultMutableTreeNode) this.treeModel.getChild(this.root,index);   
        // appId
        nodeName = appId4NodeName(appInfo.getAppId());
        index = ManageTree.getNodeIndexByName(this.tree,hostIdNode,nodeName);
        if(index < 0) {
            System.out.println(":: rem Mgr [bad  appId]");
            return;
        }
        appIdNode = (DefaultMutableTreeNode) this.treeModel.getChild(hostIdNode,index);
        
        this.treeModel = (DefaultTreeModel) tree.getModel();
        this.treeModel.removeNodeFromParent(appIdNode);
        this.treeModel.nodeStructureChanged(hostIdNode);
        this.treeModel.reload();

        return;
    }
    
                
    /** 
     * Creates a new application's node in a tree
     * @param appInfo AppInfo instance
     * @return void
     */    
     public void addNewApp(AppInfo appInfo) {
                
        System.out.println("addAppRemoteNew() called");
        createRtpsTree(appInfo,null,null);
        return;
     }

     
     public void removeApp(AppInfo appInfo) {
         
        DefaultMutableTreeNode hostIdNode = null;
        DefaultMutableTreeNode appIdNode = null;
        
        // hostId
        String nodeName = String.valueOf(ip2string(appInfo.getHostId()));
        int index = ManageTree.getNodeIndexByName(this.tree,this.root,nodeName);
        if(index < 0) {
            System.out.println(":: rem App [bad  hostId]");
            return;
        }
        hostIdNode = (DefaultMutableTreeNode) this.treeModel.getChild(this.root,index);   
        // appId
        nodeName = appId4NodeName(appInfo.getAppId());
        index = ManageTree.getNodeIndexByName(this.tree,hostIdNode,nodeName);
        if(index < 0) {
            System.out.println(":: rem App [bad  appId]");
            return;
        }
        appIdNode = (DefaultMutableTreeNode) this.treeModel.getChild(hostIdNode,index);
        
        this.treeModel = (DefaultTreeModel) tree.getModel();
        this.treeModel.removeNodeFromParent(appIdNode);
        this.treeModel.nodeStructureChanged(hostIdNode);
        this.treeModel.reload();

                     
        return;
     }
          
          
    /** 
     * Creates a new publication's node in application's subtree
     * @param appInfo a AppInfo instance
     * @param pubInfo a PubInfo instance
     * @return void
     */    
     public void addNewPub(AppInfo appInfo, PubInfo pubInfo) {
                
        System.out.println("addAppRemoteNew() called");
        createRtpsTree(appInfo,pubInfo,null);
        return;
     }

     
     public void removePub(AppInfo appInfo, PubInfo pubInfo) {
                
        System.out.println("addAppRemoteNew() called");
        removePubFromTree(appInfo,pubInfo);        
        return;
     }
    

    /** 
     * Creates a new publication's node in application's subtree
     * @param appInfo a AppInfo instance
     * @param subInfo a SubInfo instance
     * @return void
     */        
    public void addNewSub(AppInfo appInfo, SubInfo subInfo) {
                
        System.out.println("addSubRemoteNew() called");
        createRtpsTree(appInfo,null,subInfo);
        return;
    }
    
    
    /** 
     * Creates a new publication's node in application's subtree
     * @param appInfo a AppInfo instance
     * @param subInfo a SubInfo instance
     * @return void
     */                
    public void createRtpsTree(AppInfo appInfo, 
            PubInfo pubInfo, SubInfo subInfo) {
        
        DefaultMutableTreeNode hostIdNode = null;
        DefaultMutableTreeNode appIdNode = null;
        DefaultMutableTreeNode pubNode = null;
        DefaultMutableTreeNode subNode = null;
        
        int row = -1;
        int column = -1;
        int depth = -1;

        
        
        
        
        NodeInfo nodeInfo = new NodeInfo();
        
        // run only at first
        if(flag_runOnlyOnes == false) {
            DefaultMutableTreeNode root = 
                    ManageTree.createRootNodeByName("domain");
            treeModel.setRoot(root);
            flag_runOnlyOnes = true;
            this.root = (DefaultMutableTreeNode) treeModel.getRoot();
        }
        // get current tree model
        this.treeModel = (DefaultTreeModel) tree.getModel();
        
        // hostId node
        // check if node exists
        String nodeName = String.valueOf(ip2string(appInfo.getHostId()));
        int index = ManageTree.getNodeIndexByName(this.tree,this.root,nodeName);
        
        if(index < 0) {
            // hostId node doesn't exist -> create it
            hostIdNode = new DefaultMutableTreeNode(nodeName);
            this.treeModel.insertNodeInto(hostIdNode,this.root,this.root.getChildCount());
        } 
        else {
            hostIdNode = (DefaultMutableTreeNode) this.treeModel.getChild(this.root,index);    
        }           
        row = treeModel.getIndexOfChild(treeModel.getRoot(), hostIdNode);
        // appId node 
        // check if node exists
        index = ManageTree.getNodeIndexByName(this.tree,hostIdNode,appId4NodeName(appInfo.getAppId()));     
        if(index < 0) {
            // appId node doesn't exist
            appIdNode  = new DefaultMutableTreeNode(appId4NodeName(appInfo.getAppId()));
            this.treeModel.insertNodeInto(appIdNode, hostIdNode, hostIdNode.getChildCount());
        }
        else {
            appIdNode = (DefaultMutableTreeNode) this.treeModel.getChild(hostIdNode,index);                
        }    
        column = treeModel.getIndexOfChild(hostIdNode, appIdNode);
        // set AppInfo
        nodeInfo.setAppInfo(appInfo);
        // pub/sub node
        if(pubInfo != null) {
            pubNode = new DefaultMutableTreeNode("Pub:" + pubInfo.getTypeName());
            this.treeModel.insertNodeInto(pubNode, appIdNode, appIdNode.getChildCount());
            depth = treeModel.getIndexOfChild(appIdNode, pubNode);
            nodeInfo.setPSInfo(pubInfo, depth);            
        }
        if(subInfo != null) {
            subNode = new DefaultMutableTreeNode("Sub:" + subInfo.getTypeName());
            this.treeModel.insertNodeInto(subNode, appIdNode, appIdNode.getChildCount());
            depth = treeModel.getIndexOfChild(appIdNode, subNode);
            nodeInfo.setPSInfo(subInfo, depth);            
        }                                                                     
        this.treeModel.reload();
        this.tree.expandRow(0); // expand root node
        nodeInfoMatrix[row][column] = nodeInfo;        
        System.out.println((++controlCount) + ". save appInfo into matrix on position " + " row: " + 
                row + " column: " + column + " depth: " + depth);                
        return;
    }       
  
    
    /* TODO zjistit, co se muze zmenit pri changePub jestli jen topic nebo i 
       TypeName tj. jestli muze Pub produkovat i jina data
     **/
    
    public void changePub(AppInfo appInfo, PubInfo pubInfo) {
        
        DefaultMutableTreeNode hostIdNode = null;
        DefaultMutableTreeNode appIdNode = null;
        
        // hostId
        String nodeName = String.valueOf(ip2string(appInfo.getHostId()));
        int index = ManageTree.getNodeIndexByName(this.tree,this.root,nodeName);
        if(index < 0) {
            System.out.println(":: change Pub [bad  hostId]");
            return;
        }
        hostIdNode = (DefaultMutableTreeNode) this.treeModel.getChild(this.root,index);   
        // appId
        nodeName = appId4NodeName(appInfo.getAppId());
        index = ManageTree.getNodeIndexByName(this.tree,hostIdNode,nodeName);
        if(index < 0) {
            System.out.println(":: change Pub [bad  appId]");
            return;
        }
        appIdNode = (DefaultMutableTreeNode) this.treeModel.getChild(hostIdNode,index);        
        // pub
/*
        nodeName = pub4NodeName(pubInfo);
        index = ManageTree.getNodeIndexByName(this.tree,appIdNode,nodeName);        
        if(index < 0) {
            System.out.println(":: change Pub [bad  Pub]");
            return;
        }
        
*/        
        
        return;
    }
    
    
    
    public void removePubFromTree(AppInfo appInfo, PubInfo pubInfo) {
        
        DefaultMutableTreeNode hostIdNode = null;
        DefaultMutableTreeNode appIdNode = null;
        DefaultMutableTreeNode pubToRemNode = null;
        
        // hostId
        String nodeName = String.valueOf(ip2string(appInfo.getHostId()));
        int index = ManageTree.getNodeIndexByName(this.tree,this.root,nodeName);
        if(index < 0) {
            System.out.println(":: cannot find Pub node to remove! [bad  hostId]");
            return;
        }
        hostIdNode = (DefaultMutableTreeNode) this.treeModel.getChild(this.root,index);   
        // appId
        nodeName = appId4NodeName(appInfo.getAppId());
        index = ManageTree.getNodeIndexByName(this.tree,hostIdNode,nodeName);
        if(index < 0) {
            System.out.println(":: cannot find Pub node to remove! [bad  appId]");
            return;
        }
        appIdNode = (DefaultMutableTreeNode) this.treeModel.getChild(hostIdNode,index);
        // publication
        nodeName = pub4NodeName(pubInfo);
        index = ManageTree.getNodeIndexByName(this.tree,appIdNode,nodeName);
        if(index < 0) {
            System.out.println(":: cannot find Pub node to remove! [bad  pub]");
            return;
        }
        pubToRemNode = (DefaultMutableTreeNode) this.treeModel.getChild(appIdNode,index);
        this.treeModel = (DefaultTreeModel) tree.getModel();
        this.treeModel.removeNodeFromParent(pubToRemNode);
        this.treeModel.nodeStructureChanged(hostIdNode);
        this.treeModel.reload();
        
        
    }
    
    public void removeSubFromTree(AppInfo appInfo, SubInfo subInfo) {
        
        DefaultMutableTreeNode hostIdNode = null;
        DefaultMutableTreeNode appIdNode = null;
        DefaultMutableTreeNode subToRemNode = null;
        
        // hostId
        String nodeName = String.valueOf(ip2string(appInfo.getHostId()));
        int index = ManageTree.getNodeIndexByName(this.tree,this.root,nodeName);
        if(index < 0) {
            System.out.println(":: cannot find Sub node to remove! [bad  hostId]");
            return;
        }
        hostIdNode = (DefaultMutableTreeNode) this.treeModel.getChild(this.root,index);   
        // appId
        nodeName = appId4NodeName(appInfo.getAppId());
        index = ManageTree.getNodeIndexByName(this.tree,hostIdNode,nodeName);
        if(index < 0) {
            System.out.println(":: cannot find Sub node to remove! [bad  appId]");
            return;
        }
        appIdNode = (DefaultMutableTreeNode) this.treeModel.getChild(hostIdNode,index);
        // publication
        nodeName = sub4NodeName(subInfo);
        index = ManageTree.getNodeIndexByName(this.tree,appIdNode,nodeName);
        if(index < 0) {
            System.out.println(":: cannot find Sub node to remove! [bad  pub]");
            return;
        }
        subToRemNode = (DefaultMutableTreeNode) this.treeModel.getChild(appIdNode,index);
        this.treeModel = (DefaultTreeModel) tree.getModel();
        this.treeModel.removeNodeFromParent(subToRemNode);
        this.treeModel.nodeStructureChanged(hostIdNode);
        this.treeModel.reload();                
    }    
    
    
    
    /** 
     * Converts IP address from number notation into its dot notation.
     * @param ip IP address in number notation     
     * @return String with IP address in its dot notation
     */                 
    public static String ip2string(long ip) {
        long a = (ip & 0xFF000000) >> 3*8;
        long b = (ip & 0xFF0000) >> 2*8;
        long c = (ip & 0xFF00) >> 1*8;
        long d = (ip & 0xFF);
                                                 
        return(String.valueOf(a) + "." + String.valueOf(b) + "." + 
               String.valueOf(c) + "." + String.valueOf(d));
    }

    /** 
     * Creates string reprezentation of AppId. Useful for tree node name.
     * @param appId AppId in number representation
     * @return AppId in string
     */                 
     public static String appId4NodeName(long appId) {
        
        String nodeName;
        // appId (appId[4] = instanceId[3] + appKind )
        long instanceId = (appId >> 1*8);
        long appKind =    (appId & 0xFF);
        //
        if (appKind == 0x01) nodeName = ("app: " + String.valueOf(instanceId));
         else if (appKind == 0x02) nodeName = ("mgr: " + String.valueOf(instanceId));
         else nodeName = ("bad appKind");
        
        return nodeName;     
    }

     
    public String pub4NodeName(PubInfo pubInfo) {
        return("Pub: " + pubInfo.getTypeName());
    }

    
    public String sub4NodeName(SubInfo subInfo) {
        return("Sub: " + subInfo.getTypeName());
    }
   
    public void checkInfoNodeMatrix() {
        for(int i = 0; i < this.nodeInfoMatrix.length; i++) {
            for(int j = 0; j < this.nodeInfoMatrix[0].length; j++) {
                System.out.print(nodeInfoMatrix[i][j] + " ");
            }
            System.out.println("");
        }
    } 
}
