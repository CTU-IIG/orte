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

if CONFIG_ORTE_RT
module_PROGRAMS = pthread_rwlock.o
pthread_rwlock_o_CFLAGS = $(ORTE_CFLAGS) $(LINUX_CFLAGS) $(RTAI_CFLAGS) $(RTLINUX_CFLAGS)
pthread_rwlock_o_SOURCES = $(PTHREAD_RWLOCK_SOURCES)
pthread_rwlock_o_LINK = $(top_builddir)/modtool --link -o $@
endif
