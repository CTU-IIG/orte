/*
 *  $Id: orte_api.h.h,v 0.0.0.1         2003/08/21
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

#ifndef _ORTE_API_H
#define _ORTE_API_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
  #include "orte_config.h"
#elif defined ORTE_OMK_UNIX
  #include "orte_config_omk_unix.h"
#elif defined ORTE_OMK_RTL
  #include "orte_config_omk_rtl.h"
#endif


#ifdef HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#ifdef HAVE_LINUX_TYPES_H
  #include <linux/types.h>
#endif
#if defined _WIN32 || defined CONFIG_ORTE_WIN
  #include <ew_types.h>
#endif
#include <typedefs_defines_rtps.h>
#include <defines_api.h>
#include <typedefs_api.h>
#include <protos_api.h>

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif /* _ORTE_API_H */
