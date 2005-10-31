#ifndef ofb_cb_H
#define ofb_cb_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef int (*ofb_get_value)();

void ofb_cbs_octet(const ORTESendInfo *info,void *vinstance, void *sendCallBackParam);
void ofb_cbr_octet(const ORTERecvInfo *info,void *vinstance, void *recvCallBackParam);
void ofb_cbs_short(const ORTESendInfo *info,void *vinstance, void *sendCallBackParam);
void ofb_cbr_short(const ORTERecvInfo *info,void *vinstance, void *recvCallBackParam);
void ofb_cbs_long(const ORTESendInfo *info,void *vinstance, void *sendCallBackParam);
void ofb_cbr_long(const ORTERecvInfo *info,void *vinstance, void *recvCallBackParam);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
