#include <stdio.h>
#include "symbol_lookup.h"
#include "test/so/header.h"

#define LOCATE(__F__,__S__,__L__)	do	{ \
	p = symbol_lookup(#__F__, __S__, __L__); \
	printf("DBG: address of "#__F__"() is %p, symbol_lookup() return %p\n", __F__, p); \
}while(0);

int
main(int argc, const char ** argv)	{
	void * p;

	LOCATE(main, NULL, NULL);

	LOCATE(main, ".text", "");

	LOCATE(symbol_lookup, ".text", "/libsymbollookup.so");

	LOCATE(func_in_so, ".text", "/libtest.so");

	LOCATE(printf, ".text", "so");

	return 0;
}
