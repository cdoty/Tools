#include <stdio.h>

#include "File.h"
#include "Macros.h"
#include "BinFile.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		usage();

		return	1;
	}

	BinFile::Ptr	pBinFile	= BinFile::create(argv[1]);

	if (nullptr == pBinFile)
	{
		return	1;
	}

	if (false == pBinFile->save(argv[2]))
	{
		return	1;
	}

	return	0;
}

void usage()
{
	printf("Usage: BinFile File.bin File.asm\nConverts a binary file to a CATE assembler file.\n");
}
