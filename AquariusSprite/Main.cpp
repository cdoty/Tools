#include <stdio.h>

#include "Aquarius.h"
#include "File.h"
#include "Functions.h"
#include "Macros.h"

void usage();

int main(int argc, char* argv[])
{
	if (argc != 6)
	{
		usage();

		return	1;
	}

	Aquarius::Ptr	pAquarius;
	
	pAquarius	= Aquarius::create(argv[4]);

	if (nullptr == pAquarius)
	{
		return	1;
	}

	int	iSpriteWidth	= Functions::convertStringToInt(argv[1]);
	int	iSpriteHeight	= Functions::convertStringToInt(argv[2]);
	int	iSpriteCount	= Functions::convertStringToInt(argv[3]);

	if (false == pAquarius->save(iSpriteWidth, iSpriteHeight, iSpriteCount, argv[5]))
	{
		return	1;
	}

	return	0;
}

void usage()
{
	printf("Usage: AquariusSprite SpriteWidth SpriteHeight SpriteCount Screen.json SavePrefix\nConverts Aquarius Draw files to Aquarius sprite and color files.\nWidth and height are in characters.\n");
}
