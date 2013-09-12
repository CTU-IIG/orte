#ifndef _UL_HTIMDEFS_H
#define _UL_HTIMDEFS_H

#include "ul_utdefs.h"
#ifdef _WIN64
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Select heap-tree as base structure to implement
 * hierarchic timer - not recomended
 */
//#define UL_HTIMER_WITH_HPTREE

/* If defined, standard timer type
 * is built
 */
//#define UL_HTIMER_WITH_STD_TYPE

/* If defined, standard microsecond resolution 
 * data types are selected for timer implementation
 */
//#define UL_HTIMER_WITH_MSTIME

/* If defined, NTPTimer type
 * is built
 */
#define UL_HTIMER_WITH_NTPTIME 

#ifdef UL_HTIMER_WITH_MSTIME
typedef unsigned long ul_mstime_t;
typedef signed long ul_msdiff_t;

typedef ul_mstime_t ul_htim_time_t;
typedef ul_msdiff_t ul_htim_diff_t;

#define UL_HTIM_RESOLUTION 1000

static inline int
ul_htimer_cmp_fnc(const ul_htim_time_t *a, const ul_htim_time_t *b)
{
  ul_htim_diff_t d=(ul_htim_diff_t)(*a-*b);
  if (d>0) return 1;
  if (d<0) return -1;
  return 0;
}
#endif /*UL_HTIMER_WITH_MSTIME*/

#ifdef UL_HTIMER_WITH_NTPTIME
#include "orte/typedefs_defines_rtps.h"
#include "orte/defines_api.h"
typedef NtpTime ul_htim_time_t;
typedef NtpTime ul_htim_diff_t;

#define UL_HTIM_RESOLUTION 0x100000000

static inline int
ul_htimer_cmp_fnc(const ul_htim_time_t *a, const ul_htim_time_t *b){
   return NtpTimeCmp(*a,*b);
}
#endif /*UL_HTIMER_WITH_NTPTIME*/ 

/*===========================================================*/
/*  Configuration of types for standard timer (ul_htimer) */

/* Additional, user defined fields for ul_htimer_t structure */
#define UL_HTIMER_USER_FIELDS 

#ifdef _WIN64
typedef uintptr_t ul_htimer_fnc_data_t;
#else
typedef unsigned long ul_htimer_fnc_data_t;
#endif
/*typedef void *ul_htimer_fnc_data_t;*/

typedef void (ul_htimer_fnc_t)(ul_htimer_fnc_data_t data) UL_ATTR_REENTRANT;

/* The wrapper for ul_htimer_run_expired */
#define UL_HTIMER_FNC_CALL(queue, timer, pact_time) \
    do{ timer->function(timer->data); } while(0)


#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif /* _UL_HTIMDEFS_H */
