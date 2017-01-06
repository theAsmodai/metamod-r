#pragma once

class execmem_allocator
{
public:
	char* allocate(const size_t n);
	void deallocate_all();
	size_t memoryUsed() const;

private:
	void allocate_page();

	enum
	{
		Pagesize = 4096
	};

	size_t m_used = 0;
	std::vector<void *> m_pages;
};

bool is_yes(const char* str);
bool is_no(const char* str);

const char* LOCALINFO(char* key);

#ifdef _WIN32
char *mm_strtok_r(char *s, const char *delim, char **ptrptr);
#endif

void normalize_pathname(char *path);
bool is_absolute_path(const char *path);
char *realpath(const char *file_name, char *resolved_name);
