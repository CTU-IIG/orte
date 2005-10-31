#include <stdio.h>
#include <orte.h>
#include <ddso.h>
#include "ddso_cb.h"

ORTEDomain        *d=NULL; 
NtpTime           persistence, delay;
ORTEPublication   *p_octet;
ORTEPublication   *p_short;
ORTEPublication   *p_long;

ofb_octet         i2s_octet;
ofb_short         i2s_short;
ofb_long          i2s_long;

int main(int argc,char *argv[]) {
  //init
  ORTEInit();
  d=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,NULL,NULL,ORTE_FALSE);
  if (!d) return -1;
  NTPTIME_BUILD(persistence,3);  //3s
  NTPTIME_BUILD(delay,1);        //1s
  i2s_octet.guid=0x123456;
  i2s_octet.value=0;
  i2s_short.guid=0x654321;
  i2s_short.value=0;
  i2s_long.guid=0x123654;
  i2s_long.value=0;

  //register serialization/deserealization functions for a given types
  ofb_octet_type_register(d);
  ofb_short_type_register(d);
  ofb_long_type_register(d);
  ofb_float_type_register(d);
  ofb_double_type_register(d);

  //create of services
  p_octet=ORTEPublicationCreate(
       d,
      "GUID_octet",
      "ofb_octet",
      &i2s_octet,
      &persistence,
      1,
      &ofb_cbs_octet,
      NULL,
      &delay);
  p_short=ORTEPublicationCreate(
       d,
      "GUID_short",
      "ofb_short",
      &i2s_short,
      &persistence,
      1,
      &ofb_cbs_short,
      NULL,
      &delay);
  p_long=ORTEPublicationCreate(
       d,
      "GUID_long",
      "ofb_long",
      &i2s_long,
      &persistence,
      1,
      &ofb_cbs_long,
      NULL,
      &delay);
  
  while(1) {
    ORTESleepMs(1);
  }
  return 0;
}
