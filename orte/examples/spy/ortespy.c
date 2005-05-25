/*
 *  $Id: ortespy.c,v 0.0.0.1            2003/10/07 
 *
 *  DEBUG:  section                     ortespy
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
#include <stdlib.h>
#ifndef _WIN32
  #include <signal.h>
#endif
#include "orte.h"
#ifdef HAVE_CONFIG_H
  #ifdef HAVE_GETOPT_H
    #include <getopt.h>
  #endif
  #ifdef HAVE_UNISTD_H
    #include <unistd.h> //getopt.h for DarWin, Solaris, ...
  #endif
#else
  #include <getopt.h>
#endif
#include "string.h"

ORTEDomain              *d;
NtpTime                 deadline,minimumSeparation;
int32_t                 instanceRecv;

void
recvCallBack(const ORTERecvInfo *info,void *vinstance, void *recvCallBackParam) {
  char lbuff[MAX_STRING_NTPTIME_LENGTH];
  char rbuff[MAX_STRING_NTPTIME_LENGTH];
  switch (info->status) {
    case NEW_DATA:
      printf("| %-10s | %-9s | %-9s | %-18s | %-18s |\n",
             "NEW_DATA",
             info->topic,
             info->type,
             NtpTimeToStringUs(info->localTimeReceived, lbuff),
             NtpTimeToStringUs(info->remoteTimePublished, rbuff));
      break;
    case DEADLINE:
//      printf("deadline occurred\n");
      break;
  }
}

ORTESubscription*
subscriptionCallBack(const char *topic, const char *type, void *param) {
  ORTETypeRegisterAdd(d,type,NULL,NULL,NULL,0);   
  if (strcmp(topic,"Red")==0) return NULL;
  return ORTESubscriptionCreate(
        d,
        IMMEDIATE,
        BEST_EFFORTS,
        topic,
        type,
        &instanceRecv,
        &deadline,
        &minimumSeparation,
        recvCallBack,
        NULL,
        IPADDRESS_INVALID);
}

static void usage(void) {
  printf("usage: ortespy <parameters> \n");
  printf("  -d, --domain <domain>         working manager domain\n");
  printf("  -v, --verbosity <level>       set verbosity level SECTION, up to LEVEL:...\n");
  printf("      examples: ORTEManager -v 51.7:32.5 sections 51 and 32\n");
  printf("                ORTEManager -v ALL.7     all sections up to level 7\n");
  printf("  -R, --refresh <s>             refresh period in second(s)\n");
  printf("  -P, --purge <s>               purge time in second(s)\n");
  printf("  -e, --expiration <s>          expiration time of manager in second(s)\n");
  printf("  -I, --metaMulticast <IPAdd>   use multicast IPAddr for metatraffic comm.\n");
  printf("  -l, --logfile <filename>      set log file name\n");
  printf("  -V, --version                 show version\n");
  printf("  -h, --help                    this usage screen\n");
}

int main(int argc,char *argv[]) {
#if defined HAVE_GETOPT_LONG || defined HAVE_GETOPT_LONG_ORTE
  static struct option long_opts[] = {
    { "domain",1,0, 'd' },
    { "verbosity",1,0, 'v' },
    { "refresh",1,0, 'R' },
    { "purge",1,0, 'P' },
    { "expiration",1,0, 'E' },
    { "metaMulticast",1,0, 'I' },
    { "logfile",1,0, 'l' },
    { "version",0,0, 'V' },
    { "help",  0, 0, 'h' },
    { 0, 0, 0, 0}
  };
#endif
  ORTEDomainProp          dp; 
  int                     opt,domain=ORTE_DEFAULT_DOMAIN;
  
  ORTEInit();
  ORTEDomainPropDefaultGet(&dp);
  NTPTIME_BUILD(deadline,3); 
  NTPTIME_BUILD(minimumSeparation,0); 

#if defined HAVE_GETOPT_LONG || defined HAVE_GETOPT_LONG_ORTE
  while ((opt = getopt_long(argc, argv, "d:v:R:E:I:P:l:Vh",&long_opts[0], NULL)) != EOF) {
#else
  while ((opt = getopt(argc, argv, "d:v:R:E:I:P:l:Vh")) != EOF) {
#endif
    switch (opt) {
      case 'd':
        domain=strtol(optarg,NULL,0);
        break;
      case 'v':
        ORTEVerbositySetOptions(optarg);
        break;
      case 'R':
        NtpTimeAssembFromMs(dp.baseProp.refreshPeriod,strtol(optarg,NULL,0),0);
        break;
      case 'P':
        NtpTimeAssembFromMs(dp.baseProp.purgeTime,strtol(optarg,NULL,0),0);
        break;
      case 'E':
        NtpTimeAssembFromMs(dp.baseProp.expirationTime,strtol(optarg,NULL,0),0);
        break;
      case 'I':
        dp.multicast.enabled=ORTE_TRUE;
        dp.multicast.ipAddress=StringToIPAddress(optarg);
        break;	
      case 'l':
        ORTEVerbositySetLogFile(optarg);
      case 'V':
        printf("Ocera Real-Time Ethernet (%s).\n",dp.version);
        exit(0);
        break;
      case 'h':
      default:
        usage();
        exit(opt == 'h' ? 0 : 1);
    }
  }
  //Create application     
  printf("|------------------------------------------------------------------------------|\n");
  printf("| %-10s | %-9s | %-9s | %-18s | %-18s |\n", 
         "status", "type","topic","time received", "time sent");
  printf("|------------------------------------------------------------------------------|\n");
  d=ORTEDomainAppCreate(domain,&dp,NULL,ORTE_TRUE);
  ORTEDomainAppSubscriptionPatternAdd(d,"*","*",subscriptionCallBack,NULL);
  ORTEDomainStart(d,ORTE_TRUE,ORTE_FALSE,ORTE_TRUE,ORTE_FALSE,ORTE_TRUE);
  while (1) {
    ORTESleepMs(1000);
  }
  exit(0);
}

