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
 
#include "orte.h"

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

#ifdef CONFIG_ORTE_UNIX
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
  printf("  -k, --key  <IPAdd:IPAdd:...>  manualy assigned manager's keys\n");
  printf("  -d, --domain <domain>         working manager domain\n");
  printf("  -v, --verbosity <level>       set verbosity level SECTION, up to LEVEL:...\n");
  printf("      examples: ORTEManager -v 51,7:32,5 sections 51 and 32\n");
  printf("                ORTEManager -v ALL,7     all sections up to level 7\n");
  printf("  -R, --refresh <s>             refresh period in second(s)\n");
  printf("  -P, --purge <s>               purge time in second(s)\n");
#ifdef CONFIG_ORTE_UNIX
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
#ifdef CONFIG_ORTE_UNIX
    { "daemon",1,0, 'D' },
#endif
    { "expiration",1,0, 'E' },
    { "events",0,0, 'e' },
    { "logfile",1,0, 'l' },
    { "version",0,0, 'V' },
    { "help",  0, 0, 'h' },
    { 0, 0, 0, 0}
  };
  ORTEDomain	      *d;
  ORTEDomainProp      dp;
  int32_t             opt,domain=ORTE_DEFAULT_DOMAIN;
  Boolean	      daemon=ORTE_FALSE;
  ORTEDomainAppEvents *events=NULL;

  ORTEInit();
  ORTEDomainPropDefaultGet(&dp);

  while ((opt = getopt_long(argc, argv, "k:p:d:v:R:E:P:l:VhDe",&long_opts[0], NULL)) != EOF) {
    switch (opt) {
      case 'p':
        dp.mgrs=strdup(optarg);
        break;
      case 'k':
        dp.keys=strdup(optarg);
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

  #ifdef CONFIG_ORTE_UNIX
  if (daemon)
    daemon_init();
  #endif

  ORTEDomainStart(d,ORTE_TRUE,ORTE_FALSE,ORTE_FALSE);
  #ifndef CONFIG_ORTE_UNIX
    d->taskSend.terminate=ORTE_FALSE;
    ORTEAppSendThread(d);
  #endif
  ORTEDomainStart(d,ORTE_FALSE,ORTE_FALSE,ORTE_TRUE);

  #ifdef CONFIG_ORTE_UNIX
  waitForEndingCommand();
  ORTEDomainMgrDestroy(d);
  if (dp.mgrs) 
    free(dp.mgrs);
  if (dp.keys) 
    free(dp.keys);
  if (events) 
    free(events);
  #endif

  exit(0);
}
#else
char *verbosity="";
MODULE_PARM(verbosity,"1s");
MODULE_PARM_DESC(verbosity,"set verbosity level SECTION, up to LEVEL:...");
char *peer="";
MODULE_PARM(peer,"1s");
MODULE_PARM_DESC(peer,"possible locations of fellow managers");
MODULE_LICENSE("GPL");
ORTEDomain *d=NULL;
pthread_t  thread;

void *
domainDestroy(void *arg) {
  if (!d) return NULL;
  ORTEDomainMgrDestroy(d);
  return arg;
}


int
init_module(void) {
  ORTEDomainProp      dp;

  ORTEInit();
  ORTEDomainPropDefaultGet(&dp);
  ORTEVerbositySetOptions(verbosity);
  dp.mgrs=peer;
  d=ORTEDomainMgrCreate(ORTE_DEFAULT_DOMAIN,&dp,NULL,ORTE_FALSE);
  return 0;
}
void
cleanup_module(void) {
  if (!d) return;
  pthread_create(&thread,NULL,&domainDestroy,NULL);
  pthread_join(thread,NULL);
}
#endif
