/*
 *  $Id: ReliablePublisher.c,v 0.0.0.1  2003/12/27 
 *
 *  DEBUG:  section                     ReliablePublisher
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

#include <stdio.h>
#ifdef __RTL__
  #include <posix/pthread.h>
#endif
#include "orte_api.h"

ORTEDomain        *d;
char              instance2Send[64];

int 
main(int argc, char *args[]) {
  ORTEPublication *p;
  NtpTime         persistence;
  int             i=1;            

  ORTEInit();
  //ORTEVerbositySetOptions("ALL,10");
  d=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,NULL,NULL);
  ORTETypeRegisterAdd(d,"HelloMsg",NULL,NULL,64);
  NTPTIME_BUILD(persistence,3); 
  p=ORTEPublicationCreate(
       d,
      "Reliable HelloMsg",
      "HelloMsg",
      &instance2Send,
      &persistence,
      1,
      NULL,
      NULL,
      NULL);
  while (1) {
    ORTESleepMs(1000);
    printf("Sampling publication, count %d\n", i);
    sprintf(instance2Send,"Hello Universe! (%d)",i++);
    ORTEPublicationSend(p);
  }
  return 0;
}

#ifdef __RTL__
void 
hello_init(void) {
  main(0,NULL);
}
void 
hello_exit(void) {
  ORTEDomainAppDestroy(d);
}
MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
#endif

