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

#define printf rtl_printf
#define gettimeofday(tv, tz) do_gettimeofday(tv)

#define setsockopt(a, b, c, d, e) (-1)
#define getsockopt(a, b, c, d, e) (-1)
#define ioctl(a, b, c) (-1)


static inline int atoi(const char* nptr)
{
    return simple_strtol(nptr, (char **)NULL, 10);
}

#endif /* _COMPAT_H */
