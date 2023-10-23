#include <stdio.h>

#include "File.h"
#include "Macros.h"
#include "PatchAdamBoot.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		usage();

		return	1;
	}

	PatchAdamBoot::Ptr	pPatchAdamBoot	= PatchAdamBoot::create(argv[1]);

	if (nullptr == pPatchAdamBoot)
	{
		return	1;
	}

	if (false == pPatchAdamBoot->patchBootFile(argv[2]))
	{
		return	1;
	}

	return	0;
}

void usage()
{
	printf("Usage: PatchAdamBoot Base.rom Boot.rom\nPatches the boot rom with the block count to load.\n");
}
