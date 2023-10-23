#include "File.h"
#include "Functions.h"
#include "PadFile.h"

PadFile::PadFile()	:
	m_iPadByte(0),
	m_iPadSize(0)
{
}

PadFile::~PadFile()
{
	close();
}

PadFile::Ptr PadFile::create(const std::string& _strPadByte, const std::string& _strPadSize, const std::string& _strFilename)
{
	INSTANCE(pPadFile, PadFile())

	if (false == pPadFile->initialize(_strPadByte, _strPadSize, _strFilename))
	{
		pPadFile.reset();
	}

	return	pPadFile;
}

bool PadFile::initialize(const std::string& _strPadByte, const std::string& _strPadSize, const std::string& _strFilename)
{
	if (_strPadByte.find("0x") != std::string::npos)
	{
		m_iPadByte	= (int)Functions::convertHexStringToUnsigned32(_strPadByte);
	}

	else
	{
		m_iPadByte	= Functions::convertStringToInt(_strPadByte);
	}

	if (_strPadSize.find("0x") != std::string::npos)
	{
		m_iPadSize	= (int)Functions::convertHexStringToUnsigned32(_strPadSize);
	}

	else
	{
		m_iPadSize	= Functions::convertStringToInt(_strPadSize);
	}

	if (0 == m_iPadSize)
	{
		printf("Pad size cannot be zero.\n");

		return	false;
	}

	SHAREDBUFFER(m_pFileBuffer, uint8_t, m_iPadSize);

	uint8_t*	pBuffer	= m_pFileBuffer.get();

	memset(pBuffer, m_iPadByte, m_iPadSize);

	if (true == File::fileExists(_strFilename))
	{
		File::Ptr	pFile	= File::create();

		if (nullptr == pFile)
		{
			printf("Unable to create file interface.\n");

			return	false;
		}

		if (true == pFile->open(_strFilename, true))
		{
			pFile->readBuffer(&pBuffer, m_iPadSize);

			pFile->close();
		}
	}

	return	true;
}

void PadFile::close()
{
}

bool PadFile::save(const std::string& _strFilename)
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

	if (false == pFile->writeBuffer(m_pFileBuffer.get(), m_iPadSize))
	{
		printf("Unable to write to source file.\n");

		return	false;
	}

	pFile->close();

	return	true;
}
