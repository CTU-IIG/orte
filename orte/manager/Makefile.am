if CONFIG_ORTE_RT
  module_PROGRAMS = ortemanager_rt.ko 
  ortemanager_rt_ko_CFLAGS = $(ORTE_CFLAGS) 
  ortemanager_rt_ko_SOURCES = ortemanager.c  
  ortemanager_rt_ko_LINK = $(top_builddir)/modtool --link -o $@
else
bin_PROGRAMS = ortemanager
ortemanager_SOURCES = ortemanager.c
if CONFIG_ORTE_WIN
  ortemanager_SOURCES += service.c
endif
ortemanager_LDADD   = $(top_builddir)/orte/liborte/liborte.la 
ortemanager_LDFLAGS = -static
endif
