# Add --enable-orte-idl option to configure.
# From Petr Smolik

# Copyright (C) 1996, 1998, 2000, 2001, 2002, 2003, 2004
# Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# serial 3

AC_DEFUN([AM_ORTE_IDL],
[AC_MSG_CHECKING([whether to enable orte IDL compiler])
  dnl orte-idl is disabled by default
  AC_ARG_ENABLE(orte-idl,
[  --enable-orte-idl  enable to compile orte IDL compiler],
      USE_ORTE_IDL=$enableval,
      USE_ORTE_IDL=no)
  AC_MSG_RESULT([$USE_ORTE_IDL])

  if test ${USE_ORTE_IDL} == "yes" ; then
	PKG_CHECK_MODULES(LIBIDL, libIDL-2.0)
        CFLAGS="$CFLAGS $LIBIDL_CFLAGS"
	LIBS="$LIBS $LIBIDL_LIBS"
	#popt
	AC_CHECK_LIB([popt], [poptSetOtherOptionHelp])
  fi

  AM_CONDITIONAL(CONFIG_ORTE_IDL, [test $USE_ORTE_IDL = yes])
  AC_SUBST(CONFIG_ORTE_IDL)	

]
)

AU_DEFUN([jm_ORTE_IDL], [AM_ORTE_IDL])
