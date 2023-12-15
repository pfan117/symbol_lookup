#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <bfd.h>
#include <unistd.h>
#include <stdlib.h>

#include "src/internal.h"

int
sym_lk_open_self_bfd(sym_lk_ctx_t * ctx)	{
	char path[1024];
	int r;

	r = readlink("/proc/self/exe", path, sizeof(path));
	if (r <= 0 || r >= sizeof(path))	{
		printf("ERROR: %s() %d: failed to read self exe file name\n", __func__, __LINE__);
		return -1;
	}

	path[r] = '\0';

	//printf("DBG: %s() %d: path = [%s]\n", __func__, __LINE__, path);

	ctx->b = bfd_openr(path, NULL);
	if (!ctx->b)	{
		printf("ERROR: %s() %d: failed to open bfd\n", __func__, __LINE__);
		return -1;
	}

	int r1 = bfd_check_format(ctx->b, bfd_object);
	int r2 = bfd_check_format_matches(ctx->b, bfd_object, NULL);
	int r3 = bfd_get_file_flags(ctx->b) & HAS_SYMS;

    if (!(r1 && r2 && r3)) {
        bfd_close(ctx->b);
		ctx->b = NULL;
        printf("ERROR: %s() %d: failed to init bfd\n", __func__, __LINE__);
        return -1;
    }

	return 0;
}

static void
sym_lk_section_cb(bfd * b, asection * sect, PTR obj)	{
	sym_lk_ctx_t * ctx = obj;

	if (!sect->name)	{
		return;
	}

	if (strcmp(sect->name, ctx->section_name))	{
		return;
	}

	ctx->section_matched = 1;
	ctx->section_vma = (void *)(sect->vma);
	ctx->section_vma = (void *)(sect->lma);

	return;
}

int
sym_lk_get_section_vma(sym_lk_ctx_t * ctx)	{

	bfd_map_over_sections(ctx->b, sym_lk_section_cb, ctx);

	if (ctx->section_matched)	{
		return 0;
	}

	return -1;
}

int
sym_lk_get_symbol_vma(sym_lk_ctx_t * ctx)	{
	size_t size;
	asymbol **s = NULL;
	int n;
	int i;

	size = bfd_get_symtab_upper_bound(ctx->b);
	if (size <= 0)	{
		printf("ERROR: %s() %d: invalid memory size\n", __func__, __LINE__);
		return -1;
	}

	s = (asymbol **)malloc(size);
	if (!s)	{
		printf("ERROR: %s() %d: oom\n", __func__, __LINE__);
		return -1;
	}

	n = bfd_canonicalize_symtab(ctx->b, s);

	for (i = 0; i < n; i ++)	{
		if (strcmp(bfd_asymbol_name(s[i]), ctx->symbol_name)) {
			continue;
		}

		/*
		printf("DBG: %s() %d: symtab[i]->section->filepos = %p\n", __func__, __LINE__,
				(void*)(s[i]->section->filepos)
				);
		*/

		ctx->symbol_vma = (void*)(s[i]->value);
		//ctx->symbol_vma = (void*)(s[i]->value + s[i]->section->filepos);

		free(s);

		return 0;
	}

	free(s);

	//printf("ERROR: %s() %d: symbol not found\n", __func__, __LINE__);
	
	return -1;
}
