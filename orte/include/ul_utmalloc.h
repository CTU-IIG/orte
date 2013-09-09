#ifndef _UL_UTMALLOC_H
#define _UL_UTMALLOC_H

#if !defined(__RTL__)&&!defined(__KERNEL__)

#include <malloc.h>

#else /*__RTL__ or __KERNEL__*/

#ifdef UL_WITH_RTL_MALLOC
#include <rtl_malloc.h>
#define malloc    rt_malloc
#define free      rt_free
#define realloc   rt_realloc
#endif /*UL_WITH_RTL_MALLOC*/

#endif /*__RTL__ or __KERNEL__*/

#endif /*_UL_UTMALLOC_H*/
