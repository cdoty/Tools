#include <stdio.h>

#include "Comments.h"
#include "File.h"
#include "Macros.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc != 6)
	{
		usage();

		return	1;
	}

	Comments::Ptr	pComments	= Comments::create(argv[1], argv[3], argv[4]);

	if (nullptr == pComments)
	{
		return	1;
	}

	if (false == pComments->save(argv[5], argv[2]))
	{
		return	1;
	}

	return	0;
}

void usage()
{
	printf("Usage: CreateComments MAMESystem CPUTag File.in Binary.in OutDirectory\nConverts *.symbols.txt and *.lst files to a comments file.\n");
}
