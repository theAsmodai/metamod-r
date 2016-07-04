#ifndef RET_TYPE_H
#define RET_TYPE_H

#include "new_baseclass.h"

class class_ret_t : public class_metamod_new {
public:
	// Construction
	inline class_ret_t(void) { };
	inline class_ret_t(float f) { data.f = f; };
	inline class_ret_t(void * p) { data.p = p; };
	inline class_ret_t(const char * pc) { data.pc = pc; };
	inline class_ret_t(int i) { data.i = i; };
	inline class_ret_t(short s) { data.i = s; };
	inline class_ret_t(char c) { data.i = c; };
	inline class_ret_t(unsigned int ui) { data.ui = ui; };
	inline class_ret_t(unsigned long ui) { data.ui = ui; };
	inline class_ret_t(unsigned short us) { data.ui = us; };
	inline class_ret_t(unsigned char uc) { data.ui = uc; };
	
	// Reading/Writing
	inline void * getptr(void) { return(&data); };
	
	#define SET_RET_CLASS(ret,type,x) \
		*(type*)((ret).getptr()) = (type)(x)
	#define GET_RET_CLASS(ret,type) \
		(*(type*)((ret).getptr()))
private:
	//Data (select data size of largest type) (x86: 32bit, x86_64: 64bit)
	union {
		void * p;
		const char * pc;
		float f;
		long i;
		unsigned long ui;
	} data;
};

#endif /*RET_TYPE_H*/
