#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "orte.h"

typedef struct motion_speed_type motion_speed;
struct motion_speed_type {
  CORBA_short left;
  CORBA_short right;
};

struct robottype_orte_data {
  ORTEDomain *orte_domain;
  int strength;

  struct motion_speed_type motion_speed;
  ORTEPublication *publication_motion_speed;
};

struct robottype_orte_data orte;
ORTESubscription *s;

int
robottype_roboorte_init(struct robottype_orte_data *data)
{
  int rv = 0;
  ORTEDomainProp prop;

  ORTEInit();

  if (data->strength < 0)
    data->strength = 1;

  ORTEVerbositySetOptions("ALL.0");
  ORTEDomainPropDefaultGet(&prop);
  NTPTIME_BUILD(prop.baseProp.refreshPeriod, 3);
  NTPTIME_BUILD(prop.baseProp.expirationTime, 10);
  data->orte_domain = ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN, &prop, NULL, ORTE_FALSE);
  if (!data->orte_domain) {
    printf("ORTEDomainAppCreate failed!\n");
    rv = -1;
  }

  return rv;
}

void
motion_speed_serialize(CDR_Codec *cdrCodec, motion_speed *object)
{
  CORBA_short_serialize(cdrCodec, &(object->left));

  CORBA_short_serialize(cdrCodec, &(object->right));

}

void
motion_speed_deserialize(CDR_Codec *cdrCodec, motion_speed *object)
{
  CORBA_short_deserialize(cdrCodec, &(object->left));

  CORBA_short_deserialize(cdrCodec, &(object->right));

}

int
motion_speed_get_max_size(ORTEGetMaxSizeParam *gms, int num)
{
  int loop_lim = 2;
  int csize_save;

  while (num) {
    if (!loop_lim--) {
      gms->csize += num*(gms->csize-csize_save);
      return gms->csize;
    }
    num--;
    csize_save = gms->csize;
    CORBA_short_get_max_size(gms, 1);
    CORBA_short_get_max_size(gms, 1);
  }
  return gms->csize;
}

Boolean
motion_speed_type_register(ORTEDomain *d)
{
  Boolean ret;

  ret = ORTETypeRegisterAdd(d,
			    "motion_speed",
			    (ORTETypeSerialize)motion_speed_serialize,
			    (ORTETypeDeserialize)motion_speed_deserialize,
			    motion_speed_get_max_size,
			    0);
}

void
robottype_subscriber_motion_speed_create(struct robottype_orte_data *data, ORTERecvCallBack callback, void *arg)
{
  NtpTime deadline, minimumSeparation;

  motion_speed_type_register(data->orte_domain);

  NtpTimeAssembFromMs(deadline, 0, 300);
  NtpTimeAssembFromMs(minimumSeparation, 0, 0);
  s = ORTESubscriptionCreate(
    data->orte_domain, IMMEDIATE, BEST_EFFORTS,
    "motion_speed", "motion_speed",
    &data->motion_speed, &deadline, &minimumSeparation,
    callback, arg, IPADDRESS_INVALID);
}

void
rcv_motion_speed_cb(const ORTERecvInfo *info, void *vinstance,
		    void *recvCallBackParam)
{
  struct motion_speed_type *m = (struct motion_speed_type *)vinstance;

  switch (info->status) {
    case NEW_DATA:
      printf("Prijata data: left: %d, right: %d\n", m->left, m->right);
      break;
    case DEADLINE:
      printf("ORTE deadline occurred - motion_speed receive\n");
      break;
  }
}

void
destroy(int sig)
{
  ORTESubscriptionDestroy(s);
  ORTETypeRegisterDestroyAll(orte.orte_domain);
  ORTEDomainAppDestroy(orte.orte_domain);
}

int
main(void)
{
  orte.strength = 30;
  robottype_roboorte_init(&orte);
  robottype_subscriber_motion_speed_create(&orte, rcv_motion_speed_cb, &orte);

  signal(SIGINT, &destroy);
  signal(SIGTERM, &destroy);
  pause();

  return 0;
}
