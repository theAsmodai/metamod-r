#pragma once

// new/delete operators with malloc/free to remove need for libstdc++

class class_metamod_new {
public:
	// Construction
	class_metamod_new() { };
	// Operators
	inline void *operator new(size_t size)
	{
		if (size==0)
			return calloc(1, 1);
		return calloc(1, size);
	}

	inline void *operator new[](size_t size)
	{
		if (size==0)
			return calloc(1, 1);
		return calloc(1, size);
	}

	inline void operator delete(void *ptr)
	{
		if (ptr)
			free(ptr);
	}

	inline void operator delete[](void *ptr)
	{
		if (ptr)
			free(ptr);
	}
};
