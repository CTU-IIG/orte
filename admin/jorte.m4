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

AC_DEFUN([AM_JORTE],
[AC_MSG_CHECKING([whether to compile jorte])

  AC_ARG_WITH([java],
	[AC_HELP_STRING([--with-java=DIR],
	[path to JAVA home directory for jorte compilation])],
	[JAVA_HOME_DIR="${withval}"],
	[JAVA_HOME_DIR="no"])
#	[JAVA_HOME_DIR="${JAVA_HOME}"])

  USE_JORTE=no
  if test "${JAVA_HOME_DIR}" != "no" ; then
      if [[ -e "${JAVA_HOME_DIR}/include/jni.h" ]] ; then 
    	    USE_JORTE=yes
            JNI_INCLUDES="-I$JAVA_HOME_DIR/include" 	 
            dnl Strange thing, jni.h requires jni_md.h which is under genunix here.. 	 
            dnl and under linux here.. 	 
            test -d "$JAVA_HOME_DIR/include/linux" && 
		    JNI_INCLUDES="$JNI_INCLUDES -I$JAVA_HOME_DIR/include/linux" 	 
            test -d "$JAVA_HOME_DIR/include/genunix" && 
		    JNI_INCLUDES="$JNI_INCLUDES -I$JAVA_HOME_DIR/include/genunix"
            test -d "$JAVA_HOME_DIR/include/win32" && 
		    JNI_INCLUDES="$JNI_INCLUDES -I$JAVA_HOME_DIR/include/win32"
	    CFLAGS="$CFLAGS $JNI_INCLUDES"
            AC_DEFINE([CONFIG_JORTE],[1],[Define to enable JORTE's compilation])
      fi
  fi

  AC_MSG_RESULT([$USE_JORTE])

  AM_CONDITIONAL(CONFIG_JORTE, [test $USE_JORTE = yes])
  AC_SUBST(CONFIG_JORTE)	

]
)

AU_DEFUN([jm_JORTE], [AM_JORTE])
