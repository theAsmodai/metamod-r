#include "precompiled.h"

bool is_yes(const char* str)
{
	return !Q_strcmp(str, "true") || !Q_strcmp(str, "yes") || !Q_strcmp(str, "1");
}

bool is_no(const char* str)
{
	return !Q_strcmp(str, "false") || !Q_strcmp(str, "no") || !Q_strcmp(str, "0");
}

char* ENTITY_KEYVALUE(edict_t* entity, char* key)
{
	return INFOKEY_VALUE(GET_INFOKEYBUFFER(entity), key);
}

const char* LOCALINFO(char* key)
{
	return ENTITY_KEYVALUE(NULL, key);
}

char* execmem_allocator::allocate(const size_t n)
{
	if (!m_pages.size() || m_used + n > Pagesize)
		allocate_page();

	auto ptr = reinterpret_cast<char *>(m_pages.back()) + m_used;
	m_used += n;
	return ptr;
}

void execmem_allocator::deallocate_all()
{
	for (auto page : m_pages)
#ifdef WIN32
		VirtualFree(page, 0, MEM_RELEASE);
#else
		munmap(page, Pagesize);
#endif

	m_pages.clear();
}

size_t execmem_allocator::memoryUsed() const
{
	return (m_pages.size() - 1) * Pagesize + m_used;
}

void execmem_allocator::allocate_page()
{
#ifdef WIN32
	auto page = VirtualAlloc(NULL, Pagesize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else
	auto page = mmap(NULL, Pagesize, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
#endif

	m_used = 0;
	m_pages.push_back(page);
}
