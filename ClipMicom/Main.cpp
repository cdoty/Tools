#include <stdio.h>

#include "File.h"
#include "Macros.h"
#include "Micom.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		usage();

		return	1;
	}

	Micom::Ptr	pMicom	= Micom::create(argv[1], argv[2]);

	if (nullptr == pMicom)
	{
		return	1;
	}

	if (false == pMicom->save(argv[3]))
	{
		return	1;
	}

	return	0;
}

void usage()
{
	printf("Usage: ClipMicom Origin(Left, Center, Right) File.in File.out\nClips a buffer to the Micom output size with the specified origin.\n");
}
