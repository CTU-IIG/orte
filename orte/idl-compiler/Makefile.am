orte_idl_SOURCES =			\
	orte-idl-main.c		        \
	orte-idl-driver.c		\
	orte-idl-utils.c		\
	orte-idl3-types.h		\
	orte-idl-c-backend.h		\
	orte-idl-c-backend.c		\
	orte-idl-c-headers.c		\
	orte-idl-c-impls.c		\
	orte-idl-c-utils.c		\
	orte-idl2.h

bin_PROGRAMS = orte-idl

EXTRA_DIST = Makefile Makefile.omk 

dist-hook:
	cp -f  $(srcdir)/Makefile $(distdir)/Makefile
