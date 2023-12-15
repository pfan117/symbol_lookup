#ifndef __SYMBOL_LOOKUP_HEADER_INCLUDED__
#define __SYMBOL_LOOKUP_HEADER_INCLUDED__

#ifdef __cpluscplus
extern "C"	{
#endif

extern void * symbol_lookup(const char * name, const char * section_name, const char * load_obj_name);

#ifdef __cpluscplus
}
#endif

#endif
