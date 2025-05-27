#include <stdio.h>

#include "Bin.h"
#include "File.h"
#include "Macros.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc < 6 || argc > 7)
	{
		usage();

		return	1;
	}

	int	iStartingTile	= std::atoi(argv[1]);

	Bin::Ptr	pBin;
	
	if (7 == argc)
	{
		pBin	= Bin::create(argv[2], argv[4], argv[5], argv[6], iStartingTile);
	}

	else
	{
		pBin	= Bin::create(argv[2], argv[4], argv[5], "", 0);
	}

	if (nullptr == pBin)
	{
		return	1;
	}

	if (false == pBin->save(argv[3], iStartingTile, 7 == argc))
	{
		return	1;
	}

	return	0;
}

void usage()
{
	printf("Usage: PngToBitmap StartingTile PngFile BinPrefix StartY EndY [PatternPrefix]\nConverts 32 bit png to a full set of tiles.\n");
}
