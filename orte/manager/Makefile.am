bin_PROGRAMS = ORTEManager

ORTEManager_SOURCES = ORTEManager.c
if CONFIG_ORTE_WIN
  win_libs= $(top_srcdir)/orte/contrib/win_pthread/libpthreadGC.a 
endif
ORTEManager_LDADD   = ../liborte/liborte.a $(win_libs)

