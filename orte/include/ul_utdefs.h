/*******************************************************************
  uLan Utilities Library - C library of basic reusable constructions

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

#if !defined(UL_BUILD_BUG_ON_MSG_LINE) && defined(__OPTIMIZE__) && \
  ((__GNUC__ * 1000 + __GNUC_MINOR__) >= 4004)
#define UL_BUILD_BUG_ON_MSG_LINE_EXP1(condition, msg, line) \
({ \
  if (!!(condition)) { \
    void compile_time_bug_on_line_ ## line (void) __attribute__((error(msg))); \
    compile_time_bug_on_line_ ## line (); \
  } \
})
#define UL_BUILD_BUG_ON_MSG_LINE(condition, msg, line) \
  UL_BUILD_BUG_ON_MSG_LINE_EXP1(condition, msg, line)
#endif /*UL_BUILD_BUG_ON_MSG for GCC*/

#ifndef UL_BUILD_BUG_ON_MSG_LINE
#define UL_BUILD_BUG_ON_MSG_LINE(condition, msg, line) \
  ((void)sizeof(char[1 - 2*!!(condition)]))
#endif /*UL_BUILD_BUG_ON_MSG*/

#ifndef UL_BUILD_BUG_ON_MSG
#define UL_BUILD_BUG_ON_MSG(condition, msg) \
  UL_BUILD_BUG_ON_MSG_LINE(condition, msg, __LINE__)
#endif /*UL_BUILD_BUG_ON_MSG*/

#ifndef UL_BUILD_BUG_ON
#define UL_BUILD_BUG_ON(condition) \
  UL_BUILD_BUG_ON_MSG(condition, "Build time check " #condition " failed")
#endif /*UL_BUILD_BUG_ON*/

#if !defined(UL_OFFSETOF) && defined(__GNUC__) && __GNUC__ >= 4
#define UL_OFFSETOF(_type, _member) __builtin_offsetof(_type, _member)
#endif /*UL_OFFSETOF*/

#ifndef UL_OFFSETOF
/* offset of structure field */
#define UL_OFFSETOF(_type,_member) \
                ((size_t)&(((_type*)0)->_member))
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

#ifndef UL_NOPSTATEMENT
#define UL_NOPSTATEMENT do { } while(0)
#endif

#ifndef ul_cyclic_gt
#define ul_cyclic_gt(x,y) \
	((sizeof(x)>=sizeof(long long))&&(sizeof(y)>=sizeof(long long))? \
		(long long)((unsigned long long)(x)-(unsigned long long)(y))>0: \
	 (sizeof(x)>=sizeof(long))&&(sizeof(y)>=sizeof(long))? \
	 	(long)((unsigned long)(x)-(unsigned long)(y))>0: \
	 (sizeof(x)>=sizeof(int))&&(sizeof(y)>=sizeof(int))? \
	 	(int)((unsigned int)(x)-(unsigned int)(y))>0: \
	 (sizeof(x)>=sizeof(short))&&(sizeof(y)>=sizeof(short))? \
	 	(short)((unsigned short)(x)-(unsigned short)(y))>0: \
	 (signed char)((unsigned char)(x)-(unsigned char)(y))>0 \
	)
#endif /*ul_cyclic_gt*/

#ifndef ul_cyclic_ge
#define ul_cyclic_ge(x,y) \
	((sizeof(x)>=sizeof(long long))&&(sizeof(y)>=sizeof(long long))? \
		(long long)((unsigned long long)(x)-(unsigned long long)(y))>=0: \
	 (sizeof(x)>=sizeof(long))&&(sizeof(y)>=sizeof(long))? \
	 	(long)((unsigned long)(x)-(unsigned long)(y))>=0: \
	 (sizeof(x)>=sizeof(int))&&(sizeof(y)>=sizeof(int))? \
	 	(int)((unsigned int)(x)-(unsigned int)(y))>=0: \
	 (sizeof(x)>=sizeof(short))&&(sizeof(y)>=sizeof(short))? \
	 	(short)((unsigned short)(x)-(unsigned short)(y))>=0: \
	 (signed char)((unsigned char)(x)-(unsigned char)(y))>=0 \
	)
#endif /*ul_cyclic_ge*/

/* GNUC neat features */

#ifdef	__GNUC__
#ifndef UL_ATTR_UNUSED
#define UL_ATTR_PRINTF( format_idx, arg_idx )	\
  __attribute__((format (printf, format_idx, arg_idx)))
#define UL_ATTR_SCANF( format_idx, arg_idx )	\
  __attribute__((format (scanf, format_idx, arg_idx)))
#define UL_ATTR_FORMAT( arg_idx )		\
  __attribute__((format_arg (arg_idx)))
#define UL_ATTR_NORETURN			\
  __attribute__((noreturn))
#define UL_ATTR_CONST				\
  __attribute__((const))
#define	UL_ATTR_UNUSED				\
  __attribute__((unused))
#define UL_ATTR_CONSTRUCTOR			\
  __attribute__((constructor))
#define	UL_ATTR_DESCRUCTOR			\
  __attribute__((destructor))
#define	UL_ATTR_ALWAYS_INLINE			\
  __attribute__((always_inline))
#define	UL_ATTR_WEAK				\
  __attribute__((weak))
#endif  /*UL_ATTR_UNUSED*/
#else	/* !__GNUC__ */
#ifndef UL_ATTR_UNUSED
#define UL_ATTR_PRINTF( format_idx, arg_idx )
#define UL_ATTR_SCANF( format_idx, arg_idx )
#define UL_ATTR_FORMAT( arg_idx )
#define UL_ATTR_NORETURN
#define UL_ATTR_CONST
#define UL_ATTR_UNUSED
#define	UL_ATTR_CONSTRUCTOR
#define	UL_ATTR_DESCRUCTOR
#define	UL_ATTR_ALWAYS_INLINE
#define UL_ATTR_WEAK
#endif  /*UL_ATTR_UNUSED*/
#endif	/* !__GNUC__ */

#ifndef UL_ATTR_REENTRANT
#if (!defined(SDCC) && !defined(__SDCC)) || defined(SDCC_z80) || defined(__SDCC_z80)
  #define UL_ATTR_REENTRANT
#else
  #define UL_ATTR_REENTRANT __reentrant
#endif
#endif /*UL_ATTR_REENTRANT*/

/* The cast idea based on libHX by Jan Engelhardt */
#define UL_TYPEOF_REFX(ref_asterisks, ptr_type) \
  typeof(ref_asterisks(union { int z; typeof(ptr_type) x; }){0}.x)

#ifdef	__GNUC__
#define UL_CAST_UNQX(ref_asterisks, new_type, expr) ({ \
  UL_BUILD_BUG_ON_MSG(!__builtin_types_compatible_p\
    (UL_TYPEOF_REFX(ref_asterisks, expr), \
     UL_TYPEOF_REFX(ref_asterisks,new_type)), \
       "Qualifiers stripping cast to incompatible type"); \
  (new_type)(expr); \
})
#else /*__GNUC__*/
#define UL_CAST_UNQX(ref_asterisks, new_type, expr) ((new_type)(expr))
#endif /*__GNUC__*/

#define UL_CAST_UNQ1(new_type, expr) \
  UL_CAST_UNQX(*, new_type, expr)

#define UL_CAST_UNQ2(new_type, expr) \
  UL_CAST_UNQX(**, new_type, expr)

#define UL_CAST_UNQ3(new_type, expr) \
  UL_CAST_UNQX(**, new_type, expr)

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif /* _UL_UTDEFS_H */
