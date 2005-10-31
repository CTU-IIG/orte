#include <stdio.h>
#include <orte.h>
#include <ddso.h>

void
ofb_cbs_octet(const ORTESendInfo *info,void *vinstance, void *sendCallBackParam) {
  ofb_octet *instance=(ofb_octet*)vinstance;

  switch (info->status) {
    case NEED_DATA:
        instance->value++;
      break;
    case CQL:  //criticalQueueLevel
      break;
  }
} 

void
ofb_cbr_octet(const ORTERecvInfo *info,void *vinstance, void *recvCallBackParam) {
  ofb_octet *instance=(ofb_octet*)vinstance;

  switch (info->status) {
    case NEW_DATA:
        printf("GUID:0x%x, value:%d\n",instance->guid,instance->value);
      break;
    case CQL:  //criticalQueueLevel
      break;
  }
} 

void
ofb_cbs_short(const ORTESendInfo *info,void *vinstance, void *sendCallBackParam) {
  ofb_short *instance=(ofb_short*)vinstance;

  switch (info->status) {
    case NEED_DATA:
        instance->value++;
      break;
    case CQL:  //criticalQueueLevel
      break;
  }
} 

void
ofb_cbr_short(const ORTERecvInfo *info,void *vinstance, void *recvCallBackParam) {
  ofb_short *instance=(ofb_short*)vinstance;

  switch (info->status) {
    case NEW_DATA:
        printf("GUID:0x%x, value:%d\n",instance->guid,instance->value);
      break;
    case CQL:  //criticalQueueLevel
      break;
  }
} 

void
ofb_cbs_long(const ORTESendInfo *info,void *vinstance, void *sendCallBackParam) {
  ofb_long *instance=(ofb_long*)vinstance;

  switch (info->status) {
    case NEED_DATA:
        instance->value++;
      break;
    case CQL:  //criticalQueueLevel
      break;
  }
} 

void
ofb_cbr_long(const ORTERecvInfo *info,void *vinstance, void *recvCallBackParam) {
  ofb_long *instance=(ofb_long*)vinstance;

  switch (info->status) {
    case NEW_DATA:
        printf("GUID:0x%x, value:%d\n",instance->guid,instance->value);
      break;
    case CQL:  //criticalQueueLevel
      break;
  }
} 
