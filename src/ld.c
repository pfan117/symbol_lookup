
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <link.h>
#include <bfd.h>

#include "src/internal.h"

static int
sym_lk_iterate_phdr_cb(struct dl_phdr_info * info, size_t size, void * ptr)	{
	sym_lk_ctx_t * ctx = ptr;
	int l;

	//printf("DBG: %s() %d: size = %lu\n", __func__, __LINE__, size);
	//printf("info->dlpi_name = %s\n", info->dlpi_name);
	//printf("info->dlpi_addr = %p\n", (void*)info->dlpi_addr);

	if (!info->dlpi_name)	{
		return 0;
	}

	l = strnlen(info->dlpi_name, LOAD_OBJ_NAME_MAX);
	if (l < 0 || l >= LOAD_OBJ_NAME_MAX)	{
		return 0;
	}

	if (l == ctx->load_obj_name_len)	{
		if (l)	{
			if (memcmp(info->dlpi_name, ctx->load_obj_name, l))	{
				return 0;
			}
			else	{
				;	/* matched */
			}
		}
		else	{
			;	/* matched */
		}
	}
	else if (l > ctx->load_obj_name_len)	{
		if (ctx->load_obj_name_len)	{
			if (memcmp(
					info->dlpi_name + l - ctx->load_obj_name_len,
					ctx->load_obj_name,
					ctx->load_obj_name_len
					))	{
				return 0;
			}
			else	{
				;	/* matched */
			}
		}
		else	{
			return 0;
		}
	}
	else	{
		return 0;
	}

	//printf("DBG: %s() %d: [%s] vs [%s]\n", __func__, __LINE__, ctx->load_obj_name, info->dlpi_name);

	ctx->load_obj_matched = 1;
	ctx->load_obj_vma = (void*)info->dlpi_addr;

	return 1;
}

int
sym_lk_get_load_object_vma(sym_lk_ctx_t * ctx)	{

	dl_iterate_phdr(sym_lk_iterate_phdr_cb, ctx);

	if (ctx->load_obj_matched)	{
		return 0;
	}

	printf("ERROR: %s() %d: failed to locate vma of base load objects\n", __func__, __LINE__);

	return -1;
}
