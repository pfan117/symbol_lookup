#include <stdio.h>
#include <string.h>
#include <bfd.h>
#include <dlfcn.h>

#include "src/internal.h"

void *
symbol_lookup(const char * name, const char * section_name, const char * load_obj_name)	{
	int r;
	void * p;

	sym_lk_ctx_t ctx = {0};

	if (name)	{
		ctx.symbol_name = name;
	}
	else	{
		return NULL;
	}

	if (section_name)	{
		ctx.section_name = section_name;
	}
	else	{
		ctx.section_name = ".text";
	}

	if (load_obj_name)	{
		if ('\0' == load_obj_name[0])	{
			ctx.load_obj_name = "";
		}
		else	{
			#if 0
			int l;

			l = strnlen(load_obj_name, LOAD_OBJ_NAME_MAX);
			if (l < 0 || l >= LOAD_OBJ_NAME_MAX)	{
				printf("ERROR: %s() %d: invalid load_obj_name\n", __func__, __LINE__);
				return NULL;
			}
			ctx.load_obj_name = load_obj_name;
			ctx.load_obj_name_len = l;
			#else
			p = dlsym(NULL, name);
			return p;
			#endif
		}
	}
	else	{
		ctx.load_obj_name = "";
	}

	r = sym_lk_get_load_object_vma(&ctx);
	if (r)	{
		printf("ERROR: %s() %d: failed to locate vma of load object\n", __func__, __LINE__);
		return NULL;
	}

	r = sym_lk_open_self_bfd(&ctx);
	if (r)	{
		return NULL;
	}

	r = sym_lk_get_section_vma(&ctx);
	if (r)	{
		printf("ERROR: %s() %d: failed to locate vma of text section\n", __func__, __LINE__);
		return NULL;
	}

	r = sym_lk_get_symbol_vma(&ctx);
	if (r)	{
		printf("ERROR: %s() %d: failed to locate vma of symbol\n", __func__, __LINE__);
		return NULL;
	}

	bfd_close(ctx.b);

	p = (void *)((unsigned long)ctx.load_obj_vma + (unsigned long)ctx.section_vma + (unsigned long)ctx.symbol_vma);

	return p;
}
