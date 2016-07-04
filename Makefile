COMPILER = /opt/intel/bin/icpc
SDKSRC = sdk
DIRSRC = src
NAME = metamod

SRCFILES = src/api_hook.cpp src/api_info.cpp src/commands_meta.cpp src/conf_meta.cpp \
	src/dllapi.cpp src/engine_api.cpp src/engineinfo.cpp src/game_support.cpp \
	src/game_autodetect.cpp src/h_export.cpp src/linkplug.cpp \
	src/log_meta.cpp src/meta_eiface.cpp src/metamod.cpp src/mlist.cpp src/mplayer.cpp \
	src/mplugin.cpp src/mreg.cpp src/mutil.cpp src/osdep.cpp src/osdep_p.cpp src/reg_support.cpp src/sdk_util.cpp \
	src/support_meta.cpp src/vdate.cpp src/osdep_linkent_linux.cpp src/osdep_detect_gamedll_linux.cpp

OBJECTS = $(SRCFILES) lib/libirc.a lib/linux32/libaelf32.a

LINK = -lm -ldl -static-intel -static-libgcc -no-intel-extensions

OPT_FLAGS = -O3 -msse3 -no-prec-div -fp-model fast=2 -funroll-loops -fomit-frame-pointer -fno-stack-protector \
-D__METAMOD_BUILD__ -D__BUILD_FAST_METAMOD__ -D__INTERNALS_USE_REGPARAMS__

INCLUDE = -I. -I$(DIRSRC) -I$(SDKSRC)/common -I$(SDKSRC)/engine -I$(SDKSRC)/dlls -I$(SDKSRC)/pm_shared

BIN_DIR = Release
CFLAGS = $(OPT_FLAGS)

CFLAGS += -g -DNDEBUG -Dlinux -D__linux__ -std=c++0x -shared -wd147,274 -fasm-blocks

OBJ_LINUX := $(OBJECTS:%.c=$(BIN_DIR)/%.o)

$(BIN_DIR)/%.o: %.c
	$(COMPILER) $(INCLUDE) $(CFLAGS) -o $@ -c $<

all:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BIN_DIR)/sdk

	$(MAKE) $(NAME) && strip -x $(BIN_DIR)/$(NAME).so

$(NAME): $(OBJ_LINUX)
	$(COMPILER) $(INCLUDE) $(CFLAGS) $(OBJ_LINUX) $(LINK) -o$(BIN_DIR)/$(NAME).so

check:
	cppcheck $(INCLUDE) --quiet --max-configs=100 -D__linux__ -DNDEBUG -DHAVE_STDINT_H .

debug:	
	$(MAKE) all DEBUG=false

default: all

clean:
	rm -rf Release/*.o
	rm -rf Release/$(NAME).so
	rm -rf Debug/*.o
	rm -rf Debug/$(NAME).so
