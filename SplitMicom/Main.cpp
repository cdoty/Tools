#include <stdio.h>

#include "File.h"
#include "Functions.h"
#include "Macros.h"
#include "Micom.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc != 7)
	{
		usage();

		return	1;
	}

	Micom::Ptr	pMicom	= Micom::create(argv[2]);

	if (nullptr == pMicom)
	{
		return	1;
	}

	int	iRomSize	= Functions::convertStringToInt(argv[1]);

	if (false == pMicom->save(iRomSize, argv[3], argv[4], argv[5], argv[6]))
	{
		return	1;
	}

	return	0;
}

void usage()
{
	printf("Usage: SplitMicom RomSize File.in File1.out File2.out File3.out File4.out\nSplits a file into the 4 Micom roms.\n");
}
