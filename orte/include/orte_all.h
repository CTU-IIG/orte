/*
 *  $Id: orte_all.h,v 0.0.0.1           2003/08/21 
 *
 *  AUTHOR: Petr Smolik                 petr.smolik@wo.cz
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

#ifndef _ORTE_ALL_H
#define _ORTE_ALL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __RTL__
#ifdef HAVE_CONFIG_H
  #define _GNU_SOURCE
  #define SOCK_BSD
  #include "config.h"
#endif
#ifdef HAVE_UNISTD_H
  #include <unistd.h>
#endif
#ifdef HAVE_STDIO_H
  #include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
  #include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
  #include <string.h>
#endif
#ifdef HAVE_SOCKET
  #include <sys/socket.h>
#endif
#ifdef HAVE_NETDB_H
  #include <netdb.h>
#endif
#ifdef HAVE_NET_IF_H
  #include <net/if.h>
#endif
#ifdef HAVE_NETINET_IN_H
  #include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
  #include <arpa/inet.h>
#endif
#ifdef HAVE_FCNTL_H
  #include <fcntl.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
  #include <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_TIME_H
  #include <sys/time.h>
#endif
#ifdef HAVE_PTHREAD_H
  #include <pthread.h>
#endif
#ifdef HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#ifdef HAVE_BYTESWAP_H
  #include <byteswap.h>
#endif
#ifdef HAVE_STDARG_H
  #include <stdarg.h>
#endif
#ifdef HAVE_CTYPE_H
  #include <ctype.h>
#endif
#ifdef HAVE_WINSOCK2_H
  #include <winsock2.h>
#endif
#ifdef HAVE_WS2TCPIP_H
  #include <ws2tcpip.h>
#endif
#endif


#ifdef _WIN32
  #ifndef HAVE_CONFIG_H
    #include "config.h.undef"
    #if defined(_MSC_VER) || defined (_OMK_UNIX) 
      #ifndef inline
        #define inline _inline
      #endif
      #include <stdio.h>
      #include <stdlib.h>
      #include <stdarg.h>
      #include <string.h>
      #include <winsock2.h>
      #include <ws2tcpip.h>
    #endif
  #endif
  #include <pthread.h>
  #include <timeval.h>
  #include <ew_types.h>
  #define ioctl ioctlsocket
  #undef SOCK_BSD
  #define SOCK_WIN
#elif defined __RTL__
  #include "config.h.undef"
  #include <rtl.h>
  #include <posix/pthread.h>
  #include <posix/time.h>
  #include <rtl_malloc.h>
  #include <byteswap.h>
  #include <udp.h>
  #include <nic.h>
  #include <nictab.h>
  #include <ctype.h>
  #include <time.h>
  #include <rtl/compat.h>
  #include <rtl/rwlock.h>
  #define SOCK_RTL
#elif defined _OMK_UNIX
  #define _GNU_SOURCE
  #include "config.h.undef"
  #include <unistd.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/socket.h>
  #include <netdb.h>
  #include <net/if.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <fcntl.h>
  #include <sys/ioctl.h>
  #include <sys/time.h>
  #include <pthread.h>
  #include <sys/types.h>
  #include <byteswap.h>
  #include <stdarg.h>
  #include <ctype.h>
  #define SOCK_BSD
#endif

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif /* _ORTE_ALL_H */
