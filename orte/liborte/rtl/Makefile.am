#
#  Makefile for the Ocera Real-Time Ethernet
#
#  $Id: Makefile.am,v 1.0 2003/08/04
#

PTHREAD_RWLOCK_SOURCES = \
ptw32_rwlock_check_need_init.c \
ptw32_rwlock_cancelwrwait.c \
pthread_rwlock_init.c \
pthread_rwlock_destroy.c \
pthread_rwlockattr_init.c \
pthread_rwlockattr_destroy.c \
pthread_rwlockattr_getpshared.c \
pthread_rwlockattr_setpshared.c \
pthread_rwlock_rdlock.c \
pthread_rwlock_timedrdlock.c \
pthread_rwlock_wrlock.c \
pthread_rwlock_timedwrlock.c \
pthread_rwlock_unlock.c \
pthread_rwlock_tryrdlock.c \
pthread_rwlock_trywrlock.c

if USE_RTLINUX
module_PROGRAMS = rtl_pthread_rwlock.ko
#rtl_pthread_rwlock_ko_CFLAGS = $(ORTE_CFLAGS) 
rtl_pthread_rwlock_ko_SOURCES = $(PTHREAD_RWLOCK_SOURCES)
rtl_pthread_rwlock_ko_LINK = $(top_builddir)/modtool --link -o $@
endif
