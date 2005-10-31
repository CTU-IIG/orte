#include <stdio.h>
#include <orte.h>
#include <ddso.h>
#include "ddso_cb.h"

ORTEDomain        *d=NULL; 
NtpTime           deadline,minimumSeparation; 
ORTESubscription  *s_octet;
ORTESubscription  *s_short;
ORTESubscription  *s_long;

ofb_octet         i2r_octet;
ofb_short         i2r_short;
ofb_long          i2r_long;

int main(int argc,char *argv[]) {
  //init
  ORTEInit();
  d=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,NULL,NULL,ORTE_FALSE);
  if (!d) return -1;
  NTPTIME_BUILD(deadline,10);         //10s
  NTPTIME_BUILD(minimumSeparation,0); //0s

  //register serialization/deserealization functions for a given types
  ofb_octet_type_register(d);
  ofb_short_type_register(d);
  ofb_long_type_register(d);
  ofb_float_type_register(d);
  ofb_double_type_register(d);

  //create of services
  s_octet=ORTESubscriptionCreate(
       d,
       IMMEDIATE,
       BEST_EFFORTS,
       "GUID_octet",
       "ofb_octet",
       &i2r_octet,
       &deadline,
       &minimumSeparation,
       ofb_cbr_octet,
       NULL,
       IPADDRESS_INVALID); 
  s_short=ORTESubscriptionCreate(
       d,
       IMMEDIATE,
       BEST_EFFORTS,
       "GUID_short",
       "ofb_short",
       &i2r_short,
       &deadline,
       &minimumSeparation,
       ofb_cbr_short,
       NULL,
       IPADDRESS_INVALID); 
  s_long=ORTESubscriptionCreate(
       d,
       IMMEDIATE,
       BEST_EFFORTS,
       "GUID_long",
       "ofb_long",
       &i2r_long,
       &deadline,
       &minimumSeparation,
       ofb_cbr_long,
       NULL,
       IPADDRESS_INVALID); 
  
  while(1) {
    ORTESleepMs(1);
  }
  return 0;
}
