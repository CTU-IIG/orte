/*
 *  $Id: parameter.c,v 0.0.0.1          2003/10/13
 *
 *  DEBUG:  section 11                  parameter
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

/*****************************************************************************/
int
parameterDelete(CSChange *csChange) {
  ParameterSequence *ps;

  ul_list_for_each(CSChangeAttributes,
                   csChange,
                   ps) {
    if ((ps->parameterLength) > MAX_PARAMETER_LOCAL_LENGTH)
      FREE(ps->parameter);
  }
  //ps is like one piece of memory
  if (CSChangeAttributes_first(csChange))
    FREE(CSChangeAttributes_first(csChange));
  CSChangeAttributes_init_head(csChange);  
  return ORTE_TRUE;
} 


/*****************************************************************************/
int
parameterCodeStreamFromCSChange(CSChange *csChange,
    u_int8_t *rtps_msg,u_int32_t max_msg_len) {
  ParameterSequence *ps;
  int               result=0;

  ul_list_for_each(CSChangeAttributes,
                   csChange,
                   ps) {
    if (max_msg_len<(u_int32_t)(4+ps->parameterLength)) return -1; //no memory for copy
    *((ParameterId*)rtps_msg)=ps->parameterID;
    *((ParameterLength*)(rtps_msg+2))=ps->parameterLength; 
    if (ps->parameterLength>MAX_PARAMETER_LOCAL_LENGTH) {
      memcpy(rtps_msg+4,ps->parameter,ps->parameterLength);
    } else {
      memcpy(rtps_msg+4,ps->parameterLocal,ps->parameterLength);
    }
    rtps_msg=rtps_msg+4+ps->parameterLength;
    max_msg_len=max_msg_len-ps->parameterLength-4;
    result=result+4+ps->parameterLength;
  }
  return result;
}

/*****************************************************************************/
int
parameterDecodeStreamToCSChange(CSChange *csChange,u_int8_t *rtps_msg,
    u_int16_t submsg_len,u_int8_t e_bit) {
  ParameterId       parameterId;
  ParameterLength   parameterLength;
  ParameterSequence *ps;
  u_int16_t         counter=0;
  u_int8_t          *rtps_msg_it=rtps_msg;

  CSChangeAttributes_init_head(csChange);
  //count number of parameters
  while (submsg_len) {
    parameterId=*((ParameterId*)rtps_msg_it);
    conv_u16(&parameterId,e_bit);
    parameterLength=*((ParameterId*)(rtps_msg_it+2));
    conv_u16(&parameterLength,e_bit);
    if (submsg_len>parameterLength) submsg_len-=parameterLength;
    else submsg_len=0;
    if (parameterId==PID_SENTINEL) break;
    counter++;
    rtps_msg_it+=4+parameterLength;
  }
  ps=(ParameterSequence*)MALLOC(sizeof(ParameterSequence)*(counter+1));
  rtps_msg_it=rtps_msg;
  while (counter) {
    parameterId=*((ParameterId*)rtps_msg_it);
    conv_u16(&parameterId,e_bit);
    ps->parameterID=parameterId;
    parameterLength=*((ParameterId*)(rtps_msg_it+2));
    conv_u16(&parameterLength,e_bit);
    ps->parameterLength=parameterLength;
    ps->parameter=NULL;
    switch (parameterId) {
      case PID_SENTINEL:
        counter=1;
        break;
      //NtpTime
      case PID_EXPIRATION_TIME:
      case PID_PERSISTENCE:
      case PID_MINIMUM_SEPARATION:
        *(NtpTime*)ps->parameterLocal=*((NtpTime*)(rtps_msg_it+4));
        conv_ntp((NtpTime*)ps->parameterLocal,e_bit);
        break;
      //unsigned long
      case PID_TYPE_CHECKSUM:
      case PID_STRENGTH:
      case PID_SEND_QUEUE_SIZE:
      case PID_RECV_QUEUE_SIZE:
      case PID_RELIABILITY_OFFERED:
      case PID_RELIABILITY_REQUESTED:
      case PID_MANAGER_KEY:
        *(u_int32_t*)ps->parameterLocal=*((u_int32_t*)(rtps_msg_it+4));
        conv_u32((u_int32_t*)ps->parameterLocal,e_bit);
        break;
      //IPAddress
      case PID_MATATRAFFIC_MULTICAST_IPADDRESS:
      case PID_APP_IPADDRESS:
      case PID_USERDATA_MULTICAST_IPADDRESS:
        *(IPAddress*)ps->parameterLocal=*((IPAddress*)(rtps_msg_it+4));
        conv_u32((IPAddress*)ps->parameterLocal,e_bit);
        break;
      //Port
      case PID_METATRAFFIC_UNICAST_PORT:
      case PID_USERDATA_UNICAST_PORT:
        *(Port*)ps->parameterLocal=*((Port*)(rtps_msg_it+4));
        conv_u32((Port*)ps->parameterLocal,e_bit);
        break;
      //VendorId
      case PID_VENDOR_ID:
        *(VendorId*)ps->parameterLocal=*((VendorId*)(rtps_msg_it+4));
        break;
      //ProtocolVersion
      case PID_PROTOCOL_VERSION:
        *(ProtocolVersion*)ps->parameterLocal=*((ProtocolVersion*)(rtps_msg_it+4));
        break;
      //SequenceNumber
      case PID_VARGAPPS_SEQUENCE_NUMBER_LAST:
        *(SequenceNumber*)ps->parameterLocal=*((SequenceNumber*)(rtps_msg_it+4));
        conv_sn((SequenceNumber*)ps->parameterLocal,e_bit);
        break;
      //Boolean
      case PID_EXPECTS_ACK:
        *(Boolean*)ps->parameterLocal=*((Boolean*)(rtps_msg_it+4));
        break;
      //PathName,TypeName
      case PID_TOPIC:
      case PID_TYPE_NAME:
        if (ps->parameterLength>MAX_PARAMETER_LOCAL_LENGTH) {
          ps->parameter=MALLOC(ps->parameterLength);
          memcpy(ps->parameter,rtps_msg_it+4,ps->parameterLength);
        } else {
          memcpy(ps->parameterLocal,rtps_msg_it+4,ps->parameterLength);
        }
        break;
      case PID_PAD:
        break;
    }
    CSChangeAttributes_insert(csChange,ps);
    ps++; //next ps
    counter--;
    rtps_msg_it+=4+parameterLength;
  }
  //SENTINEL
  ps->parameterID=PID_SENTINEL;
  ps->parameterLength=0;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  return ORTE_TRUE;
}

/*****************************************************************************/
int
parameterUpdateCSChange(
     CSChange *csChange,AppParams *ap,Boolean Manager) {
  ParameterSequence *ps;
  int               i;

  i=ap->managerKeyCount+
    ap->metatrafficMulticastIPAddressCount+
    ap->unicastIPAddressCount+7;
  if (!Manager) i--;
  CSChangeAttributes_init_head(csChange);
  ps=(ParameterSequence*)MALLOC(sizeof(ParameterSequence)*i);
  //EXPIRATION_TIME
  ps->parameterID=PID_EXPIRATION_TIME;
  ps->parameterLength=8;
  memcpy(ps->parameterLocal,&ap->expirationTime,ps->parameterLength);
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;  
  //MATATRAFFIC_MULTICAST_IPADDRESS
  for (i=0;i<ap->metatrafficMulticastIPAddressCount;i++) {
    ps->parameterID=PID_MATATRAFFIC_MULTICAST_IPADDRESS;
    ps->parameterLength=4;
    *(IPAddress*)ps->parameterLocal=ap->metatrafficMulticastIPAddressList[i];
    ps->parameter=NULL;
    CSChangeAttributes_insert(csChange,ps);
    ps++;
  }
  //METATRAFFIC_UNICAST_PORT
  ps->parameterID=PID_METATRAFFIC_UNICAST_PORT;
  ps->parameterLength=4;
  *(Port*)&ps->parameterLocal=ap->metatrafficUnicastPort;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;  
  //USERDATA_UNICAST_PORT
  ps->parameterID=PID_USERDATA_UNICAST_PORT;
  ps->parameterLength=4;
  *(Port*)&ps->parameterLocal=ap->userdataUnicastPort;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;  
  //APP_IPADDRESS
  for (i=0;i<ap->unicastIPAddressCount;i++) {
    ps->parameterID=PID_APP_IPADDRESS;;
    ps->parameterLength=4;
    *(IPAddress*)ps->parameterLocal=ap->unicastIPAddressList[i];
    ps->parameter=NULL;
    CSChangeAttributes_insert(csChange,ps);
    ps++;
  }
  //PROTOCOL_VERSION
  ps->parameterID=PID_PROTOCOL_VERSION;
  ps->parameterLength=2;
  *(ProtocolVersion*)&ps->parameterLocal=ap->protocolVersion;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;  
  //VENDOR_ID
  ps->parameterID=PID_VENDOR_ID;
  ps->parameterLength=2;
  *(VendorId*)&ps->parameterLocal=ap->vendorId;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;  
  if (Manager) {    //Manager
    //VARGAPPS_SEQUENCE_NUMBER_LAST
    ps->parameterID=PID_VARGAPPS_SEQUENCE_NUMBER_LAST;
    ps->parameterLength=8;
    *(SequenceNumber*)&ps->parameterLocal=ap->vargAppsSequenceNumber;
    ps->parameter=NULL;
    CSChangeAttributes_insert(csChange,ps);
    ps++;  
  }
  //MANAGER_KEY
  for (i=0;i<ap->managerKeyCount;i++) {
    ps->parameterID=PID_MANAGER_KEY;
    ps->parameterLength=4;
    *(u_int32_t*)ps->parameterLocal=ap->managerKeyList[i];
    ps->parameter=NULL;
    CSChangeAttributes_insert(csChange,ps);
    ps++;
  }
  //SENTINEL
  ps->parameterID=PID_SENTINEL;
  ps->parameterLength=0;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  return 0;
} 

/*****************************************************************************/
int
parameterUpdateCSChangeFromPublication(CSChange *csChange,ORTEPublProp *pp) {
  int               len;
  ParameterSequence *ps;
  
  CSChangeAttributes_init_head(csChange);
  ps=(ParameterSequence*)MALLOC(sizeof(ParameterSequence)*8);
  //persistence
  ps->parameterID=PID_PERSISTENCE;
  ps->parameterLength=8;
  memcpy(ps->parameterLocal,&pp->persistence,ps->parameterLength);
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;  
  /* reliabitityOffered */
  ps->parameterID=PID_RELIABILITY_OFFERED;
  ps->parameterLength=4;
  *(u_int32_t*)ps->parameterLocal=pp->reliabilityOffered;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;
  /* sendQueueSize */
  ps->parameterID=PID_SEND_QUEUE_SIZE;
  ps->parameterLength=4;
  *(u_int32_t*)ps->parameterLocal=pp->sendQueueSize;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;
  /* strength */
  ps->parameterID=PID_STRENGTH;
  ps->parameterLength=4;
  *(u_int32_t*)ps->parameterLocal=pp->strength;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;
  /* topic */
  ps->parameterID=PID_TOPIC;
  len=strlen(pp->topic);
  ps->parameter=(int8_t*)MALLOC(len+8);
  *(u_int32_t*)ps->parameter=len+1;
  strncpy((4+ps->parameter),pp->topic,len);
  *(u_int32_t*)(ps->parameter+len+4)=0;
  ps->parameterLength=len+8;
  CSChangeAttributes_insert(csChange,ps);
  ps++;  
  /* typeCheckSum */
  ps->parameterID=PID_TYPE_CHECKSUM;
  ps->parameterLength=4;
  *(u_int32_t*)ps->parameterLocal=pp->typeChecksum;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;
  /* typeName */
  ps->parameterID=PID_TYPE_NAME;
  len=strlen(pp->typeName);
  ps->parameter=(int8_t*)MALLOC(len+8);
  *(u_int32_t*)ps->parameter=len+1;
  strncpy((4+ps->parameter),pp->typeName,len);
  *(u_int32_t*)(ps->parameter+len+4)=0;
  ps->parameterLength=len+8;
  CSChangeAttributes_insert(csChange,ps);
  ps++;  
  /* sentinel -> end */  
  ps->parameterID=PID_SENTINEL;
  ps->parameterLength=0;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  return 0;
} 

/*****************************************************************************/
int
parameterUpdateCSChangeFromSubscription(CSChange *csChange,ORTESubsProp *sp) {
  int               len;
  ParameterSequence *ps;
  
  CSChangeAttributes_init_head(csChange);
  ps=(ParameterSequence*)MALLOC(sizeof(ParameterSequence)*7);
  //minimum separation
  ps->parameterID=PID_MINIMUM_SEPARATION;
  ps->parameterLength=8;
  memcpy(ps->parameterLocal,&sp->minimumSeparation,ps->parameterLength);
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;  
  /* receive queue size*/
  ps->parameterID=PID_RECV_QUEUE_SIZE;
  ps->parameterLength=4;
  *(u_int32_t*)ps->parameterLocal=sp->recvQueueSize;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;
  /* reliabitityRequested */
  ps->parameterID=PID_RELIABILITY_REQUESTED;
  ps->parameterLength=4;
  *(u_int32_t*)ps->parameterLocal=sp->reliabilityRequested;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;
  /* topic */
  ps->parameterID=PID_TOPIC;
  len=strlen(sp->topic);
  ps->parameter=(int8_t*)MALLOC(len+8);
  *(u_int32_t*)ps->parameter=len+1;
  strncpy((4+ps->parameter),sp->topic,len);
  *(u_int32_t*)(ps->parameter+len+4)=0;
  ps->parameterLength=len+8;
  CSChangeAttributes_insert(csChange,ps);
  ps++;  
  /* typeCheckSum */
  ps->parameterID=PID_TYPE_CHECKSUM;
  ps->parameterLength=4;
  *(u_int32_t*)ps->parameterLocal=sp->typeChecksum;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  ps++;
  /* typeName */
  ps->parameterID=PID_TYPE_NAME;
  len=strlen(sp->typeName);
  ps->parameter=(int8_t*)MALLOC(len+8);
  *(u_int32_t*)ps->parameter=len+1;
  strncpy((4+ps->parameter),sp->typeName,len);
  *(u_int32_t*)(ps->parameter+len+4)=0;
  ps->parameterLength=len+8;
  CSChangeAttributes_insert(csChange,ps);
  ps++;  
  /* sentinel -> end */  
  ps->parameterID=PID_SENTINEL;
  ps->parameterLength=0;
  ps->parameter=NULL;
  CSChangeAttributes_insert(csChange,ps);
  return 0;
} 

/*****************************************************************************/
int
parameterUpdateApplication(CSChange *csChange,AppParams *ap) {
  ParameterSequence   *ps;

  ap->managerKeyCount=0;
  ap->metatrafficMulticastIPAddressCount=0;
  ap->unicastIPAddressCount=0;
  ap->appId=csChange->guid.aid;
  ap->hostId=csChange->guid.hid;
  ul_list_for_each(CSChangeAttributes,csChange,ps) {
    switch (ps->parameterID) {
      case PID_EXPIRATION_TIME:
        ap->expirationTime=*(NtpTime*)ps->parameterLocal;
        break;
      case PID_MANAGER_KEY:
        ap->managerKeyList[ap->managerKeyCount]=
            *(u_int32_t*)ps->parameterLocal;
        ap->managerKeyCount++;
        break;
      case PID_MATATRAFFIC_MULTICAST_IPADDRESS:
        ap->metatrafficMulticastIPAddressList[ap->metatrafficMulticastIPAddressCount]=
            *(IPAddress*)ps->parameterLocal;
        ap->metatrafficMulticastIPAddressCount++;
        break;
      case PID_APP_IPADDRESS:
        ap->unicastIPAddressList[ap->unicastIPAddressCount]=
            *(IPAddress*)ps->parameterLocal;
        ap->unicastIPAddressCount++;
        break;
      case PID_USERDATA_MULTICAST_IPADDRESS:
        break;
      case PID_METATRAFFIC_UNICAST_PORT:
        ap->metatrafficUnicastPort=*(Port*)ps->parameterLocal;
        break;
      case PID_USERDATA_UNICAST_PORT:
        ap->userdataUnicastPort=*(Port*)ps->parameterLocal;
        break;
      case PID_VENDOR_ID:
        ap->vendorId=*(VendorId*)ps->parameterLocal;
        break;
      case PID_PROTOCOL_VERSION:
        ap->protocolVersion=*(ProtocolVersion*)ps->parameterLocal;
        break;
      case PID_VARGAPPS_SEQUENCE_NUMBER_LAST:
        ap->vargAppsSequenceNumber=*(SequenceNumber*)ps->parameterLocal;
        break;
    }
  }
  return 0;
}

/*****************************************************************************/
int
parameterUpdatePublication(CSChange *csChange,ORTEPublProp *pp) {
  ParameterSequence   *ps;

  ul_list_for_each(CSChangeAttributes,csChange,ps) {
    switch (ps->parameterID) {
      case PID_EXPECTS_ACK:
        pp->expectsAck=*(Boolean*)&ps->parameterLocal;
      break;
      case PID_PERSISTENCE:
        pp->persistence=*(NtpTime*)&ps->parameterLocal;
      break;
      case PID_RELIABILITY_OFFERED:
        pp->reliabilityOffered=*(u_long*)&ps->parameterLocal;
      break;
      case PID_SEND_QUEUE_SIZE:
        pp->sendQueueSize=*(u_long*)&ps->parameterLocal;
      break;
      case PID_STRENGTH:
        pp->strength=*(int32_t*)&ps->parameterLocal;
      break;
      case PID_TOPIC:
        if (ps->parameterLength>MAX_PARAMETER_LOCAL_LENGTH)
          strncpy(pp->topic,ps->parameter+4,ps->parameterLength-3);
        else
          strncpy(pp->topic,ps->parameterLocal+4,ps->parameterLength-3);
      break;
      case PID_TYPE_CHECKSUM:
        pp->typeChecksum=*(TypeChecksum*)&ps->parameterLocal;
      break;
      case PID_TYPE_NAME:
        if (ps->parameterLength>MAX_PARAMETER_LOCAL_LENGTH)
          strncpy(pp->typeName,ps->parameter+4,ps->parameterLength-3);
        else
          strncpy(pp->typeName,ps->parameterLocal+4,ps->parameterLength-3);
      break;      
    }
  }
  return 0;
}

/*****************************************************************************/
int
parameterUpdateSubscription(CSChange *csChange,ORTESubsProp *sp) {
  ParameterSequence   *ps;

  ul_list_for_each(CSChangeAttributes,csChange,ps) {
    switch (ps->parameterID) {
      case PID_MINIMUM_SEPARATION:
        sp->minimumSeparation=*(NtpTime*)&ps->parameterLocal;
      break;
      case PID_RECV_QUEUE_SIZE:
        sp->recvQueueSize=*(u_int32_t*)&ps->parameterLocal;
      break;
      case PID_RELIABILITY_REQUESTED:
        sp->reliabilityRequested=*(u_int32_t*)&ps->parameterLocal;
      break;
      case PID_TOPIC:
        if (ps->parameterLength>MAX_PARAMETER_LOCAL_LENGTH)
          strncpy(sp->topic,ps->parameter+4,ps->parameterLength-3);
        else
          strncpy(sp->topic,ps->parameterLocal+4,ps->parameterLength-3);
      break;
      case PID_TYPE_CHECKSUM:
        sp->typeChecksum=*(TypeChecksum*)&ps->parameterLocal;
      break;
      case PID_TYPE_NAME:
        if (ps->parameterLength>MAX_PARAMETER_LOCAL_LENGTH)
          strncpy(sp->typeName,ps->parameter+4,ps->parameterLength-3);
        else
          strncpy(sp->typeName,ps->parameterLocal+4,ps->parameterLength-3);
      break;
    }
  }
  return 0;
}

