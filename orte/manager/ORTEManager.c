/*
 *  $Id: ORTEManager.c,v 0.0.0.1        2003/10/07
 *
 *  DEBUG:  section                     Manager
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
#ifdef HAVE_CONFIG_H
  #include <orte_config.h>
#endif

#ifdef CONFIG_ORTE_RTAI
  #include <linux/module.h>
  #include <rtai.h>
  #include <rtai_sched.h>
  #include <rtai_sem.h>
#else
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #ifdef _WIN32
    #include <getopt.h>
  #elif defined __RTL__
  #else
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <pthread.h>
    #include <getopt.h>
    #include <signal.h>
    #define _UNIX
  #endif
#endif

#include "orte_api.h"

#ifndef CONFIG_ORTE_RT
Boolean
onMgrAppRemoteNew(const struct ORTEAppInfo *appInfo, void *param) {
  printf("%s 0x%x-0x%x was accepted\n",
         (appInfo->appId & 0x3)==MANAGER ? "manager" : "application",
         appInfo->hostId,appInfo->appId);
  return ORTE_TRUE;
}

void
onMgrAppDelete(const struct ORTEAppInfo *appInfo, void *param) {
  printf("%s 0x%x-0x%x was deleted\n",
         (appInfo->appId & 0x3)==MANAGER ? "manager" : "application",
         appInfo->hostId,appInfo->appId);
}

#ifdef _UNIX
pthread_mutex_t     mutex;
void sig_usr(int signo) {
  if ((signo==SIGTERM) || (signo==SIGINT)) {
    pthread_mutex_unlock(&mutex);
  }
}
void waitForEndingCommand(void) {
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_lock(&mutex);
  signal(SIGTERM,sig_usr);
  signal(SIGINT,sig_usr);
  pthread_mutex_lock(&mutex);
}
static int daemon_init(void) {
  pid_t pid;

  if ((pid = fork()) < 0) {
    return -1;
  } else
    if (pid != 0) {
      exit(0);	/* parent vanishes */
    }
  /* child process */
  setsid();
  umask(0);
  close(0);
  close(1);
  close(2);
  return 0;
}
#endif

static void usage(void) {
  printf("usage: ORTEManager <parameters> \n");
  printf("  -p, --peer <IPAdd:IPAdd:...>  possible locations of fellow managers\n");
  printf("  -k, --key  <IPAdd>            manager's additional key\n");
  printf("  -d, --domain <domain>         working manager domain\n");
  printf("  -v, --verbosity <level>       set verbosity level SECTION, up to LEVEL:...\n");
  printf("      examples: ORTEManager -v 51,7:32,5 sections 51 and 32\n");
  printf("                ORTEManager -v ALL,7     all sections up to level 7\n");
  printf("  -R, --refresh <s>             refresh period in second(s)\n");
  printf("  -P, --purge <s>               purge time in second(s)\n");
#ifdef _UNIX
  printf("  -D, --daemon                  start program like daemon\n");
#endif
  printf("  -E, --expiration <s>          expiration time of manager in second(s)\n");
  printf("  -e, --events                  register event system\n");
  printf("  -l, --logfile <filename>      set log file name\n");
  printf("  -V, --version                 show version\n");
  printf("  -h, --help                    this usage screen\n");
}

int main(int argc,char *argv[]) {
  static struct option long_opts[] = {
    { "peer",1,0, 'p' },
    { "key",1,0, 'k' },
    { "domain",1,0, 'd' },
    { "verbosity",1,0, 'v' },
    { "refresh",1,0, 'R' },
    { "purge",1,0, 'P' },
#ifdef _UNIX
    { "daemon",1,0, 'D' },
#endif
    { "expiration",1,0, 'E' },
    { "events",0,0, 'e' },
    { "logfile",1,0, 'l' },
    { "version",0,0, 'V' },
    { "help",  0, 0, 'h' },
    { 0, 0, 0, 0}
  };
  ORTEDomain		      *d;
  ORTEDomainProp	    dp;
  int32_t             opt,domain=ORTE_DEFAULT_DOMAIN;
  Boolean		          daemon=ORTE_FALSE,start_sending_thread=ORTE_FALSE;
  ORTEDomainAppEvents *events=NULL;

  #ifdef _UNIX
  start_sending_thread=ORTE_TRUE;
  #endif

  ORTEInit();
  ORTEDomainPropDefaultGet(&dp);

  while ((opt = getopt_long(argc, argv, "k:p:d:v:R:E:P:l:VhDe",&long_opts[0], NULL)) != EOF) {
    switch (opt) {
      case 'p':
        dp.mgrs=strdup(optarg);
        break;
      case 'k':
        dp.mgrAddKey=StringToIPAddress(strdup(optarg));
        break;
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
      case 'e':
        events=(ORTEDomainAppEvents*)malloc(sizeof(ORTEDomainAppEvents));
        ORTEDomainInitEvents(events);
        events->onMgrNew=onMgrAppRemoteNew;
        events->onAppRemoteNew=onMgrAppRemoteNew;
        events->onMgrDelete=onMgrAppDelete;
        events->onAppDelete=onMgrAppDelete;
        break;
      case 'l':
        ORTEVerbositySetLogFile(optarg);
        break;
      case 'V':
        printf("Ocera Real-Time Ethernet (%s).\n",dp.version);
        exit(0);
        break;
      case 'D':
        daemon=ORTE_TRUE;
        break;
      case 'h':
      default:
        usage();
        exit(opt == 'h' ? 0 : 1);
    }
  }

  d=ORTEDomainMgrCreate(domain,&dp,events,ORTE_TRUE);
  if (!d)
    exit(1);

  #ifdef _UNIX
  if (daemon)
    daemon_init();
  #endif

  ORTEDomainStart(d,ORTE_TRUE,ORTE_FALSE,ORTE_FALSE);
  if (!start_sending_thread) {
    ORTEAppSendThread(d);
  }
  ORTEDomainStart(d,ORTE_FALSE,ORTE_FALSE,ORTE_TRUE);

  #ifdef _UNIX
  waitForEndingCommand();
  ORTEDomainMgrDestroy(d);
  if (events)
    free(events);
  #endif

  exit(0);
}
#else
MODULE_LICENSE("GPL");
ORTEDomain          *d;

void
createManager(void) {
  ORTEInit();
  ORTEVerbositySetOptions("ALL,2");
  d=ORTEDomainMgrCreate(ORTE_DEFAULT_DOMAIN,NULL,NULL,ORTE_FALSE);
}

int
init_module(void) {
  createManager();
  return 0;
}
void
cleanup_module(void) {
  if (d)
    ORTEDomainMgrDestroy(d);
}
#endif
