#include <stdio.h>

#include "File.h"
#include "Macros.h"
#include "Symbols.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		usage();

		return	1;
	}

	Symbols::Ptr	pSymbols	= Symbols::create(argv[1]);

	if (nullptr == pSymbols)
	{
		return	1;
	}

	if (false == pSymbols->save(argv[2]))
	{
		return	1;
	}

	return	0;
}

void usage()
{
	printf("Usage: CreateSym File.Symbols.txt File.sym\nConverts *.symbols.txt and *.lst files to a sym file.\n");
}
