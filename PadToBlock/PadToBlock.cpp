#include "File.h"
#include "Functions.h"
#include "PadToBlock.h"

PadToBlock::PadToBlock()	:
	m_iFinalSize(0)
{
}

PadToBlock::~PadToBlock()
{
	close();
}

PadToBlock::Ptr PadToBlock::create(const std::string& _strBlockSize, const std::string& _strFilename)
{
	INSTANCE(pPadToBlock, PadToBlock())

	if (false == pPadToBlock->initialize(_strBlockSize, _strFilename))
	{
		pPadToBlock.reset();
	}

	return	pPadToBlock;
}

bool PadToBlock::initialize(const std::string& _strBlockSize, const std::string& _strFilename)
{
	int	iBlockSize;	// Block size

	if (_strBlockSize.find("0x") != std::string::npos)
	{
		iBlockSize	= (int)Functions::convertHexStringToUnsigned32(_strBlockSize);
	}

	else
	{
		iBlockSize	= Functions::convertStringToInt(_strBlockSize);
	}

	if (0 == iBlockSize)
	{
		printf("Block size cannot be zero.\n");

		return	false;
	}

	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		printf("Unable to create file interface.\n");

		return	false;
	}

	if (false == pFile->open(_strFilename, true))
	{
		printf("Unable to open file.\n");

		return	false;
	}

	int	iFileSize	= pFile->getLength();

	m_iFinalSize	= iFileSize;

	pFile->close();

	int	iExtraBytes	= iFileSize % iBlockSize;

	if (iExtraBytes > 0)
	{
		m_iFinalSize	-= iExtraBytes;
		m_iFinalSize	+= iBlockSize;
	}

	SHAREDBUFFER(m_pFileBuffer, uint8_t, m_iFinalSize);

	if (nullptr == m_pFileBuffer)
	{
		printf("Unable to allocate memory for padded file.");

		return	false;
	}
	
	uint8_t*	pBuffer	= m_pFileBuffer.get();

	memset(pBuffer, 0, m_iFinalSize);

	if (false == pFile->open(_strFilename, true))
	{
		printf("Unable to read from file.\n");

		return	false;
	}

	pFile->readBuffer(&pBuffer, iFileSize);

	pFile->close();

	return	true;
}

void PadToBlock::close()
{
}

bool PadToBlock::save(const std::string& _strFilename)
{
	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		printf("Unable to create file interface.\n");

		return	false;
	}

	if (false == pFile->create(_strFilename, true))
	{
		printf("Unable to update source file.\n");

		return	false;
	}

	if (false == pFile->writeBuffer(m_pFileBuffer.get(), m_iFinalSize))
	{
		printf("Unable to write to source file.\n");

		return	false;
	}

	pFile->close();

	return	true;
}
