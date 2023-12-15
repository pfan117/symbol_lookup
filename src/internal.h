#ifndef __SYMBOL_LOOKUP_INTERNAL_HEADER_INCLUDED__
#define __SYMBOL_LOOKUP_INTERNAL_HEADER_INCLUDED__

typedef struct _sym_lk_ctx_t	{
	const char * symbol_name;
	const char * section_name;
	const char * load_obj_name;

	int load_obj_name_len;

	bfd * b;

	void * symbol_vma;
	void * section_vma;
	void * load_obj_vma;
	int section_matched:1;
	int load_obj_matched:1;
}
sym_lk_ctx_t;

#define LOAD_OBJ_NAME_MAX	1024

int sym_lk_get_load_object_vma(sym_lk_ctx_t *);
int sym_lk_open_self_bfd(sym_lk_ctx_t *);
int sym_lk_get_section_vma(sym_lk_ctx_t *);
int sym_lk_get_symbol_vma(sym_lk_ctx_t *);

#endif
