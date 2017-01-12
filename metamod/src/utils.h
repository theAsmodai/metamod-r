#pragma once

template <typename T, size_t N>
char(&ArraySizeHelper(T(&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))

class static_allocator
{
public:
	enum memory_protection : uint8
	{
#ifdef _WIN32
		mp_readwrite = PAGE_READWRITE,
		mp_rwx = PAGE_EXECUTE_READWRITE
#else
		mp_readwrite = PROT_READ | PROT_WRITE,
		mp_rwx = PROT_READ | PROT_WRITE | PROT_EXEC
#endif
	};

	static_allocator(memory_protection protection);
	char* allocate(const size_t n);
	char* strdup(const char* string);
	void deallocate_all();
	size_t memory_used() const;

	template<typename T>
	T* allocate()
	{
		return (T *)allocate(sizeof(T));
	}

private:
	void allocate_page();

	enum
	{
		Pagesize = 4096
	};

	size_t m_used = 0;
	std::vector<void *> m_pages;
	memory_protection m_protection;
};

extern static_allocator g_static_allocator;

bool is_yes(const char* str);
bool is_no(const char* str);

const char* LOCALINFO(char* key);

#ifdef _WIN32
char *mm_strtok_r(char *s, const char *delim, char **ptrptr);
#endif

char* trimbuf(char *str);
void normalize_pathname(char *path);
bool is_absolute_path(const char *path);
char *realpath(const char *file_name, char *resolved_name);
