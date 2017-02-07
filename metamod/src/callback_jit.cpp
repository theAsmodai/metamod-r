#include "precompiled.h"

CJit g_jit;

class CUniqueLabel
{
public:
	CUniqueLabel(const char* name) : m_name(name)
	{
		m_name += m_unique_index++;
	}

	operator std::string&()
	{
		return m_name;
	}

private:
	std::string m_name;
	static size_t m_unique_index;
};
size_t CUniqueLabel::m_unique_index;

class CForwardCallbackJIT : public jitasm::function<int, CForwardCallbackJIT, int>
{
public:
	CForwardCallbackJIT(jitdata_t *jitdata);
	void naked_main();
	void call_func(jitasm::Frontend::Reg32 addr);

private:
	jitdata_t* m_jitdata;

	enum
	{
		mg_mres = 0,
		mg_prev_mres = 4,
		mg_status = 8,
		mg_orig_ret = 12,
		mg_over_ret = 16,
	};
};

CForwardCallbackJIT::CForwardCallbackJIT(jitdata_t* jitdata) : m_jitdata(jitdata)
{
}

void CForwardCallbackJIT::naked_main()
{
	// prologue
	push(ebp);
	mov(ebp, esp);
	push(ebx);
	and_(esp, 0xFFFFFFF0);

	enum // stack map
	{
		orig_ret = 0,
		over_ret = 4
	};

	auto align = [](size_t v, size_t a)
	{
		return (v + a - 1) & ~a;
	};

	auto globals = ebx;
	auto mg_backup = m_jitdata->has_ret ? 8 /* orig + over */ : 0;
	auto framesize = align(mg_backup + sizeof(meta_globals_t) + m_jitdata->args_count * sizeof(int), 16) - m_jitdata->args_count * sizeof(int);

	if (m_jitdata->has_varargs) {
		size_t buf_offset = framesize;

		sub(esp, framesize += align(MAX_STRBUF_LEN, 16));

		// format varargs
		lea(edx, dword_ptr[ebp + 8 + m_jitdata->args_count * sizeof(int)]); // varargs ptr
		lea(eax, dword_ptr[esp + buf_offset]); // buf ptr
		mov(ecx, size_t(vsnprintf));

		push(edx);
		push(dword_ptr[ebp + 8 + (m_jitdata->args_count - 1) * sizeof(int)]); // last arg of pfn (format string)
		push(MAX_STRBUF_LEN);
		push(eax);
		call(ecx);
		add(esp, 4 * sizeof(int));
	}
	else
		sub(esp, framesize);

	// setup globals ptr and backup old data
	mov(globals, size_t(&g_metaGlobals));
	movups(xmm0, xmmword_ptr[globals]);
	mov(eax, dword_ptr[globals + 16]);
	movups(xmmword_ptr[esp + mg_backup], xmm0);
	mov(dword_ptr[esp + mg_backup + 16], eax);

	// call metamod's pre hook if present
	if (m_jitdata->mm_hook && m_jitdata->mm_hook_time == P_PRE) {
		mov(ecx, m_jitdata->mm_hook);
		call_func(ecx);
	}

	// setup meta globals
	mov(dword_ptr[globals + mg_mres], MRES_UNSET);

	// setup retval pointers
	if (m_jitdata->has_ret) {
		lea(eax, dword_ptr[esp + over_ret]);
		mov(dword_ptr[globals + mg_orig_ret], esp);
		mov(dword_ptr[globals + mg_over_ret], eax);
	}

	// call pre
	for (int i = 0, hookid = 0; i < m_jitdata->plugins_count; i++) {
		auto plug = &m_jitdata->plugins[i];

		if (plug->m_status < PL_RUNNING)
			continue;

		size_t fn_table = *(size_t *)(size_t(plug) + m_jitdata->table_offset);

		// plugin don't want any hooks from that table
		if (!fn_table)
			continue;

		CUniqueLabel go_next_plugin("go_next_plugin");

		// check status and handler set
		cmp(byte_ptr[size_t(&plug->m_status)], PL_RUNNING);
		mov(ecx, dword_ptr[fn_table + m_jitdata->pfn_offset]);
		jnz(go_next_plugin);
		jecxz(go_next_plugin);

		if (hookid++) {
			mov(eax, dword_ptr[globals + mg_mres]);
			mov(dword_ptr[globals + mg_mres], MRES_IGNORED);
			mov(dword_ptr[globals + mg_prev_mres], eax);
		}
		else { // init
			xor_(eax, eax);
			mov(dword_ptr[globals + mg_mres], MRES_IGNORED);
			mov(dword_ptr[globals + mg_prev_mres], eax); // MRES_UNSET
			mov(dword_ptr[globals + mg_status], eax); // NULL
		}

		call_func(ecx);

		mov(edx, dword_ptr[globals + mg_mres]);
		mov(ecx, dword_ptr[globals + mg_status]);
		cmp(edx, ecx);
		cmovg(ecx, edx);
		mov(dword_ptr[globals + mg_status], ecx);

		if (m_jitdata->has_ret) {
			mov(ecx, dword_ptr[esp + over_ret]);
			cmp(edx, MRES_SUPERCEDE);
			cmovz(ecx, eax);
			mov(dword_ptr[esp + over_ret], ecx);
		}

		L(go_next_plugin);
	}

	// call original if need
	cmp(dword_ptr[globals + mg_status], MRES_SUPERCEDE);
	jz("skip_original");
	{
		if (m_jitdata->pfn_original) {
			mov(ecx, m_jitdata->pfn_original);
			call_func(ecx);
		}

		if (m_jitdata->has_ret) {
			if (m_jitdata->pfn_original)
				mov(dword_ptr[esp + orig_ret], eax);
			else
				mov(dword_ptr[esp + orig_ret], TRUE); // for should collide :/

			jmp("skip_supercede");
		}
	}
	L("skip_original");
	{
		if (m_jitdata->has_ret) {
			// if supercede
			mov(eax, dword_ptr[esp + over_ret]);
			mov(dword_ptr[esp + orig_ret], eax);

			L("skip_supercede");
		}
	}
	L("skip_all");

	// call post
	for (int i = 0, hookid = 0; i < m_jitdata->plugins_count; i++) {
		auto plug = &m_jitdata->plugins[i];

		if (plug->m_status < PL_RUNNING)
			continue;

		size_t fn_table = *(size_t *)(size_t(plug) + m_jitdata->post_table_offset);

		// plugin don't want any hooks from that table
		if (!fn_table)
			continue;

		CUniqueLabel go_next_plugin("go_next_plugin");

		// check status and handler set
		cmp(byte_ptr[size_t(&plug->m_status)], PL_RUNNING);
		mov(ecx, dword_ptr[fn_table + m_jitdata->pfn_offset]);
		jnz(go_next_plugin);
		jecxz(go_next_plugin);

		if (hookid++) {
			mov(eax, dword_ptr[globals + mg_mres]);
			mov(dword_ptr[globals + mg_mres], MRES_IGNORED);
			mov(dword_ptr[globals + mg_prev_mres], eax);
		}
		else { // init
			xor_(eax, eax);
			mov(dword_ptr[globals + mg_mres], MRES_IGNORED);
			mov(dword_ptr[globals + mg_prev_mres], eax); // MRES_UNSET
			mov(dword_ptr[globals + mg_status], eax); // NULL
		}

		call_func(ecx);

		mov(edx, dword_ptr[globals + mg_mres]);
		mov(ecx, dword_ptr[globals + mg_status]);
		cmp(ecx, edx);
		cmovl(ecx, edx);
		mov(dword_ptr[globals + mg_status], ecx);

		if (m_jitdata->has_ret) {
			cmp(edx, MRES_SUPERCEDE);
			mov(ecx, dword_ptr[esp + over_ret]);
			cmovz(ecx, eax);
			mov(dword_ptr[esp + over_ret], ecx);
		}

		L(go_next_plugin);
	}

	// call metamod's post hook if present
	if (m_jitdata->mm_hook && m_jitdata->mm_hook_time == P_POST) {
		mov(ecx, m_jitdata->mm_hook);
		call_func(ecx);
	}

	movups(xmm0, xmmword_ptr[esp + mg_backup]);
	mov(eax, dword_ptr[esp + mg_backup + 16]);
	movups(xmmword_ptr[globals], xmm0);
	mov(dword_ptr[globals + 16], eax);

	if (m_jitdata->has_ret) {
		mov(eax, dword_ptr[esp + orig_ret]);
		cmp(dword_ptr[globals + mg_status], MRES_OVERRIDE);
		cmovz(eax, dword_ptr[esp + over_ret]);
	}

	// epilogue
	pop(ebx);
	mov(esp, ebp);
	pop(ebp);
	ret();
}

void CForwardCallbackJIT::call_func(jitasm::Frontend::Reg32 addr)
{
	const size_t fixed_args_count = m_jitdata->args_count - (m_jitdata->has_varargs ? 1u /* excluding format string */ : 0u);
	const size_t strbuf_offset = (m_jitdata->has_ret ? 8u : 0u) + sizeof(meta_globals_t);

	// push formatted buf instead of format string
	if (m_jitdata->has_varargs) {
		lea(eax, dword_ptr[esp + strbuf_offset]);
		push(eax);
	}

	// push normal args
	for (size_t j = fixed_args_count; j > 0; j--)
		push(dword_ptr[ebp + 8 + (j - 1) * sizeof(int)]);

	// call
	call(addr);

	// pop stack
	if (m_jitdata->args_count)
		add(esp, m_jitdata->args_count * sizeof(int));
}

CJit::CJit() : m_callback_allocator(static_allocator::mp_rwx), m_tramp_allocator(static_allocator::mp_rwx)
{
}

size_t CJit::compile_callback(jitdata_t* jitdata)
{
	if (!is_hook_needed(jitdata)) {
		return jitdata->pfn_original;
	}

	CForwardCallbackJIT callback(jitdata);
	callback.Assemble();

	auto code = callback.GetCode();
	auto codeSize = callback.GetCodeSize();
	auto ptr = m_callback_allocator.allocate(codeSize);

	return (size_t)memcpy(ptr, code, codeSize);
}

size_t CJit::compile_tramp(size_t ptr_to_func)
{
	auto code = (uint8 *)m_tramp_allocator.allocate(2 + sizeof(int));

	// jmp dword [ptr_to_func]
	code[0] = 0xFFu;
	code[1] = 0x25u;
	*(size_t *)&code[2] = ptr_to_func;

	return (size_t)code;
}

void CJit::clear_callbacks()
{
	m_callback_allocator.deallocate_all();
}

void CJit::clear_tramps()
{
	m_tramp_allocator.deallocate_all();
}

bool CJit::is_hook_needed(jitdata_t* jitdata)
{
	if (jitdata->mm_hook)
		return true;

	if (!jitdata->plugins)
		return false;

	for (int i = 0, hookid = 0; i < jitdata->plugins_count; i++) {
		auto plug = &jitdata->plugins[i];

		const size_t fn_table			= *(size_t *)(size_t(plug) + jitdata->table_offset);
		const size_t fn_table_post		= *(size_t *)(size_t(plug) + jitdata->post_table_offset);

		if (fn_table || fn_table_post) {
			return true;
		}
	}

	return false;
}
