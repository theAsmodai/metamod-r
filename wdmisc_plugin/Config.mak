MODNAME = wdmisc_mm

EXTRA_CFLAGS = 

SRCFILES = dll_plugin.cpp engine_api.cpp h_export.cpp log_plugin.cpp \
	meta_api.cpp sdk_util.cpp vdate.cpp \
	wdmisc.cpp 

LINKED_SRCFILES = sdk_util.cpp res_meta.rc
LINK_DEST_DIR = ../metamod

INFOFILES = info_name.h ../metamod/vers_meta.h
RESFILE = res_meta.rc
