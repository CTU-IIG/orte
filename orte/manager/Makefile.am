if CONFIG_ORTE_RT
  module_PROGRAMS = rt_ORTEManager.ko 
  rt_ORTEManager_ko_CFLAGS = $(ORTE_CFLAGS) $(LINUX_CFLAGS) $(RTAI_CFLAGS) $(RTLINUX_CFLAGS)
  rt_ORTEManager_ko_SOURCES = ORTEManager.c  
  rt_ORTEManager_ko_LINK = $(top_builddir)/modtool --link -o $@
else
bin_PROGRAMS = ORTEManager
ORTEManager_SOURCES = ORTEManager.c
if CONFIG_ORTE_WIN
  win_libs= $(top_srcdir)/orte/contrib/win_pthread/libpthreadGC.a 
endif
ORTEManager_LDADD   = ../liborte/liborte.a $(win_libs)
endif
