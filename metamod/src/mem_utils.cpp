#include "precompiled.h"

static_allocator::static_allocator(memory_protection protection) : m_protection(protection)
{
}

char *static_allocator::allocate(const size_t n)
{
	if (!m_pages.size() || m_used + n > Pagesize)
		allocate_page();

	auto ptr = reinterpret_cast<char *>(m_pages.back()) + m_used;
	m_used += n;
	return ptr;
}

char *static_allocator::strdup(const char *string)
{
	size_t len = Q_strlen(string) + 1;
	return (char *)Q_memcpy(allocate(len), string, len);
}

void static_allocator::deallocate_all()
{
	for (auto page : m_pages)
#ifdef WIN32
		VirtualFree(page, 0, MEM_RELEASE);
#else
		munmap(page, Pagesize);
#endif

	m_pages.clear();
}

size_t static_allocator::memory_used() const
{
	return (m_pages.size() - 1) * Pagesize + m_used;
}

bool static_allocator::contain(uint32 addr)
{
	for (auto p : m_pages) {
		if (uint32(p) <= addr && addr < uint32(p) + Pagesize)
			return true;
	}
	return false;
}

char *static_allocator::find_pattern(char *pattern, size_t len)
{
	for (auto p : m_pages) {
		for (char *c = (char *)p, *e = c + Pagesize - len; c < e; c++) {
			if (mem_compare(c, pattern, len))
				return c;
		}
	}
	return nullptr;
}

void static_allocator::allocate_page()
{
#ifdef WIN32
	auto page = VirtualAlloc(nullptr, Pagesize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else
	auto page = mmap(nullptr, Pagesize, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
#endif

	m_used = 0;
	m_pages.push_back(page);
}

bool mem_compare(const char *addr, const char *pattern, size_t len)
{
	for (auto c = pattern, pattern_end = pattern + len; c < pattern_end; ++c, ++addr) {
		if (*c == *addr || *c == '\x2A')
			continue;
		return false;
	}
	return true;
}

char *mem_find_pattern(char *pos, int range, const char *pattern, size_t len)
{
	for (auto c = pos + range - len; pos < c; ++pos) {
		if (mem_compare(pos, pattern, len))
			return pos;
	}

	return nullptr;
}

char *mem_find_ref(char *pos, char *end, char opcode, uint32 ref, bool relative)
{
	for (; pos < end; ++pos)
	{
		if (*pos == opcode)
		{
			if (relative)
			{
				if ((uint32)pos + 5 + *(uint32 *)(pos + 1) == ref)
					return pos;
			}
			else
			{
				if (*(uint32 *)(pos + 1) == ref)
					return pos;
			}
		}
	}

	return nullptr;
}

char *mem_find_string_push(char *addr, const char *string, size_t len)
{
	char *ptr = mem_find_pattern(addr, len, string, Q_strlen(string) + 1);
	return mem_find_ref(addr, addr + len - 5, '\x68', (uint32)ptr, false);
}
