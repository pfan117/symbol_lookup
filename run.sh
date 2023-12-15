#!/bin/bash

function show_usage
{
	echo ""
	echo "examples:"
	echo "    $0"
	echo "    $0 c"
	echo "    $0 release"
	echo "    $0 h"
	echo ""
}

DO_CLEAN="no"
DO_BUILD_FOR_RELEASE="no"

for opt in "$@"
do
case $opt in
	c|clean)
	DO_CLEAN="yes"
	;;
	release)
	DO_BUILD_FOR_RELEASE="yes"
	;;
	*)
	show_usage
	exit 1
	;;
esac
done

function do_clean	{
	make clean
}

if [[ "yes" == "${DO_CLEAN}" ]] ; then
	do_clean
	exit 0
fi

if [[ "yes" == "${DO_BUILD_FOR_RELEASE}" ]] ; then
	make libsymbollookup.so || exit 1
	exit 0
fi

make debug=1 libsymbollookup.so || exit 1
make debug=1 libtest.so || exit 1
make debug=1 a.out || exit 1

ctags -R *

export LD_LIBRARY_PATH=./:${LD_LIBRARY_PATH}
./a.out

# eof
