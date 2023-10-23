#include "BinFile.h"
#include "File.h"

BinFile::BinFile()	:
	m_iFileLength(0)
{
}

BinFile::~BinFile()
{
	close();
}

BinFile::Ptr BinFile::create(const std::string& _strFilename)
{
	INSTANCE(pBinFile, BinFile())

	if (false == pBinFile->initialize(_strFilename))
	{
		pBinFile.reset();
	}

	return	pBinFile;
}

bool BinFile::initialize(const std::string& _strFilename)
{
	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		printf("Unable to create file interface.\n");

		return	false;
	}

	if (false == pFile->open(_strFilename, true))
	{
		printf("Unable to open file %s.\n", _strFilename.c_str());

		return	false;
	}

	m_iFileLength	= pFile->getLength();

	if (0 == m_iFileLength)
	{
		printf("File %s contains no data.\n", _strFilename.c_str());

		return	false;
	}

	SHAREDBUFFER(m_pFileBuffer, uint8_t, m_iFileLength);

	if (false == pFile->readBuffer(m_pFileBuffer.get(), m_iFileLength))
	{
		printf("Unable to read from file %s.\n", _strFilename.c_str());

		return	false;
	}

	pFile->close();

	return	true;
}

void BinFile::close()
{
}

bool BinFile::save(const std::string& _strFilename)
{
	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		printf("Unable to create file interface.\n");

		return	false;
	}

	if (false == pFile->create(_strFilename, false))
	{
		printf("Unable to create file %s.\n", _strFilename.c_str());

		return	false;
	}

	if (false == pFile->writeLine("cseg"))
	{
		printf("Unable to write to file %s.\n", _strFilename.c_str());

		return	false;
	}

	std::string	strBase	= File::getFilenameFromPath(_strFilename);
	std::string	strName	= strBase + "_";

	std::string	strValue;

	strValue	= strName;
	strValue	+= ": public ";
	strValue	+= strName;

	if (false == pFile->writeLine(strValue))
	{
		printf("Unable to write to file %s.\n", _strFilename.c_str());

		return	false;
	}

	for (int iLoop = 0; iLoop < m_iFileLength; ++iLoop)
	{
		if (0 == (iLoop & 7))
		{
			strValue	= "\tdb ";

			writeValue(strValue, m_pFileBuffer.get()[iLoop], iLoop >= m_iFileLength - 1);
		}

		else if (7 == (iLoop & 7))
		{
			strValue.clear();

			writeValue(strValue, m_pFileBuffer.get()[iLoop], true);
		}

		else
		{
			strValue.clear();

			writeValue(strValue, m_pFileBuffer.get()[iLoop], iLoop >= m_iFileLength - 1);
		}

		if (false == pFile->writeString(strValue))
		{
			printf("Unable to write to file %s.\n", _strFilename.c_str());

			return	false;
		}
	}

	strName	= strBase + "Length_";

	strValue	= "\n";
	strValue	+= strName;
	strValue	+= ": public ";
	strValue	+= strName;

	if (false == pFile->writeLine(strValue))
	{
		printf("Unable to write to file %s.\n", _strFilename.c_str());

		return	false;
	}

	strValue	= "\tdw ";

	writeLength(strValue, m_iFileLength & 0xFFFF, true);

	if (false == pFile->writeString(strValue))
	{
		printf("Unable to write to file %s.\n", _strFilename.c_str());

		return	false;
	}

	pFile->close();

	return	true;
}

void BinFile::writeValue(std::string& _strValue, uint8_t _uValue, bool _bEndLine)
{
	char	szValue[4];
			
	sprintf_s(szValue, "%02X", _uValue);

	_strValue	+= "$";
	_strValue	+= szValue;

	if (false == _bEndLine)
	{
		_strValue	+= ", ";
	}

	else
	{
		_strValue	+= "\n";
	}
}

void BinFile::writeLength(std::string& _strValue, uint16_t _uValue, bool _bEndLine)
{
	char	szValue[6];
			
	sprintf_s(szValue, "%04X", _uValue);

	_strValue	+= "$";
	_strValue	+= szValue;

	if (false == _bEndLine)
	{
		_strValue	+= ", ";
	}

	else
	{
		_strValue	+= "\n";
	}
}
