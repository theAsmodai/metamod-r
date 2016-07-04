#ifndef OSDEP_P_H
#define OSDEP_P_H

#include "types_meta.h"		// mBOOL
#include "osdep.h"		// PATH_MAX

// Checks if file is hlsdk api game dll 
//   (osdep_detect_gamedll_linux.cpp and osdep_detect_gamedll_win32.cpp)
//  --Jussi Kivilinna
mBOOL DLLINTERNAL is_gamedll(const char *filename);

// MSVC doesn't provide opendir/readdir/closedir, so we write our own.
//  --Jussi Kivilinna
#ifdef _WIN32
	struct my_dirent {
		char d_name[PATH_MAX];
	};
	typedef struct {
		HANDLE			 handle;
		WIN32_FIND_DATAA find_data;
		struct my_dirent ent;
		int              not_found;
	} my_DIR;
	
	#define dirent my_dirent
	#define DIR my_DIR
	
	DIR * DLLINTERNAL my_opendir(const char *);
	struct dirent * DLLINTERNAL my_readdir(DIR *);
	void DLLINTERNAL my_closedir(DIR *);
	
	#define opendir(x) my_opendir(x)
	#define readdir(x) my_readdir(x)
	#define closedir(x) my_closedir(x)
#else
	#include <dirent.h>
#endif /* _WIN32 */

DLHANDLE DLLINTERNAL get_module_handle_of_memptr(void * memptr);

#ifdef linux
	void * DLLINTERNAL get_dlsym_pointer(void);
#endif

#endif /* OSDEP_P_H */
