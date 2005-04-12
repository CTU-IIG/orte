/** ORTEConstant.java */

/**
  * Class ORTEConstant contains the definitions of basic ORTE constants.
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


public class ORTEConstant {
 /* ************************************************* *
  * constants java.lang.Object shell size in bytes:   *
  * ************************************************* */

  public static final int BYTE_FIELD_SIZE     = 1;
  public static final int BOOLEAN_FIELD_SIZE  = 1;
  public static final int SHORT_FIELD_SIZE    = 2;
  public static final int CHAR_FIELD_SIZE     = 2;
  public static final int OBJREF_SIZE         = 4;
  public static final int INT_FIELD_SIZE      = 4;
  public static final int FLOAT_FIELD_SIZE    = 4;
  public static final int OBJECT_SHELL_SIZE   = 8;
  public static final int LONG_FIELD_SIZE     = 8;
  public static final int DOUBLE_FIELD_SIZE   = 8;
    
			
 /* ************************************************* *
  * constants from the file 'typedefs_defines_rtps.h' *
  * ************************************************* */

  public static final int RTPS_HEADER_LENGTH                =  16;

  public static final int MAX_PATHNAME                      =  128;
  public static final int MAX_TYPENAME                      =  128;

  public static final int ORTE_TRUE                         =  1;
  public static final int ORTE_FALSE                        =  0;


  /* hosID*/
  public static final int HID_UNKNOWN                       =  0x00;

  /* appID */
  public static final int AID_UNKNOWN                       =  0x00;
  public static final int MANAGEDAPPLICATION                =  0x01;
  public static final int MANAGER                           =  0x02;

  /* objID */
  public static final int OID_UNKNOWN                       =  0x00000000;
  public static final int OID_APP                           =  0x000001C1;
  public static final int OID_WRITE_APPSELF                 =  0x000008C2;
  public static final int OID_READ_APPSELF                  =  0x000008C7;
  public static final int OID_WRITE_APP                     =  0x000001C2;
  public static final int OID_READ_APP                      =  0x000001C7;
  public static final int OID_WRITE_MGR                     =  0x000007C2;
  public static final int OID_READ_MGR                      =  0x000007C7;
  public static final int OID_WRITE_PUBL                    =  0x000003C2;
  public static final int OID_READ_PUBL                     =  0x000003C7;
  public static final int OID_WRITE_SUBS                    =  0x000004C2;
  public static final int OID_READ_SUBS                     =  0x000004C7;
   /* Kind */
  public static final int OID_APPLICATION                   =  0x01;
  public static final int OID_CSTWRITER                     =  0x02;
  public static final int OID_PUBLICATION                   =  0x03;
  public static final int OID_SUBSCRIPTION                  =  0x04;
  public static final int OID_CSTREADER                     =  0x07;
  //
  public static final int OID_USEROBJ                       =  0x00;
  public static final int OID_RESUSEROBJ                    =  0x40;
  public static final int OID_METAOBJ                       =  0x80;
  public static final int OID_RESMETAOBJ                    =  0xC0;


  /* IP */
  public static final int IPADDRESS_INVALID                 =  0;


  /* port */
  public static final int PORT_INVALID                      =  0;


  /* PID  */
  public static final int PID_PAD                           =  0x00;
  public static final int PID_SENTINEL                      =  0x01;
  public static final int PID_EXPIRATION_TIME               =  0x02;
  public static final int PID_PERSISTENCE                   =  0x03;
  public static final int PID_MINIMUM_SEPARATION            =  0x04;
  public static final int PID_TOPIC                         =  0x05;
  public static final int PID_STRENGTH                      =  0x06;
  public static final int PID_TYPE_NAME                     =  0x07;
  public static final int PID_TYPE_CHECKSUM                 =  0x08;
  public static final int RTPS_PID_TYPE2_NAME               =  0x09;
  public static final int RTPS_PID_TYPE2_CHECKSUM           =  0x0a;
  public static final int PID_MATATRAFFIC_MULTICAST_IPADDRESS =0x0b;
  public static final int PID_APP_IPADDRESS                 =  0x0c;
  public static final int PID_METATRAFFIC_UNICAST_PORT      =  0x0d;
  public static final int PID_USERDATA_UNICAST_PORT         =  0x0e;
  public static final int PID_IS_RELIABLE                   =  0x0f;
  public static final int PID_EXPECTS_ACK                   =  0x10;
  public static final int PID_USERDATA_MULTICAST_IPADDRESS  =  0x11;
  public static final int PID_MANAGER_KEY                   =  0x12;
  public static final int PID_SEND_QUEUE_SIZE               =  0x13;
  public static final int PID_RELIABILITY_ENABLED           =  0x14;
  public static final int PID_PROTOCOL_VERSION              =  0x15;
  public static final int PID_VENDOR_ID                     =  0x16;
  public static final int PID_VARGAPPS_SEQUENCE_NUMBER_LAST =  0x17;
  public static final int PID_RECV_QUEUE_SIZE               =  0x18;
  public static final int PID_RELIABILITY_OFFERED           =  0x19;
  public static final int PID_RELIABILITY_REQUESTED         =  0x1a;
  /* possible values for PID_RELIABILITY_REQUEST */
  public static final int PID_VALUE_RELIABILITY_BEST_EFFORTS = 0x01;
  public static final int PID_VALUE_RELIABILITY_STRICT      =  0x02;



 /* *************************************** * 
  * constants from the file 'defines_api.h' *
  * *************************************** */

  public static final int MAX_INTERFACES                     =  5;
  public static final int ORTE_DEFAULT_DOMAIN                =  0;
  public static final int MAX_STRING_IPADDRESS_LENGTH        =  4*3+3+1;
  public static final int MAX_STRING_NTPTIME_LENGTH          =  30; //need fix

  /* Return values */
  public static final int ORTE_OK                            =  0;
  public static final int ORTE_BAD_HANDLE                    = -1;
  public static final int ORTE_TIMEOUT                       = -2;
  public static final int ORTE_QUEUE_FULL                    = -3;


}

