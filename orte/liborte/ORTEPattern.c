/*
 *  $Id: ORTEPattern.c,v 0.0.0.1        2003/07/12 
 *
 *  DEBUG:  section 28                  Pattern
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

/**********************************************************************************/
Boolean
ORTEPatternCheckDefault(const char *topic) {
  if (strchr(topic, '*')!=NULL) 
    return ORTE_TRUE;
  return ORTE_FALSE;
}

/**********************************************************************************/
Boolean
ORTEPatternMatchDefault(const char *topic,const char *pattern,void *param) {
  fnmatch(pattern,topic,0);
  return ORTE_FALSE;
}

/**********************************************************************************/
void
ORTEPatternRegister(ORTEDomain *d,ORTEPatternCheck check,
     ORTEPatternMatch match,void *param) {
  pthread_rwlock_wrlock(&d->patternEntry.lock);
  d->patternEntry.check=check;
  d->patternEntry.match=match;
  d->patternEntry.param=param;
  pthread_rwlock_unlock(&d->patternEntry.lock);
}
