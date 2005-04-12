/* MessageData.java */

/**
 * Class MessageData.
 *
 * @author Lukas Pokorny (lukas_pokorny@centrum.cz)
 * @author CTU FEE Prague - Department of Control Engineering (dce.felk.cvut.cz)
 * @author Project ORTE - OCERA Real Time Ethernet (www.ocera.org)
 * @author dedication to Kj
 * @version 0.1
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

package org.ocera.orte.types;
import java.nio.ByteBuffer;


public class MessageData
{

  private   String         topic;
  private   int            maxLength;
  protected  ByteBuffer    buffer; 
  
   /**
    * default constructor
    * */
   public MessageData() 
   {
     this.buffer = ByteBuffer.allocate(getMaxDataLength());
     //System.out.println(":j: instance MessageData created..");	
   }   	

  public void  read() {};
  public void write() {};

  /** 
   * Get the Publication's Topic.  
   * @ return Publication's topic  
   * */
  public String getTopic()
  {
    return topic;
  }
   
  /** 
   * Set the Publication's Topic.  
   * @ return void 
   * */
  public void setTopic(String newTopic)
  {
    topic = newTopic;
  	 return;
  }
       
  /** 
   * Get default Type of Publication. Default type name is abbreviate from its class name.  
   * @ return default type of Publication 
   * */
  public String getType()
  {
    char    extensionSeparator = '.';
    // get name of class
	String  className = getClass().getName();
    // find last separator
	int     dot = className.lastIndexOf(extensionSeparator);
	return  className.substring(dot + 1);
  }
  
  public int getMaxDataLength() { return 0; };
  
  public ByteBuffer getBuffer()
  {
  	return this.buffer;
  }
  
}



