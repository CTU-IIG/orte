/* MessageInterpret.java */

/**
 * Class MessageInterpret substitutes the struct MessageInterpret
 * from C-source code defines in: 'typedefs_defines_rtps.h'
 *
 *  typedef struct {
 *     ProtocolVersion           sourceVersion;
 *     VendorId                  sourceVendorId;
 *     HostId                    sourceHostId;
 *     AppId                     sourceAppId;
 *     HostId                    destHostId;
 *     AppId                     destAppId;
 *     IPAddress                 unicastReplyIPAddress;
 *     Port                      unicastReplyPort;
 *     IPAddress                 multicastReplyIPAddress;
 *     Port                      multicastReplyPort;
 *     Boolean                   haveTimestamp;
 *     NtpTime                   timestamp;
 *   } MessageInterpret;
 *
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


public class MessageInterpret {

       public  ProtocolVersion      sourceVersion;
       public  VendorId             sourceVendorId;
       public  long                 sourceHostId;
       public  long                 sourceAppId;
       public  long                 destHostId;
       public  long                 destAppId;
       public  long                 unicastReplyIPAddress;
       public  long                 unicastReplyPort;
       public  long                 multicastReplyIPAddress;
       public  long                 multicastReplyPort;
       public  boolean              haveTimestamp;
       public  NtpTime              timestamp;


       /* constructor  */
       public MessageInterpret(ProtocolVersion   srcVersion,
                               VendorId          srcVendorId,
                               long              srcHostId,
                               long              srcAppId,
                               long              dstHostId,
                               long              dstAppId,
                               long              unicReplyIPAddr,
                               long              unicReplyPort,
                               long              multicReplyIPAddr,
                               long              multicReplyPort,
                               boolean           haveTimeStamp,
                               NtpTime           timeStamp)

       {
          sourceVersion            =  srcVersion;
          sourceVendorId           =  srcVendorId;
          sourceHostId             =  srcHostId;
          sourceAppId              =  srcAppId;
          destHostId               =  dstHostId;
          destAppId                =  dstAppId;
          unicastReplyIPAddress    =  unicReplyIPAddr;
          unicastReplyPort         =  unicReplyPort;
          multicastReplyIPAddress  =  multicReplyIPAddr;
          multicastReplyPort       =  multicReplyPort;
          haveTimestamp            =  haveTimeStamp;
          timestamp                =  timeStamp;
       }

}

