/*
 *  $Id: compat.h,v 0.0.0.1             2003/12/19 
 *
 *  AUTHOR: Michal Sojka                sojka1@fel.cvut.cz
 *
 *  ORTE - OCERA Real-Time Ethernet     http://www.ocera.org/
 *  --------------------------------------------------------------------
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 */

#ifndef _COMPAT_H
#define _COMPAT_H

#include <linux/if.h>
#include <linux/in.h>
#include <linux/inet.h>

#define printf rtl_printf
#define gettimeofday(tv, tz) do_gettimeofday(tv)

#define socket                      ocn_socket
#define setsockopt                  ocn_setsockopt
#define getsockopt                  ocn_getsockopt
#define ioctl                       ocn_ioctl
#define bind                        ocn_bind
#define getsockname                 ocn_getsockname
#define recvfrom                    ocn_recvfrom
#define sendto                      ocn_sendto
#define close                       ocn_close

static inline int atoi(const char* nptr)
{
    return simple_strtol(nptr, (char **)NULL, 10);
}

static inline __u32 inet_addr(const char *cp)
{
		    return in_aton(cp);
}

static char inet_ntoa_buf[16];
static inline char *inet_ntoa(struct in_addr in) {
  unsigned char *octet;

  octet = (unsigned char *)&(in.s_addr);
  sprintf(inet_ntoa_buf, "%u.%u.%u.%u", octet[0], octet[1], octet[2], octet[3]);

  return inet_ntoa_buf;
}


#endif /* _COMPAT_H */
