#include <stdio.h>

#include "Chars.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		usage();

		return	1;
	}

	Chars::Ptr	pChars	= Chars::create(argv[1]);

	if (nullptr == pChars)
	{
		return	1;
	}

	if (false == pChars->save(argv[2]))
	{
		return	1;
	}

	return	0;
}

void usage()
{
	printf("Usage: PNGToChars ImageFile CharData\n");
}
