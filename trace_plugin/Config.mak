MODNAME = trace_mm

EXTRA_CFLAGS = -Wno-unused -fno-rtti

SRCFILES = api_info.cpp dllapi.cpp dllapi_post.cpp engine_api.cpp \
	engine_api_post.cpp h_export.cpp log_plugin.cpp meta_api.cpp \
	plugin.cpp sdk_util.cpp trace_api.cpp vdate.cpp

LINKED_SRCFILES = sdk_util.cpp api_info.cpp res_meta.rc
LINK_DEST_DIR = ../metamod

INFOFILES = info_name.h ../metamod/vers_meta.h
RESFILE = res_meta.rc
