/*******************************************************************
  uLan Communication - C interface library

  ul_utdefs.h	- common defines used in uLan utilities library

 *******************************************************************/


#ifndef _UL_UTDEFS_H
#define _UL_UTDEFS_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)&&defined(_MSC_VER)&&!defined(inline)
#define inline _inline
#endif

#ifndef UL_OFFSETOF
/* offset of structure field */
#define UL_OFFSETOF(_type,_member) \
                ((int)&(((_type*)0)->_member))
#endif /*UL_OFFSET*/

#ifndef UL_CONTAINEROF
#ifdef  __GNUC__
#define UL_CONTAINEROF(_ptr, _type, _member) ({ \
        const typeof( ((_type *)0)->_member ) *__mptr = (_ptr); \
        (_type *)( (char *)__mptr - UL_OFFSETOF(_type,_member) );})
#else /*!__GNUC__*/
#define UL_CONTAINEROF(_ptr, _type, _member) \
        ((_type *)( (char *)_ptr - UL_OFFSETOF(_type,_member)))
#endif /*__GNUC__*/
#endif /*UL_CONTAINEROF*/



#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif /* _UL_UTDEFS_H */
