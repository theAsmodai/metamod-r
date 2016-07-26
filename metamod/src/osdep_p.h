#pragma once

#include "types_meta.h"		// mBOOL
#include "osdep.h"		// PATH_MAX

// Checks if file is hlsdk api game dll (osdep_detect_gamedll_linux.cpp and osdep_detect_gamedll_win32.cpp)
mBOOL is_gamedll(const char *filename);

// MSVC doesn't provide opendir/readdir/closedir, so we write our own.
#ifdef _WIN32
	struct my_dirent {
		char d_name[PATH_MAX];
	};
	typedef struct {
		HANDLE handle;
		WIN32_FIND_DATAA find_data;
		struct my_dirent ent;
		int not_found;
	} my_DIR;
	
	#define dirent my_dirent
	#define DIR my_DIR
	
	DIR *my_opendir(const char *);
	struct dirent *my_readdir(DIR *);
	void my_closedir(DIR *);
	
	#define opendir(x) my_opendir(x)
	#define readdir(x) my_readdir(x)
	#define closedir(x) my_closedir(x)
#endif // _WIN32

DLHANDLE get_module_handle_of_memptr(void *memptr);
