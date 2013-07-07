#include <stdlib.h>
#include <stdio.h>
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

void send_dummy_cb(const ORTESendInfo *info, void *vinstance, void *sendCallBackParam)
{
printf("sent1\n");
}

int robottype_roboorte_init(struct robottype_orte_data *data) {
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

void motion_speed_serialize(CDR_Codec *cdrCodec, motion_speed *object) {
	CORBA_short_serialize(cdrCodec,&(object->left));

	CORBA_short_serialize(cdrCodec,&(object->right));

}

void motion_speed_deserialize(CDR_Codec *cdrCodec, motion_speed *object) {
	CORBA_short_deserialize(cdrCodec,&(object->left));

	CORBA_short_deserialize(cdrCodec,&(object->right));

}

int motion_speed_get_max_size(ORTEGetMaxSizeParam *gms, int num) {
	int loop_lim=2;
	int csize_save;
	while(num) {
		if (!loop_lim--) {
			gms->csize+=num*(gms->csize-csize_save);
			return gms->csize;
		}
		num--;
		csize_save=gms->csize;
		CORBA_short_get_max_size(gms, 1);
		CORBA_short_get_max_size(gms, 1);
	}
	return gms->csize;
}

Boolean
motion_speed_type_register(ORTEDomain *d) {
	Boolean ret;

	ret=ORTETypeRegisterAdd(d,
				"motion_speed",
				(ORTETypeSerialize)motion_speed_serialize,
				(ORTETypeDeserialize)motion_speed_deserialize,
				motion_speed_get_max_size,
				0);
}

void robottype_publisher_motion_speed_create(struct robottype_orte_data *data, ORTESendCallBack callback, void *arg) {
	NtpTime persistance, delay;

	motion_speed_type_register(data->orte_domain);
	NtpTimeAssembFromMs(persistance, 3, 0);
	NtpTimeAssembFromMs(delay, 0, 100);
	printf("delay: s = %d, f = %d\n", delay.seconds, delay.fraction);
	data->publication_motion_speed = ORTEPublicationCreate(data->orte_domain, "motion_speed", "motion_speed", &data->motion_speed, &persistance, data->strength, callback, arg, &delay);
}

int main(void) {
	orte.strength = 30;
	robottype_roboorte_init(&orte);
	robottype_publisher_motion_speed_create(&orte, send_dummy_cb, &orte);

	orte.motion_speed.right = -19000;
	orte.motion_speed.left = 20000;
	ORTEPublicationSend(orte.publication_motion_speed);

	while (1) {
		ORTESleepMs(1000);
	}

	return 0;
}
