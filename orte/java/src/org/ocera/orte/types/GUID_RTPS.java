/* GUID_RTPS.java */

/**
 * Class GUID_RTPS substitutes the struct GUID_RTPS
 * from C-source code defines in: 'typedefs_defines_rtps.h'
 *
 *  typedef struct {
 *      HostId              hid;
 *      AppId               aid;
 *      ObjectId            oid;
 *   } GUID_RTPS;
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

/* TODO @hid,aid,oid: v C pretypovat z long na uint32_t */

public class GUID_RTPS {
  private long       hid;
  private long       aid;
  private long       oid;


  /* constructor */
  public GUID_RTPS(long hostId, long appId, long objectId) {
    hid = hostId;
    aid = appId;
    oid = objectId;

	//System.out.println(":j: instance of GUID_RTPS created..");
  }

  /* toString() */
  public String toString()
  {
    return("GUID: hid = " + hid + " aid = " + aid + " oid = " + oid);
  }


  /* get GUID_RTPS */
  public GUID_RTPS get()
  {
    return this;
  }

  /* set GUID_RTPS */
  public void set(long h, long a, long o)
  {
    this.hid = h;
    this.aid = a;
    this.oid = o;
	return;
  }
}





