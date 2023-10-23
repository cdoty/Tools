#include <stdio.h>

#include "File.h"
#include "Macros.h"
#include "SetBootFile.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		usage();

		return	1;
	}

	SetBootFile::Ptr	pSetBootFile	= SetBootFile::create();

	if (nullptr == pSetBootFile)
	{
		return	1;
	}

	if (false == pSetBootFile->setBootFile(argv[1], argv[2]))
	{
		return	1;
	}

	return	0;
}

void usage()
{
	printf("Usage: SetBootFile File.dsk BootFile\nSets the boot file on an Einstein disk image.\n");
}
