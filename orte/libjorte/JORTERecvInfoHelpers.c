#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>
#include <assert.h>

// library header file's path
#include "orte_all.h"
// pregenerated header
#include "jorte/org_ocera_orte_types_RecvInfo.h"
// enable TEST_STAGE run level
#include "jorte/4all.h"
// new data types
#include "jorte/jorte_typedefs_defines.h"
#include "jorte/jorte_protos_api.h"

/*
 * Class:     org_ocera_orte_types_RecvInfo
 * Method:    c_helper
 * Signature: (Ljava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_ocera_orte_types_RecvInfo_c_1helper
  (JNIEnv *env, jclass cls, jobject b_buffer) {

    _Static_assert(sizeof(char*) == 1 || \
                   sizeof(char*) == 2 || \
                   sizeof(char*) == 4 || \
                   sizeof(char*) == 8, \
                   "char* has incompatible size");
    _Static_assert(sizeof(ORTERecvStatus) == 1 || \
                   sizeof(ORTERecvStatus) == 2 || \
                   sizeof(ORTERecvStatus) == 4 || \
                   sizeof(ORTERecvStatus) == 8, \
                   "ORTERecvStatus has incompatible size");

    int32_t* buffer = (*env)->GetDirectBufferAddress(env, b_buffer);
    
    buffer[0] = (int32_t)sizeof(ORTERecvInfo);
    buffer[1] = (int32_t)sizeof(ORTERecvStatus);
    buffer[2] = (int32_t)sizeof(char*);
    buffer[3] = (int32_t)offsetof(ORTERecvInfo,status);
    buffer[4] = (int32_t)offsetof(ORTERecvInfo,topic);
    buffer[5] = (int32_t)offsetof(ORTERecvInfo,type);
    buffer[6] = (int32_t)offsetof(ORTERecvInfo,senderGUID);
    buffer[7] = (int32_t)offsetof(ORTERecvInfo,localTimeReceived);
    buffer[8] = (int32_t)offsetof(ORTERecvInfo,remoteTimePublished);
    buffer[9] = (int32_t)offsetof(ORTERecvInfo,sn);
    buffer[10] = (int32_t)offsetof(GUID_RTPS,hid);
    buffer[11] = (int32_t)offsetof(GUID_RTPS,aid);
    buffer[12] = (int32_t)offsetof(GUID_RTPS,oid);
    buffer[13] = (int32_t)offsetof(NtpTime,seconds);
    buffer[14] = (int32_t)offsetof(NtpTime,fraction);
    buffer[15] = (int32_t)offsetof(SequenceNumber,high);
    buffer[16] = (int32_t)offsetof(SequenceNumber,low);
  }

/*
 * Class:     org_ocera_orte_types_RecvInfo
 * Method:    get_string
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_ocera_orte_types_RecvInfo_get_1string
  (JNIEnv *env, jclass cls, jlong string_ptr) {
    return (*env)->NewStringUTF(env, (char*) string_ptr);
  }
