bin_PROGRAMS = ortespy

ortespy_SOURCES = ortespy.c
if CONFIG_ORTE_MINGW
  win_libs= $(top_srcdir)/orte/contrib/win_pthread/libpthreadGC.a
endif
ortespy_LDADD   = $(top_builddir)/orte/liborte/liborte.la $(win_libs)
ortespy_LDFLAGS = -static


