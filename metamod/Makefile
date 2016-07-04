# vi: set ts=4 sw=4 :
# vim: set tw=75 :

# MetaMod makefile
# Copyright (c) 2001-2003 Will Day <willday@hpgx.net>
#
# based on the Valve SDK 2.1 Makefile as well as the Makefile
# in adminmod by Alfred Reynolds.
#
# From SDK 2.1 dlls/Makefile:
#! Half-Life StandardSDK 2.0 mp_i386.so Makefile for i386 Linux
#! April 2000 by Leon Hartwig (jehannum@planethalflife.com)


# NOTE: This is a generic Makefile for metamod and the bundled plugins, and
# is symlinked into each subdir.  Per-module config statements are in
# Config.mak in each subdir.

ifeq "$(shell uname | cut -d _ -f1)" "CYGWIN"
	HOST=cygwin
endif

ifdef COMSPEC
	ifeq "$(HOST)" "cygwin"
		ifeq "$(TARGET)" "win32"
			OS=windows
		else
			OS=linux
		endif
	else
		OS=windows
	endif
else
	OS=linux
endif

#############################################################################
# CONFIGURATION
#############################################################################

# TARGET amd64 disabled since Valve has dropped support for x86-64 server
#ifeq "$(TARGET)" "amd64"
#	TARGETTYPE = amd64
#else
	TARGETTYPE = i386
#endif

# set paths for your environment
ifeq "$(OS)" "linux"
	INST_DIR=$(HOME)/half-life/cstrike/dlls
	TEST_DIR=$(HOME)/test/tfc/dlls
	TST_DIR=$(HOME)/tmp
else	## windows
	INST_DIR=/hlserver/tfc/dlls
	TEST_DIR=/hlserver/tfc/dlls
endif

DLLS_DIR=../dlls
SDKSRC=../hlsdk
METADIR=../metamod

COMPILE_TZ=EET
## Developer overrides
ifeq "$(USER)" "jussi"
	COMPILE_TZ=EET
endif


#############################################################################
# OS DEPENDENCIES
#############################################################################

ifeq "$(OS)" "linux"
	INSTALL=install -m 644
	LD_WINDLL= i686-w64-mingw32-dllwrap
	OSTARGET=linux
	LIBFILE=$(LIBFILE_LINUX)
	TARGET_FILE=$(TARGET_LINUX)
else	## windows
	INSTALL=cp
	LD_WINDLL= i686-w64-mingw32-dllwrap
	OSTARGET=win32
	LIBFILE=$(LIBFILE_WIN)
	TARGET_FILE=$(TARGET_WIN)
endif

CC_WIN=i686-w64-mingw32-gcc
RES_WIN=i686-w64-mingw32-windres

ifeq "$(HOST)" "cygwin"
	CC_WIN += -mno-cygwin
	EXTRA_LINK += -L/lib/w32api
endif

OBJDIR_LINUX_OPT=opt.linux_$(TARGETTYPE)
OBJDIR_LINUX_DBG=debug.linux_$(TARGETTYPE)
OBJDIR_WIN_OPT=opt.win32
OBJDIR_WIN_DBG=debug.win32


#############################################################################
# COMPILE OPTIONS - ARCHITECTURE AND OPTIMIZATIONS
#############################################################################

ifeq "$(OS)" "linux"
	GCCMAJ = $(shell $(CC) -dumpversion | sed -e 's/\.[0-9][0-9]*//')
	GCCMIN = $(shell $(CC) -dumpversion | sed -e 's/[0-9]\.//;s/\.[0-9]//')
else
	GCCMAJ = $(shell $(CC_WIN) -dumpversion | sed -e 's/\.[0-9][0-9]*//')
	GCCMIN = $(shell $(CC_WIN) -dumpversion | sed -e 's/[0-9]\.//;s/\.[0-9]//')
endif

ifeq "$(HOST)" "cygwin"
	ifeq "$(TARGETTYPE)" "amd64"
		CC=gcc-linux-x86_64
	else
		CC=gcc-linux
	endif
else
	ifeq "$(TARGETTYPE)" "amd64"
		CC=gcc -m64
	else
		CC=gcc -m32
	endif
endif

MCPU=-mcpu

ifeq "$(GCCMAJ)" "3"
	ifeq "$(GCCMIN)" "4"
		MCPU=-mtune
	endif
endif
ifeq "$(GCCMAJ)" "4"
	MCPU=-mtune
endif

ifeq "$(OS)" "linux"
	CC_DEP=$(CC)
else
	CC_DEP=$(CC_WIN)
endif

# Note! About gcc optimization levels.
#  There is four optimization levels:
#	-O0	No optimizations.
#	-O1	Optimize for smaller size.
#	-O2	Optimize for speed without increasing size (alot).
#	-O3	Optimize for speed, can result much greater filesize.
#  Levels higher -O3 (-O6 for example) is threaded as -O3.
#  See differences at "http://gcc.gnu.org/" (look for link to 'gcc manual').

# original safe optimization, from valve Makefile
#CCOPT = -O2 -ffast-math -funroll-loops \
#	-fomit-frame-pointer -fexpensive-optimizations -malign-loops=2 \
#	-malign-jumps=2 -malign-functions=2

# safe optimization, adapted from adminmod Makefile
#CCOPT = -m486 -O6 -ffast-math -funroll-loops \
# -fexpensive-optimizations -malign-loops=2  -malign-jumps=2 \
# -malign-functions=2 -Wall

# full optimization, adapted from adminmod Makefile
# "WONT WORK WITH omit-frame-pointer"?
# - disable (unneeded) C++ exceptions and rtti code to save some space ?

CCOPT = $(CCO) $(CCOPT_ARCH) -fno-exceptions -fno-rtti

# optimization level; overridden for certain problematic files
CCO = -O2 -fomit-frame-pointer -ffast-math

# architecture tuning by target type
ifeq "$(TARGETTYPE)" "amd64"
	CCOPT_ARCH =
else
	CCOPT_ARCH = -march=i686 $(MCPU)=generic
endif

# debugging; halt on warnings
CCDEBUG+= -ggdb3


#############################################################################
# COMPILE SETUP
#############################################################################

SRCDIR=.
INCLUDEDIRS+=-I$(SRCDIR) -I$(METADIR) -I$(SDKSRC)/engine -I$(SDKSRC)/common -I$(SDKSRC)/pm_shared -I$(SDKSRC)/dlls -I$(SDKSRC)
FILES_ALL = *.cpp *.h [A-Z]* *.rc

CFLAGS=-Wall -Wno-unknown-pragmas -Wno-attributes

CFLAGS+=

#CFLAGS += -DTEST

ifeq "$(OPT)" "opt-fast"
	ODEF = -DOPT_TYPE="\"optimized+meta_debug-disabled\""
	CFLAGS := $(CCOPT) $(CFLAGS) $(ODEF) -D__BUILD_FAST_METAMOD__
	OBJDIR_LINUX = $(OBJDIR_LINUX_OPT)
	OBJDIR_WIN = $(OBJDIR_WIN_OPT)
else	#other
ifeq "$(OPT)" "opt"
	ODEF = -DOPT_TYPE="\"optimized\""
	CFLAGS := $(CCOPT) $(CFLAGS) $(ODEF)
	OBJDIR_LINUX = $(OBJDIR_LINUX_OPT)
	OBJDIR_WIN = $(OBJDIR_WIN_OPT)
else	# debug
	ODEF = -DOPT_TYPE="\"debugging\""
	CFLAGS := $(CCDEBUG) $(CFLAGS) $(ODEF)
	OBJDIR_LINUX = $(OBJDIR_LINUX_DBG)
	OBJDIR_WIN = $(OBJDIR_WIN_DBG)
	DLLS_DIR := $(DLLS_DIR)/debug
endif
endif

ifeq "$(OS)" "linux"
	OBJDIR = $(OBJDIR_LINUX)
else
	OBJDIR = $(OBJDIR_WIN)
endif

include Config.mak
# any local CFLAGS from Config.mak
CFLAGS += $(EXTRA_CFLAGS)

# provide timezone info
$(OBJDIR_LINUX)/vdate.o $(OBJDIR_WIN)/vdate.o: CFLAGS += -DCOMPILE_TZ=\"$(COMPILE_TZ)\"

# ignore complaints from SDK files like cbase.h
#$(OBJDIR_LINUX)/sdk_util.o $(OBJDIR_WIN)/sdk_util.o: CFLAGS += -Wno-unused

# ignore complaints from STL headers 
STLOBJ = $(STLFILES:%.cpp=$(OBJDIR_LINUX)/%.o) 
STLOBJ += $(STLFILES:%.cpp=$(OBJDIR_WIN)/%.o) 
#$(OBJDIR_LINUX)/mreg.o $(OBJDIR_WIN)/mreg.o: CFLAGS += -Wno-effc++
$(STLOBJ): FILTER= 2>&1 | ../tools/stlfilter
$(STLOBJ): CFLAGS += -Wno-error

# these files seem to create "Internal compiler error" errors under mingw
# when using -O6
#$(OBJDIR_WIN)/engine_api.o: CCO = -O5
#$(OBJDIR_WIN)/dllapi_api.o: CCO = -O5

#############################################################################
# BUILDING LINUX SO
#############################################################################

# linux .so compile commands
DO_CC_LINUX=$(CC) $(CFLAGS) -fPIC $(INCLUDEDIRS) -o $@ -c $< $(FILTER)
LINK_LINUX=$(CC) $(CFLAGS) -shared -ldl -lm -static-libgcc $(EXTRA_LINK) $(OBJ_LINUX) -o $@

# sort by date
#SRCFILES := $(shell ls -t $(SRCFILES))

# linux object files
OBJ_LINUX := $(SRCFILES:%.cpp=$(OBJDIR_LINUX)/%.o)

# compiling linux object files
$(OBJDIR_LINUX)/%.o: $(SRCDIR)/%.cpp
	$(DO_CC_LINUX)

# linux .so target file
LIBFILE_LINUX = $(MODNAME).so
TARGET_LINUX = $(OBJDIR_LINUX)/$(LIBFILE_LINUX)


#############################################################################
# BUILDING WINDOWS DLL
#############################################################################

# windows .dll compile commands
DO_CC_WIN=$(CC_WIN) $(CFLAGS) $(INCLUDEDIRS) -o $@ -c $<
DO_RES_WIN=$(RES_WIN) '$(ODEF)' --include-dir . --include-dir ../metamod -i $< -O coff -o $@
#LINK_WIN=$(LD_WINDLL) -k -mwindows --add-stdcall-alias --def metamod.def -o $@ $(OBJ_WIN)
#LINK_WIN=$(LD_WINDLL) -A -k -mwindows --export-all-symbols -o $@ $(OBJ_WIN)
#LINK_WIN=$(LD_WINDLL) -mwindows --add-stdcall-alias $(OBJ_WIN) $(RES_OBJ_WIN) -lstdc++ -s -o $@
LINK_WIN=$(CC_WIN) $(CFLAGS) -mdll -Xlinker --add-stdcall-alias $(EXTRA_LINK) $(OBJ_WIN) $(RES_OBJ_WIN) -s -o $@

# windows object files
OBJ_WIN := $(SRCFILES:%.cpp=$(OBJDIR_WIN)/%.o)
RES_OBJ_WIN := $(RESFILE:%.rc=$(OBJDIR_WIN)/%.o)

# compiling windows object files
$(OBJDIR_WIN)/%.o: $(SRCDIR)/%.cpp
	$(DO_CC_WIN)

# compiling windows resource file
$(OBJDIR_WIN)/%.o: $(SRCDIR)/%.rc $(INFOFILES)
	$(DO_RES_WIN)

# windows .dll target file
LIBFILE_WIN = $(MODNAME).dll
TARGET_WIN = $(OBJDIR_WIN)/$(LIBFILE_WIN)


#############################################################################
# OVERRIDES
#############################################################################

ifeq "$(PLATFORM)" "linux-only"
	LIBFILE_WIN =
	TARGET_WIN =
endif
ifeq "$(PLATFORM)" "win32-only"
	LIBFILE_LINUX =
	TARGET_LINUX =
	OSTARGET=win32
	LIBFILE=$(LIBFILE_WIN)
	TARGET_FILE=$(TARGET_WIN)
endif


#############################################################################
# BUILD RULES
#############################################################################

default: $(TARGET_FILE)

all: do_dll_linux do_dll_win32

opt:
	$(MAKE) default OPT=opt

linux: do_dll_linux
win32: do_dll_win32

linux_opt: 
	$(MAKE) linux OPT=opt
win32_opt: 
	$(MAKE) win32 OPT=opt

$(TARGET_LINUX): msgs/debug msgs/warning msgs/log msgs/error $(OBJDIR_LINUX) $(OBJ_LINUX)
	$(LINK_LINUX)

# for plugins, recompile meta_api.cpp if info_name.h changed
$(OBJDIR_LINUX)/meta_api.o $(OBJDIR_WIN)/meta_api.o: info_name.h

$(TARGET_WIN): msgs/debug msgs/warning msgs/log msgs/error $(OBJDIR_WIN) $(OBJ_WIN) $(RES_OBJ_WIN)
	$(LINK_WIN)

$(OBJDIR_LINUX) $(OBJDIR_WIN) msgs:
	mkdir $@

# make sure to recompile vdate.c for each link
$(OBJDIR_LINUX)/vdate.o $(OBJDIR_WIN)/vdate.o : $(SRCFILES) *.h

domsgs: msgs/debug msgs/log msgs/error msgs/warning

msgs/debug: $(SRCFILES) *.h msgs
	egrep "DEBUG\([0-9]" $(SRCFILES) *.h | sed "s/:[ 	]*/	/" | sort -k2,2 > $@

msgs/log: $(SRCFILES) *.h msgs
	egrep "META_LOG\(" $(SRCFILES) *.h | sed "s/:[ 	]*/	/" | sort > $@

msgs/error: $(SRCFILES) *.h msgs
	egrep "META_ERROR\(" $(SRCFILES) *.h | sed "s/:[ 	]*/	/" | sort > $@

msgs/warning: $(SRCFILES) *.h msgs
	egrep "META_WARNING\(" $(SRCFILES) *.h | sed "s/:[ 	]*/	/" | sort > $@

tags:	.tags .htags
ctags:	.tags
htags:	.htags

.tags: $(SRCFILES) *.h
	-ctags -a $?

.htags: *.h
	-htags $?

retags:
	-rm -f .tags .htags
	ctags -f .tags `find $(SDKSRC) -name '*.h'`
	ctags -f .tags -a `find $(METADIR) -name old -prune -o -name '*.h' -print`
	ctags -f .tags -a $(SRCFILES)
	htags -R $(SDKSRC)
	htags `find $(METADIR) -name old -prune -o -name '*.h' -print`
	htags *.h

clean: clean_$(OSTARGET)

clean_linux:
	test -n "$(OBJDIR_LINUX)"
	-rm -f $(OBJDIR_LINUX)/*

clean_win32:
	test -n "$(OBJDIR_WIN)"
	-rm -f $(OBJDIR_WIN)/*

cleanall_linux:
	$(MAKE) clean_linux
	$(MAKE) clean_linux OPT=opt
	$(MAKE) clean_linux TARGET=amd64
	$(MAKE) clean_linux TARGET=amd64 OPT=opt

cleanall_win32:
	$(MAKE) clean_win32
	$(MAKE) clean_win32 OPT=opt

cleanall: cleanall_linux cleanall_win32

dll_linux dll_win32: 
	$(MAKE) do_$@
	$(MAKE) do_$@ OPT=opt

do_dll_linux: $(DLLS_DIR) $(DLLS_DIR)/$(LIBFILE_LINUX)
do_dll_win32: $(DLLS_DIR) $(DLLS_DIR)/$(LIBFILE_WIN)

$(DLLS_DIR):
	mkdir $(DLLS_DIR)

$(DLLS_DIR)/$(LIBFILE_LINUX): $(TARGET_LINUX)
	$(INSTALL) $+ $@

$(DLLS_DIR)/$(LIBFILE_WIN): $(TARGET_WIN)
	$(INSTALL) $+ $@

dlls: dll_linux dll_win32
rmdlls:
	-rm -f ../dlls/*.* ../dlls/debug/*.*

spotless: cleanall
	-rmdir $(OBJDIR_LINUX) $(OBJDIR_WIN)
#	-rm -f .snap $(GENERATED)

distclean:	spotless

install: $(INST_DIR)/$(LIBFILE)
test: $(TEST_DIR)/$(LIBFILE)
test_opt:
	$(MAKE) test OPT=opt

$(INST_DIR)/$(LIBFILE) $(TEST_DIR)/$(LIBFILE): $(TARGET_FILE)
	$(INSTALL) $< $@

tst: $(TST_DIR)/$(LIBFILE_WIN)

$(TST_DIR)/$(LIBFILE_WIN): $(TARGET_WIN)
	cp $< $@

snap:	.snap

.snap: $(FILES_ALL)
	mkdir -p snapshots
	tar zcvf snapshots/`date '+%m%d-%H%M'`.tgz $(FILES_ALL)
	touch .snap
	
depend: $(OBJDIR)/Rules.depend

$(OBJDIR)/Rules.depend: Makefile $(SRCFILES) $(OBJDIR)
	$(CC_DEP) -MM $(INCLUDEDIRS) $(SRCFILES) | sed "s;\(^[^ 	]*\):\(.*\);$(OBJDIR)/\1: Makefile Config.mak \2;" > $@

include $(OBJDIR)/Rules.depend
