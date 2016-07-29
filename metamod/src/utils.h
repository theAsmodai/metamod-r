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
