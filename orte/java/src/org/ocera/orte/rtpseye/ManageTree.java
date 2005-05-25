/*
 * ManageTree.java
 *
 * Created on 29. duben 2005, 11:53
 */

package org.ocera.orte.rtpseye;

import java.awt.*;
import javax.swing.*;
import javax.swing.tree.*;

/**
 *
 * @author luky
 */
public class ManageTree {
    
    /** Creates a new instance of ManageTree */
    public ManageTree() {
    }

    
    /** Creates root node */
    public static DefaultMutableTreeNode createRootNodeByName(String rootName) {

        DefaultMutableTreeNode root = new DefaultMutableTreeNode(rootName);

// /////////////////////////////////// for TESTING     
/*
        DefaultMutableTreeNode child1 = new DefaultMutableTreeNode("192.168.1.1");
        DefaultMutableTreeNode child2 = new DefaultMutableTreeNode("147.32.87.224");
        DefaultMutableTreeNode child11 = new DefaultMutableTreeNode("App 0x112233");
        DefaultMutableTreeNode child12 = new DefaultMutableTreeNode("Mgr 0x445566");
        //        
        root.add(child1);
        child1.add(child11);
        child1.add(child12);
        root.add(child2);
*/
// ///////////////////////////////////        
        
        return root;
    }  

    /** Find node specified by name in it's tree. 
     * @param tree with the node
     * @param parent node
     * @param name of the node
     */
     public static void addNode(JTree tree, DefaultMutableTreeNode parent, 
             String nodeName) {

        // get actual tree model
        DefaultTreeModel treeModel = (DefaultTreeModel) tree.getModel();             
        // create new node
        DefaultMutableTreeNode newChild = new DefaultMutableTreeNode(nodeName);
        // insert node into a tree
        System.out.println("newChild: " + newChild);
        System.out.println("parent: " + parent);
        System.out.println("parent.count: " + parent.getChildCount());
        treeModel.insertNodeInto(newChild,parent,parent.getChildCount());        
        return;
     }
    
       
    /** Find node specified by name in it's tree. 
     * @param tree with the node
     * @param parent node
     * @param name of the node
     * @return index of node, -1 if node hasn't been found 
     */
    public static int getNodeIndexByName(JTree tree, Object parent, 
            String nodeName) {
        
        // takto to nefunguje
        //DefaultMutableTreeNode node = new DefaultMutableTreeNode(nodeName);
        //return(tree.getIndexOfChild(parent, node));

        // get actual tree model
        DefaultTreeModel treeModel = (DefaultTreeModel) tree.getModel();             
            
        int nodeIndex = -1;
        // find index of node
        /* TODO neni nejstastnejsi hleda uzel for cyklem - pro stromy s malym
         * poctem uzlu nevadi, ale pro rozsahle stromy ano - zkusit najit 
         * jine reseni
         */
        for(int i = 0; i < treeModel.getChildCount(parent); i++) {
            System.out.println("position: " + i);                                                         
            
            if(nodeName.equals(treeModel.getChild(parent,i).toString())) {
                System.out.println("find node on position: " + i);
                nodeIndex = i;
                break;
            }            
        }
        return nodeIndex;                
    }
    
        
    /** Removes specified node by name from it's tree. 
     * @param tree with the node
     * @param parent node
     * @param name of the node
     * @return true if node was found and remove, false otherwise
     */
    public static boolean removeNodeByName(JTree tree, Object parent,
            String nodeName) {

        // get actual tree model
        DefaultTreeModel treeModel = (DefaultTreeModel) tree.getModel();             
        Object getOutNode = new Object();
        int nodeIndex = getNodeIndexByName(tree,parent,nodeName);
        if (nodeIndex == -1) {
            System.out.println("node:'" + nodeName + 
                    " 'to remove hasn't been found");
            return false;
        }        
        getOutNode = treeModel.getChild(parent, nodeIndex);
        // remove node                       
        treeModel.removeNodeFromParent((MutableTreeNode) getOutNode);                        
        return true;        
    }
    
    
    
}
