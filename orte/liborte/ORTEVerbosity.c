/*
 *  $Id: ORTEVerbosity.c,v 0.0.0.1  2003/08/21 
 *
 *  DEBUG:  section 27                  Verbosity function                  
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

#include "orte_all.h"

/*****************************************************************************/
inline void 
ORTEVerbositySetOptions(const char *options) {
  debug_options(options);
}

/*****************************************************************************/
inline void 
ORTEVerbositySetLogFile(const char *logfile) {
  debug_open_log(logfile);
}





