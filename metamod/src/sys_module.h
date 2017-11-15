#pragma once

#ifdef _WIN32
typedef HINSTANCE module_handle_t;
#else
typedef void* module_handle_t;
#endif

class CSysModule
{
public:
	CSysModule();

	module_handle_t load(void *addr);
	module_handle_t load(const char *filename);
	bool unload();
	void *getsym(const char *name) const;
	module_handle_t gethandle() const;
	uintptr_t getbase() const;
	size_t getsize() const;
	bool contain(void *addr) const;
	bool is_opened() const;

	char *find_string_push(const char *string);
	char *find_pattern(char *pos, int range, const char *pattern, size_t len);

	static module_handle_t find(void *addr);
	static const char *getloaderror();
	static const module_handle_t INVALID_HANDLE;

private:
	module_handle_t m_handle;
	uintptr_t m_base;
	size_t m_size;
	bool m_free;				// m_handle should be released
};
