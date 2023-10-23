#include <stdio.h>

#include "File.h"
#include "Functions.h"
#include "Macros.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		usage();

		return	1;
	}

	int	iBankSize	= Functions::convertStringToInt(argv[1]);

	File::Ptr	pFile	= File::create();

	if (false == pFile->open(argv[2], true))
	{
		printf("Unable to open binary file %s", argv[1]);

		return	1;
	}

	DEFINESHAREDBUFFER(pBinary, uint8_t, iBankSize * 2);

	memset(pBinary.get(), 0xFF, iBankSize * 2);

	if (false == pFile->readBuffer(pBinary.get(), iBankSize * 2))
	{
		printf("Unable to read from binary file %s", argv[1]);

		return	1;
	}

	pFile->close();

	if (false == pFile->create(argv[2], true))
	{
		printf("Unable to create binary file %s", argv[1]);

		return	1;
	}

	if (false == pFile->writeBuffer(pBinary.get() + iBankSize, iBankSize))
	{
		printf("Unable to write to binary file %s", argv[1]);

		return	1;
	}

	if (false == pFile->writeBuffer(pBinary.get(), iBankSize))
	{
		printf("Unable to write to binary file %s", argv[1]);

		return	1;
	}

	pFile->close();

	return	0;
}

void usage()
{
	printf("Usage: SwapBanks BankSize BinFile\n");
}
