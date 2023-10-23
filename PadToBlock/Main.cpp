#include <stdio.h>

#include "File.h"
#include "Macros.h"
#include "PadToBlock.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		usage();

		return	1;
	}

	PadToBlock::Ptr	pPadToBlock	= PadToBlock::create(argv[1], argv[2]);

	if (nullptr == pPadToBlock)
	{
		return	1;
	}

	if (false == pPadToBlock->save(argv[2]))
	{
		return	1;
	}

	return	0;
}

void usage()
{
	printf("Usage: PadToBlock BlockSize File.bin\nPads a file to align with the supplied block size.\n");
}
