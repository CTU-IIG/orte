if CONFIG_ORTE_RT
  module_PROGRAMS = rt_ortemanager.ko 
  rt_ortemanager_ko_CFLAGS = $(ORTE_CFLAGS) 
  rt_ortemanager_ko_SOURCES = ortemanager.c  
  rt_ortemanager_ko_LINK = $(top_builddir)/modtool --link -o $@
else
bin_PROGRAMS = ortemanager
ortemanager_SOURCES = ortemanager.c
if CONFIG_ORTE_WIN
  ortemanager_SOURCES += service.c
endif
if CONFIG_ORTE_MINGW
  win_libs= $(top_srcdir)/orte/contrib/win_pthread/libpthreadGC.a 
endif
ortemanager_LDADD   = ../liborte/liborte.a $(win_libs)
endif
