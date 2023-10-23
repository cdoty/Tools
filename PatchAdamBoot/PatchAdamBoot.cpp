#include <cctype>

#include "File.h"
#include "PatchAdamBoot.h"

static const int	gsc_iBlockSize	= 1024;	// Block size

PatchAdamBoot::PatchAdamBoot()	:
	m_iBlocks(0)
{
}

PatchAdamBoot::~PatchAdamBoot()
{
	close();
}

PatchAdamBoot::Ptr PatchAdamBoot::create(const std::string& _strBaseFile)
{
	INSTANCE(pPatchAdamBoot, PatchAdamBoot())

	if (false == pPatchAdamBoot->initialize(_strBaseFile))
	{
		pPatchAdamBoot.reset();
	}

	return	pPatchAdamBoot;
}

bool PatchAdamBoot::initialize(const std::string& _strBaseFile)
{
	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		printf("Unable to create file interface.\n");

		return	false;
	}

	if (false == pFile->open(_strBaseFile, true))
	{
		printf("Unable to open base file.\n");

		return	false;
	}

	int	iFileSize	= pFile->getLength();

	pFile->close();

	if ((iFileSize % gsc_iBlockSize) != 0)
	{
		printf("Base file is not aligned to the block size.\n");

		return	false;
	}

	m_iBlocks	= iFileSize / gsc_iBlockSize;

	if (0 == m_iBlocks)
	{
		printf("Base file contains no blocks.\n");

		return	false;
	}

	return	true;
}

void PatchAdamBoot::close()
{
}

bool PatchAdamBoot::patchBootFile(const std::string& _strBootFile)
{
	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		printf("Unable to create file interface.\n");

		return	false;
	}

	if (false == pFile->open(_strBootFile, true))
	{
		printf("Unable to open file.\n");

		return	false;
	}

	int	iLength	= pFile->getLength();

	SHAREDBUFFER(m_pFileBuffer, uint8_t, iLength);

	if (nullptr == m_pFileBuffer)
	{
		printf("Unable to allocate memory for boot file.");

		return	false;
	}

	uint8_t*	pBuffer	= m_pFileBuffer.get();

	memset(pBuffer, 0, iLength);

	pFile->readBuffer(&pBuffer, iLength);

	pFile->close();
	
	uint8_t*	pBootFile	= pBuffer;

	for (int iLoop = 0; iLoop < iLength - 1; ++iLoop)
	{
		uint16_t	value	= *(uint16_t*)pBootFile;

		if (value == 0xBC3E)
		{
			pBootFile++;

			*pBootFile	= (m_iBlocks & 0xFF);

			break;
		}

		pBootFile++;
	}

	if (false == pFile->create(_strBootFile, true))
	{
		printf("Unable to create new boot file.");

		return	false;
	}

	if (false == pFile->writeBuffer(pBuffer, iLength))
	{
		printf("Unable to write to new boot file.");

		return	false;
	}

	pFile->close();

	return	true;
}
