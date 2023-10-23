#include <cctype>

#include "File.h"
#include "SetBootFile.h"

static const int	gsc_iStartOffset	= 0xE09;	// Start offset
static const int	gsc_iMaxCharacters	= 8;		// Max characters
static const int	gsc_iMaxFillBytes	= 0x17;		// Bytes to fill, minus the boot filename size

SetBootFile::SetBootFile()
{
}

SetBootFile::~SetBootFile()
{
	close();
}

SetBootFile::Ptr SetBootFile::create()
{
	INSTANCE(pSetBootFile, SetBootFile())

	if (false == pSetBootFile->initialize())
	{
		pSetBootFile.reset();
	}

	return	pSetBootFile;
}

bool SetBootFile::initialize()
{
	return	true;
}

void SetBootFile::close()
{
}

bool SetBootFile::setBootFile(const std::string& _strFilename, const std::string& _strBootFile)
{
	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		printf("Unable to create file interface.\n");

		return	false;
	}

	if (false == pFile->append(_strFilename, true))
	{
		printf("Unable to open file.\n");

		return	false;
	}

	if (false == pFile->seek(gsc_iStartOffset, File::SeekFromStart))
	{
		printf("Unable to seek to boot filename location.\n");

		return	false;
	}

	int	iBootFileSize	= (int)_strBootFile.size();

	if (iBootFileSize > gsc_iMaxCharacters)
	{
		iBootFileSize	= gsc_iMaxCharacters;
	}

	int	iFillSize	= gsc_iMaxFillBytes - iBootFileSize;

	for (int iLoop = 0; iLoop < iBootFileSize; ++iLoop)
	{
		char	szCharacter	= toupper(_strBootFile[iLoop]);

		if (false == pFile->write8Bit(szCharacter))
		{
			printf("Unable to clear the rest of the boot file field.\n");

			return	false;
		}
	}

	for (int iLoop = 0; iLoop < iFillSize; ++iLoop)
	{
		if (false == pFile->write8Bit(0))
		{
			printf("Unable to clear the rest of the boot file field.\n");

			return	false;
		}
	}
	
	pFile->close();

	return	true;
}
