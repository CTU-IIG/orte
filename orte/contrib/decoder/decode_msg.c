#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <endian.h>
#include <stdint.h>
#include <byteswap.h>
#include <typedefs_defines_rtps.h>

// global variables
  FILE *ptr_in, *ptr_out;


/**********************************************************************************/
void conv_u32(uint32_t *x,uint8_t ef) {
  #if __BYTE_ORDER == __LITTLE_ENDIAN
     if(ef) bswap_32(*x);
  #elif __BYTE_ORDER == __BIG_ENDIAN
     if(!ef)bswap_32(*x);
  #endif
}

/**********************************************************************************/
void print_obj(void *obj) {
  fprintf(ptr_out," (");
  switch (bswap_32(*((uint32_t*)obj))) {
    case OID_APP:
      fprintf(ptr_out,"ManagedApplicaton or Manager");
      break;
    case OID_WRITE_APPSELF:
      fprintf(ptr_out,"Write appself");
      break;
    case OID_WRITE_APP:
      fprintf(ptr_out,"Write app");
      break;
    case OID_READ_APP:
      fprintf(ptr_out,"Read app");
      break;
    case OID_WRITE_MGR:
      fprintf(ptr_out,"Write mgr");
      break;
    case OID_READ_MGR:
      fprintf(ptr_out,"Read mgr");
      break;
    case OID_WRITE_PUBL:
      fprintf(ptr_out,"Write publ");
      break;
    case OID_READ_PUBL:
      fprintf(ptr_out,"Read publ");
      break;
    case OID_WRITE_SUBS:
      fprintf(ptr_out,"Write subs");
      break;
    case OID_READ_SUBS:
      fprintf(ptr_out,"Read subs");
      break;
    default:
      fprintf(ptr_out,"unknown");
      break;
  }
  fprintf(ptr_out,")\n");
}		      

/**********************************************************************************/
void print_app(uint8_t app) {
  switch (app) {
    case MANAGEDAPPLICATION:
      fprintf(ptr_out,"(managed application)\n");
      break;
    case MANAGER:
      fprintf(ptr_out,"(manager)\n");
      break;
    default:
      fprintf(ptr_out,"(unknown class)\n");
      break;    
  }
}  			   

/**********************************************************************************/
void decode_parametr_sequence(uint8_t *msg) {
  NtpTime            ts;
  ProtocolVersion    pv;
  VendorId           vid;
  SequenceNumber     sn;
  uint8_t      i,j,l;
  uint8_t      *k;

  while(*((ParameterId*)msg)!=PID_SENTINEL) {
    printf("id%d,len%d\n",*((ParameterId*)msg),*((ParameterLength*)(msg+2)));
    if ((*((ParameterLength*)(msg+2)))==0) break;
    switch(*(ParameterId*)msg) {
      case PID_PAD:
        fprintf(ptr_out,"     PID_PAD                              :\n");
        break;
      case PID_EXPIRATION_TIME:
        ts=*((NtpTime*)(msg+4));
        fprintf(ptr_out,"     PID_EXPIRATION_TIME                  : %u,%u\n",
	                ts.seconds,ts.fraction);
        break;
      case PID_PERSISTENCE:
        ts=*((NtpTime*)(msg+4));
        fprintf(ptr_out,"     PID_PERSISTENCE                      : %u,%u\n",
	                ts.seconds,ts.fraction);
        break;
      case PID_MINIMUM_SEPARATION:
        ts=*((NtpTime*)(msg+4));
        fprintf(ptr_out,"     PID_MINIMUM_SEPARATION               : %u,%u\n",
	                ts.seconds,ts.fraction);
        break;
      case PID_TOPIC:
        fprintf(ptr_out,"     PID_TOPIC                            :\n");
        k=msg+4;
	//len of message is also in message (first ulong)
        l=(*(uint32_t*)(msg+4))+4;
        while(l!=0) {
          if (l>8) j=8;
    	  else j=l;
 	  l-=j; 
          fprintf(ptr_out,"                    : ");
          for(i=0;i<j;i++,k++) fprintf(ptr_out,"%02X ",k[0]);
          if (l==0) {
            for(i=j;i<8;i++) fprintf(ptr_out,"   ");
	  } 
	  k-=j;
          for(i=0;i<j;i++,k++) fprintf(ptr_out,"%c",k[0]);
          fprintf(ptr_out,"\n");
        } 
        break;
      case PID_STRENGTH:
        fprintf(ptr_out,"     PID_STRENGTH                         : %u\n",
	                *(uint32_t*)(msg+4));
        break;
      case PID_TYPE_NAME:
        fprintf(ptr_out,"     PID_TYPE_NAME                        :\n");
        k=msg+4;
	//len of message is also in message (first ulong)
        l=(*(uint32_t*)(msg+4))+4;
        while(l!=0) {
          if (l>8) j=8;
    	  else j=l;
 	  l-=j; 
          fprintf(ptr_out,"                    : ");
          for(i=0;i<j;i++,k++) fprintf(ptr_out,"%02X ",k[0]);
          if (l==0) {
            for(i=j;i<8;i++) fprintf(ptr_out,"   ");
	  } 
	  k-=j;
          for(i=0;i<j;i++,k++) fprintf(ptr_out,"%c",k[0]);
          fprintf(ptr_out,"\n");
        } 
        break;
      case PID_TYPE_CHECKSUM:
        fprintf(ptr_out,"     PID_TYPE_CHECKSUM                    : %u\n",
	                *(TypeChecksum*)(msg+4));
        break;
      case RTPS_PID_TYPE2_NAME:
        fprintf(ptr_out,"     RTPS_PID_TYPE2_NAME                  :\n");
        break;
      case RTPS_PID_TYPE2_CHECKSUM:
        fprintf(ptr_out,"     RTPS_PID_TYPE2_CHECKSUM              :\n");
        break;
      case PID_MATATRAFFIC_MULTICAST_IPADDRESS:
        fprintf(ptr_out,"     PID_MATATRAFFIC_MULTICAST_IPADDRESS  : %u.%u.%u.%u\n",
	                msg[7],msg[6],msg[5],msg[4]);
        break;
      case PID_APP_IPADDRESS:
        fprintf(ptr_out,"     PID_APP_IPADDRESS                    : %u.%u.%u.%u\n",
	                msg[7],msg[6],msg[5],msg[4]);
        break;
      case PID_METATRAFFIC_UNICAST_PORT:
        fprintf(ptr_out,"     PID_METATRAFFIC_UNICAST_PORT         : %u\n",
	                *(Port*)(msg+4));
        break;
      case PID_USERDATA_UNICAST_PORT:
        fprintf(ptr_out,"     PID_USERDATA_UNICAST_PORT            : %u\n",
	                *(Port*)(msg+4));
        break;
      case PID_EXPECTS_ACK:
        fprintf(ptr_out,"     PID_EXPECTS_ACK                      : %u\n",
	                *(uint32_t*)(msg+4));
        break;
      case PID_USERDATA_MULTICAST_IPADDRESS:
        fprintf(ptr_out,"     PID_USERDATA_MULTICAST_IPADDRESS     : %u.%u.%u.%u\n",
	                msg[7],msg[6],msg[5],msg[4]);
        break;
      case PID_MANAGER_KEY:
        fprintf(ptr_out,"     PID_MANAGER_KEY                      : 0x%02x%02x%02x%02x (%u.%u.%u.%u)\n",
	                msg[7],msg[6],msg[5],msg[4],msg[7],msg[6],msg[5],msg[4]);
        break;
      case PID_SEND_QUEUE_SIZE:
        fprintf(ptr_out,"     PID_SEND_QUEUE_SIZE                  : %u\n",
	                *(uint32_t*)(msg+4));
        break;
      case PID_PROTOCOL_VERSION:
        pv=*((ProtocolVersion*)(msg+4));
        fprintf(ptr_out,"     PID_PROTOCOL_VERSION                 : %u.%u\n",
	        pv.major,pv.minor);
        break;
      case PID_VENDOR_ID:
        vid=*((VendorId*)(msg+4));
        fprintf(ptr_out,"     PID_VENDOR_ID                        : %u.%u\n",
	        vid.major,vid.minor);
        break;
      case PID_VARGAPPS_SEQUENCE_NUMBER_LAST:
        sn=*((SequenceNumber*)(msg+4));
        fprintf(ptr_out,"     PID_VARGAPPS_SEQUENCE_NUMBER_LAST    : %u\n",sn.low);
        break;
      case PID_RECV_QUEUE_SIZE:
        fprintf(ptr_out,"     PID_RECV_QUEUE_SIZE                  : %u\n",
	                *(uint32_t*)(msg+4));
        break;
      case PID_RELIABILITY_OFFERED:
        fprintf(ptr_out,"     PID_RELIABILITY_OFFERED              : %u\n",
	                *(uint32_t*)(msg+4));
        break;
      case PID_RELIABILITY_REQUESTED:
        fprintf(ptr_out,"     PID_RELIABILITY_REQUESTED            : %u\n",
	                *(uint32_t*)(msg+4));
        break;
      default:
        fprintf(ptr_out,"     unknow paramerId                 : %d len : %d\n",
                *((ParameterId*)msg),*((ParameterLength*)(msg+2)));
	break;      
    }
    msg+=(*((ParameterLength*)(msg+2)))+4;                      /* next message */
  }
  msg+=(*((ParameterLength*)(msg+2)))+4;                        /* message end*/
}


/**********************************************************************************/
int decode_header(uint8_t *msg, unsigned len, MessageInterpret *mi) {
  if (len<16) return -1;                          /* message is to small */
  if (msg[0]!='R') return -2;                     /* header is invalid */
  if (msg[1]!='T') return -2;                     /* header is invalid */
  if (msg[2]!='P') return -2;                     /* header is invalid */
  if (msg[3]!='S') return -2;                     /* header is invalid */
  mi->sourceVersion=*((ProtocolVersion*)(msg+4)); /* ProtocolVersion */
  mi->sourceVendorId=*((VendorId*)(msg+6));       /* Vendor Id */
  mi->sourceHostId=*((HostId*)(msg+8));           /* Host Id */
  mi->sourceAppId=*((AppId*)(msg+12));            /* App Id */
  mi->haveTimestamp=0;			          /* false */
  return 0;
}

/**********************************************************************************/
int decode_submessage(uint8_t *msg, int ptr, MessageInterpret *mi) {
  IPAddress          ipa;
  Port               port;
  ObjectId           roid,woid,oid;
  SequenceNumber     fsn,lsn,wsn;
  uint8_t      i,j,l;
  uint8_t      *k;
  HostId             hid;
  AppId              aid;
  int                ptr_off;

  switch ((SubmessageId)msg[ptr]) {
    case PAD:
      fprintf(ptr_out,"   PAD\n");
      break;
    case VAR:
      fprintf(ptr_out,"   VAR\n");
      fprintf(ptr_out,"     h-bit          : %d\n",(msg[ptr+1] & 8) ? 1:0);
      fprintf(ptr_out,"     alive-bit      : %d\n",(msg[ptr+1] & 4) ? 1:0);
      fprintf(ptr_out,"     p-bit          : %d\n",(msg[ptr+1] & 2) ? 1:0);
      roid=*((ObjectId*)(msg+ptr+4));
      fprintf(ptr_out,"     ObjectId       : %d.%d.%d.%02x",
                        *(uint8_t*)&roid,*(((uint8_t*)&roid)+1),*(((uint8_t*)&roid)+2),*(((uint8_t*)&roid)+3));
      print_obj(&roid);		      
      woid=*((ObjectId*)(msg+ptr+8));
      fprintf(ptr_out,"     ObjectId       : %d.%d.%d.%02x",
                        *(uint8_t*)&woid,*(((uint8_t*)&woid)+1),*(((uint8_t*)&woid)+2),*(((uint8_t*)&woid)+3));
      print_obj(&woid);		      
      if (msg[ptr+1] & 8) {
        hid=*((HostId*)(msg+ptr+12));                   /* hostId */
        aid=*((AppId*)(msg+ptr+16));                   /* appId */
        fprintf(ptr_out,"     HostId         : %d.%d.%d.%d\n",
                        *(uint8_t*)&hid,*(((uint8_t*)&hid)+1),*(((uint8_t*)&hid)+2),*(((uint8_t*)&hid)+3));
        fprintf(ptr_out,"     AppId          : %d.%d.%d.%d ",
                        *(uint8_t*)&aid,*(((uint8_t*)&aid)+1),*(((uint8_t*)&aid)+2),*(((uint8_t*)&aid)+3));
        print_app(*(((uint8_t*)&aid)+3));
        ptr_off=8;
      } else ptr_off=0;
      oid=*((ObjectId*)(msg+ptr+12+ptr_off));
      fprintf(ptr_out,"     ObjectId       : %d.%d.%d.%02x",
                        *(uint8_t*)&oid,*(((uint8_t*)&oid)+1),*(((uint8_t*)&oid)+2),*(((uint8_t*)&oid)+3));
      print_obj(&oid);		      
      wsn=*((SequenceNumber*)(msg+ptr+16+ptr_off));
      fprintf(ptr_out,"     sequenceNumber : %u\n",wsn.low);		      
      if (msg[ptr+1] & 2) {
        decode_parametr_sequence(msg+ptr+24+ptr_off);
      }
      break;
    case ISSUE:
      fprintf(ptr_out,"   ISSUE\n");
      roid=*((ObjectId*)(msg+ptr+4));woid=*((ObjectId*)(msg+ptr+8));
      fsn=*((SequenceNumber*)(msg+ptr+12));
      fprintf(ptr_out,"     p-bit          : %d\n",(msg[ptr+1] & 2) ? 1:0);
      fprintf(ptr_out,"     readerObjectId : %d.%d.%d.%02x",
                      *(uint8_t*)&roid,*(((uint8_t*)&roid)+1),*(((uint8_t*)&roid)+2),*(((uint8_t*)&roid)+3));
      print_obj(&roid);		      
      fprintf(ptr_out,"     writeObjectId  : %d.%d.%d.%02x",
                      *(uint8_t*)&woid,*(((uint8_t*)&woid)+1),*(((uint8_t*)&woid)+2),*(((uint8_t*)&woid)+3));
      print_obj(&woid);		      
      fprintf(ptr_out,"     issueSeqNumber : %u\n",fsn.low);		      
      k=msg+ptr+20;
      if (msg[ptr+1] & 2) {
        decode_parametr_sequence(k);          /* after k is set to end of paramseq. */
      }
//      l=(*(unsigned*)(msg+ptr+2))-(k-(msg+ptr+20)+16);
      //len of message is also in message (first ulong), internaly defined by RTI !!! 
      l=(*(uint32_t*)k)+4;
      while(l!=0) {
        if (l>8) j=8;
	else j=l;
 	l-=j; 
        fprintf(ptr_out,"                    : ");
        for(i=0;i<j;i++,k++) fprintf(ptr_out,"%02X ",k[0]);
        if (l==0) {
          for(i=j;i<8;i++) fprintf(ptr_out,"   ");
        } 
	k-=j;
        for(i=0;i<j;i++,k++) fprintf(ptr_out,"%c",k[0]);
        fprintf(ptr_out,"\n");
      }	
      break;
    case ACK:
      fprintf(ptr_out,"   ACK\n");
      roid=*((ObjectId*)(msg+ptr+4));woid=*((ObjectId*)(msg+ptr+8));
      fprintf(ptr_out,"     final-bit      : %d\n",(msg[ptr+1] & 2) ? 1:0);
      fprintf(ptr_out,"     readerObjectId : %d.%d.%d.%02x",
                      *(uint8_t*)&roid,*(((uint8_t*)&roid)+1),*(((uint8_t*)&roid)+2),*(((uint8_t*)&roid)+3));
      print_obj(&roid);		      
      fprintf(ptr_out,"     writeObjectId  : %d.%d.%d.%02x",
                      *(uint8_t*)&woid,*(((uint8_t*)&woid)+1),*(((uint8_t*)&woid)+2),*(((uint8_t*)&woid)+3));
      print_obj(&woid);		      
      fprintf(ptr_out,"     bitmap         : %u/%u:",*((uint32_t*)(msg+ptr+16)),
                      *((uint32_t*)(msg+ptr+20)));
      for(i=0;i<msg[ptr+20]/8;) {
        for(j=128;j!=0;j>>=1) fprintf(ptr_out,"%d",(msg[ptr+24+i] & j) ? 1:0);
	fprintf(ptr_out," ");
	if ((!(++i % 4)) && (i<msg[ptr+20]/8)) 
	  fprintf(ptr_out,"\n                          :");
      }
      fprintf(ptr_out,"\n");		      
      break;
    case HEARTBEAT:
      fprintf(ptr_out,"   HEARTBEAT\n");
      roid=*((ObjectId*)(msg+ptr+4));woid=*((ObjectId*)(msg+ptr+8));
      fsn=*((SequenceNumber*)(msg+ptr+12));lsn=*((SequenceNumber*)(msg+ptr+20));
      fprintf(ptr_out,"     final-bit      : %d\n",(msg[ptr+1] & 2) ? 1:0);
      fprintf(ptr_out,"     readerObjectId : %d.%d.%d.%02x",
                      *(uint8_t*)&roid,*(((uint8_t*)&roid)+1),*(((uint8_t*)&roid)+2),*(((uint8_t*)&roid)+3));
      print_obj(&roid);		      
      fprintf(ptr_out,"     writeObjectId  : %d.%d.%d.%02x",
                      *(uint8_t*)&woid,*(((uint8_t*)&woid)+1),*(((uint8_t*)&woid)+2),*(((uint8_t*)&woid)+3));
      print_obj(&woid);		      
      fprintf(ptr_out,"     firstSeqNumber : %u\n",fsn.low);		      
      fprintf(ptr_out,"     lastSeqNumber  : %u\n",lsn.low);		      
      break;
    case GAP:
      fprintf(ptr_out,"   GAP\n");
      roid=*((ObjectId*)(msg+ptr+4));woid=*((ObjectId*)(msg+ptr+8));
      fsn=*((SequenceNumber*)(msg+ptr+12));
      lsn=*((SequenceNumber*)(msg+ptr+20));
      fprintf(ptr_out,"     readerObjectId : %d.%d.%d.%02x",
                      *(uint8_t*)&roid,*(((uint8_t*)&roid)+1),*(((uint8_t*)&roid)+2),*(((uint8_t*)&roid)+3));
      print_obj(&roid);		      
      fprintf(ptr_out,"     writeObjectId  : %d.%d.%d.%02x",
                      *(uint8_t*)&woid,*(((uint8_t*)&woid)+1),*(((uint8_t*)&woid)+2),*(((uint8_t*)&woid)+3));
      print_obj(&woid);		      
      fprintf(ptr_out,"     firstSeqNumber : %u\n",fsn.low);		      
      fprintf(ptr_out,"     bitmap         : %u/%u:",lsn.low,
                      *((uint32_t*)(msg+ptr+28)));
      for(i=0;i<(*((uint32_t*)(msg+ptr+28)));i++) {
        l=((*(((uint32_t*)(msg+ptr+32))+i/32)) & (1<<(31-i%32))) ? 1:0;
        fprintf(ptr_out,"%d",l);
      }
      fprintf(ptr_out,"\n");		      
      break;
    case INFO_TS:
      fprintf(ptr_out,"   INFO_TS\n");
      fprintf(ptr_out,"     i-bit          : %d\n",(msg[ptr+1] & 2) ? 1:0);
      if (!(msg[ptr+1]&2)) {
        mi->timestamp=*((NtpTime*)(msg+ptr+4));
        fprintf(ptr_out,"     ntpTimeStamp   : %u,%u\n",
	                mi->timestamp.seconds,mi->timestamp.fraction);
      }	
      break;
    case INFO_SRC:
      fprintf(ptr_out,"   INFO_SRC\n");
      break;
    case INFO_REPLY:
      fprintf(ptr_out,"   INFO_REPLY\n");
      ipa=*((uint32_t*)(msg+ptr+4));                  /* unicastReplyIPAddress */
      port=*((uint32_t*)(msg+ptr+8));                 /* unicastReplyPort */
      fprintf(ptr_out,"     unicastReplyIPAddress   : %d.%d.%d.%d\n",
                      *(uint8_t*)&ipa,*(((uint8_t*)&ipa)+1),*(((uint8_t*)&ipa)+2),*(((uint8_t*)&ipa)+3));
      fprintf(ptr_out,"     unicastReplyPort        : %u\n",port);
      if (ipa!=IPADDRESS_INVALID) {
        mi->unicastReplyIPAddress=ipa;
      }
      mi->unicastReplyPort=port; 
      if ((msg[ptr+1] & 0x02) !=0) {
        ipa=*((uint32_t*)(msg+ptr+12));               /* multicastReplyIPAddress */
        port=*((uint32_t*)(msg+ptr+16));              /* multicastReplyPort */
        fprintf(ptr_out,"     multicastReplyIPAddress : %d.%d.%d.%d\n",
                      *(uint8_t*)&ipa,*(((uint8_t*)&ipa)+1),*(((uint8_t*)&ipa)+2),*(((uint8_t*)&ipa)+3));
        fprintf(ptr_out,"     multicastReplyPort      : %u\n",port);
        mi->multicastReplyIPAddress=ipa;
        mi->multicastReplyPort=port; 
      } else {
        mi->multicastReplyIPAddress=IPADDRESS_INVALID;
	mi->multicastReplyPort     =PORT_INVALID; 
      }
      break;
    case INFO_DST:
      fprintf(ptr_out,"   INFO_DST\n");
      hid=*((HostId*)(msg+ptr+4));                   /* hostId */
      aid=*((AppId*)(msg+ptr+8));                   /* appId */
      fprintf(ptr_out,"     HostId                  : %d.%d.%d.%d\n",
                        *(uint8_t*)&hid,*(((uint8_t*)&hid)+1),*(((uint8_t*)&hid)+2),*(((uint8_t*)&hid)+3));
      fprintf(ptr_out,"     AppId                   : %d.%d.%d.%d ",
                        *(uint8_t*)&aid,*(((uint8_t*)&aid)+1),*(((uint8_t*)&aid)+2),*(((uint8_t*)&aid)+3));
      print_app(*(((uint8_t*)&aid)+3));
      break;                
    default:
      fprintf(ptr_out,"   undefined submessage code\n");
      break;  
  }  
  return 0;
}


/**********************************************************************************/
int main(int argc, char * argv[]) {
  uint8_t rtps_msg[16384],rtps_msg_c[3]={0,0,0};
  unsigned rtps_msg_ptr=0,rtps_msg_len,rtps_submsg_cnt,submsg_len;
  int err,c,no_lchar=1;
  MessageInterpret mi;
  
  printf("-------------------------------------------------------------------------------\n");
  printf("             RTPS messages decoder                OCERA project                \n");
  printf("                                                                               \n");
  printf("-------------------------------------------------------------------------------\n");
  if (argc != 3) {
    printf(" syntax :    decode_msg 'filename to decode' 'output file'\n\n");
    return 0;
  }
  printf(" decode file    : %s\n output file    : %s\n\n",argv[1],argv[2]);
  ptr_in = fopen(argv[1],"r");                            /* open input file */
  if (ferror(ptr_in)) {
    printf(" unable to open %s\n",argv[1]);
  }
  ptr_out = fopen(argv[2],"w");                           /* create output file */
  if (ferror(ptr_out)) {
    printf(" unable to create %s\n",argv[2]);
  }
  while ((c = fgetc(ptr_in)) !=EOF) {                     /* read one character */
    if (((no_lchar==1) && (!isdigit(c))) || (no_lchar==0)) { /* outside RTPS line */
      if (rtps_msg_ptr) {                                 /* proc. data read data ? */
        rtps_msg[rtps_msg_ptr]=0;		          /* end of rtps data */
	rtps_msg_len=rtps_msg_ptr;rtps_msg_ptr=0;
        printf(" message length : %d\n",rtps_msg_len);
	/* decoded message - msg stored in rtps_msg, length msg in rtps_msg_len */
        if ((err=decode_header(rtps_msg,rtps_msg_len,&mi))==0) {
	  fprintf(ptr_out,"message length      : %d\n",rtps_msg_len);
	  fprintf(ptr_out," Header\n");
	  fprintf(ptr_out,"   protocol version : %d.%d \
	                 \n   vendorId         : %d.%d \
	                 \n   hostId           : %d.%d.%d.%d \
		         \n   appId            : %d.%d.%d.%d ",
	                   mi.sourceVersion.major,mi.sourceVersion.minor,mi.sourceVendorId.major,
			   mi.sourceVendorId.minor,*(u_char*)&mi.sourceHostId,*(((u_char*)&mi.sourceHostId)+1),
			   *(((u_char*)&mi.sourceHostId)+2),*(((u_char*)&mi.sourceHostId)+3),*(((u_char*)&mi.sourceAppId)),
			   *(((u_char*)&mi.sourceAppId)+1),*(((u_char*)&mi.sourceAppId)+2),*(((u_char*)&mi.sourceAppId)+3));
	  print_app(*(((u_char*)&mi.sourceAppId)+3));
	  rtps_msg_ptr=16;rtps_submsg_cnt=1;
	  do {
	    fprintf(ptr_out," Submessage : %d",rtps_submsg_cnt++);
	    if ((rtps_msg_ptr+3)<=rtps_msg_len) {  /* len. submessage header is OK */
	      submsg_len=rtps_msg[rtps_msg_ptr+2]+rtps_msg[rtps_msg_ptr+3]*0x100;
	      fprintf(ptr_out," len : %d\n",submsg_len+4);
	      if ((submsg_len+rtps_msg_ptr+3)<=rtps_msg_len) {
                /* decode submessage */
	        decode_submessage(rtps_msg,rtps_msg_ptr,&mi);
	        rtps_msg_ptr+=submsg_len+4;
	      } else rtps_msg_ptr=rtps_msg_len;    /* submessage is too small */	
	    } else {
	      rtps_msg_ptr=rtps_msg_len;           /* submessage is too small */
	      fprintf(ptr_out,"\n");
	    }  
	  } while (rtps_msg_ptr<rtps_msg_len);
	} else {
  	  fprintf(ptr_out,"invalid header - error code : %d\n",err);
	}
        rtps_msg_ptr=0;
      }
      no_lchar=0;
      if (c=='\n') no_lchar=1;
      fputc(c,ptr_out);                                   /* copy to output file */
    }  
    if (no_lchar>1) {                                     /* reading RTPS message*/
      no_lchar++;
      if (c=='\n') no_lchar=1;
      else {
        if ((no_lchar>5) && (no_lchar<57)) {              /* converted data from */ 
	  if ((rtps_msg_c[0]!=0) && (isxdigit(c))) {      /* position 6 to 56    */
	    rtps_msg_c[1]=(uint8_t)c;
	    rtps_msg[rtps_msg_ptr++]=(uint8_t)strtol(rtps_msg_c,NULL,16);
	    rtps_msg_c[0]=0;
	  } else {
  	    if ((rtps_msg_c[0]==0) && (isxdigit(c))) rtps_msg_c[0]=(uint8_t)c;
	  }
	}
      }
    }
    if ((no_lchar==1) && (isxdigit(c))) no_lchar++;        /* is begin RTPS line ? */
  }
  fclose(ptr_in);
  fclose(ptr_out);
  return 0;
}
