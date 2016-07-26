#include "precompiled.h"

#ifdef _WIN32
// MSVC doesn't provide "dirent.h" header. These functions wrap opendir/readdir/closedir
// functions to FindFirst/FindNext/FindClose win32api-functions.
DIR *my_opendir(const char *path)
{
	char search_path[MAX_PATH];
	DIR *dir;

	// Add wildcards to path
	safevoid_snprintf(search_path, sizeof(search_path), "%s\\*.*", path);

	// Get memory for new DIR object
	dir = (DIR *)calloc(1, sizeof(DIR));

	// Start searching
	dir->handle = FindFirstFileA(search_path, &dir->find_data);
	if (dir->handle == INVALID_HANDLE_VALUE)
	{
		free(dir);
		return nullptr;
	}

	// Found file
	dir->not_found = 0;
	return dir;
}

struct dirent *my_readdir(DIR *dir)
{
	// If not found stop
	if (!dir || dir->not_found)
		return nullptr;

	// Set filename
	Q_strncpy(dir->ent.d_name, dir->find_data.cFileName, sizeof(dir->ent.d_name) - 1);
	dir->ent.d_name[sizeof(dir->ent.d_name) - 1] = '\0';

	// Search next
	dir->not_found = !FindNextFileA(dir->handle, &dir->find_data);
	return &dir->ent;
}

void my_closedir(DIR *dir)
{
	if (!dir)
		return;

	FindClose(dir->handle);
	free(dir);
}

#endif // _WIN32

// get module handle of memptr
#ifdef _WIN32
DLHANDLE get_module_handle_of_memptr(void *memptr)
{
	MEMORY_BASIC_INFORMATION MBI;

	if (!VirtualQuery(memptr, &MBI, sizeof(MBI)))
		return nullptr;

	if (MBI.State != MEM_COMMIT)
		return nullptr;

	if (!MBI.AllocationBase)
		return nullptr;

	return (DLHANDLE)MBI.AllocationBase;
}
#else
DLHANDLE get_module_handle_of_memptr(void *memptr)
{
	Dl_info dli;
	Q_memset(&dli, 0, sizeof(dli));

	if (dladdr(memptr, &dli))
		return dlopen(dli.dli_fname, RTLD_NOW);
	else
		return (void*)0;
}
#endif
