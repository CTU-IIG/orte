/*
 *  $Id: HelloWorldPublisher.c,v 0.0.0.1 2003/12/26 
 *
 *  DEBUG:  section                     HelloWorldPublisher
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
  #define printf rtl_printf
#endif
#include "orte_api.h"

ORTEDomain        *d;
char              instance2Send[64];
char              terminate=0;

void * 
publisher(void *arg) {
  ORTEPublication     *p;
  ORTEDomainProp	    dp; 
  NtpTime             persistence;
  int                 i=1;            

  ORTEInit();
  ORTEDomainPropDefaultGet(&dp);
  #ifdef __RTL__
  dp.appLocalManager=StringToIPAddress("192.168.0.4");
  #endif
  d=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,&dp,NULL);
  ORTETypeRegisterAdd(d,"HelloMsg",NULL,NULL,64);
  NTPTIME_BUILD(persistence,3); 
  p=ORTEPublicationCreate(
       d,
      "Example HelloMsg",
      "HelloMsg",
      &instance2Send,
      &persistence,
      1,
      NULL,
      NULL,
      NULL);
  while (!terminate) {
    ORTESleepMs(1000);
    printf("Sampling publication, count %d\n", i);
    sprintf(instance2Send,"Hello Universe! (%d)",i++);
    ORTEPublicationSend(p);
  }
  #ifdef __RTL__
    pthread_exit(arg);
  #endif
  return 0;
}

#ifndef __RTL__
int 
main(int argc, char *args[]) {
  publisher(NULL);
  return 0;
}
#else
pthread_t t;
int 
init_module(void) {
  pthread_create (&t, NULL, publisher, 0);
  return 0;
}
void 
cleanup_module(void) {
  int result;
  terminate=1;
  pthread_join(t,(void **) &result);
  ORTEDomainAppDestroy(d);
}
#endif
