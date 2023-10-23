#include <stdio.h>

#include "File.h"
#include "Macros.h"
#include "PadFile.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		usage();

		return	1;
	}

	PadFile::Ptr	pPadFile	= PadFile::create(argv[1], argv[2], argv[3]);

	if (nullptr == pPadFile)
	{
		return	1;
	}

	if (false == pPadFile->save(argv[3]))
	{
		return	1;
	}

	return	0;
}

void usage()
{
	printf("Usage: PadFile PadByte PadSize File.bin\nPads a file to a specific file size. Creates the file if it doesn't exist.\n");
}
