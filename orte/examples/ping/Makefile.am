bin_PROGRAMS = orteping

orteping_SOURCES = orteping.c
if CONFIG_ORTE_MINGW
  win_libs= $(top_srcdir)/orte/contrib/win_pthread/libpthreadGC.a 
endif
orteping_LDADD   = ../../liborte/liborte.a $(win_libs)

