#include <stdio.h>

#include "File.h"
#include "Macros.h"
#include "Pattern.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		usage();

		return	1;
	}

	Pattern::Ptr	pPattern	= Pattern::create(argv[1], argv[2]);

	if (nullptr == pPattern)
	{
		return	1;
	}

	if (false == pPattern->save(argv[3]))
	{
		return	1;
	}

	return	0;
}

void usage()
{
	printf("Usage: PngToSprite SpriteSize PngFile PatternPrefix\nConverts 32 bit png to pattern and color files.\n");
}
