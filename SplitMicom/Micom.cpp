#include "File.h"
#include "Functions.h"
#include "Micom.h"

int	gsc_iFullRomSize	= 16 * 1024;	// Full rom size
 
Micom::Micom()	:
	m_iLength(0)
{
}

Micom::~Micom()
{
	close();
}

Micom::Ptr Micom::create(const std::string& _strFilename)
{
	INSTANCE(pMicom, Micom())

	if (false == pMicom->initialize(_strFilename))
	{
		pMicom.reset();
	}

	return	pMicom;
}

bool Micom::initialize(const std::string& _strFilename)
{
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

	m_iLength	= pFile->getLength();

	if (m_iLength != gsc_iFullRomSize)
	{
		printf("Input file must be %s bytes.\n", gsc_iFullRomSize);

		return	false;
	}

	SHAREDBUFFER(m_pFileBuffer, uint8_t, m_iLength);

	uint8_t*	pFileBuffer	= m_pFileBuffer.get();

	memset(pFileBuffer, 0, m_iLength);

	if (false == pFile->readBuffer(&pFileBuffer, m_iLength))
	{
		printf("Unable to read from file.\n");

		return	false;
	}

	pFile->close();

	return	true;
}

void Micom::close()
{
}

bool Micom::save(int _iRomSize, const std::string& _strFilename1, const std::string& _strFilename2, const std::string& _strFilename3, const std::string& _strFilename4)
{
	uint8_t*	pFileBuffer	= m_pFileBuffer.get();

	if (false == writeRom(_strFilename1, pFileBuffer, _iRomSize))
	{
		printf("Unable to write to file.\n");

		return	false;
	}

	pFileBuffer	+= _iRomSize;

	if (false == writeRom(_strFilename2, pFileBuffer, _iRomSize))
	{
		printf("Unable to write to file.\n");

		return	false;
	}

	pFileBuffer	+= _iRomSize;

	if (false == writeRom(_strFilename3, pFileBuffer, _iRomSize))
	{
		printf("Unable to write to file.\n");

		return	false;
	}

	pFileBuffer	+= _iRomSize;

	if (false == writeRom(_strFilename4, pFileBuffer, _iRomSize))
	{
		printf("Unable to write to file.\n");

		return	false;
	}

	return	true;
}

bool Micom::writeRom(const std::string& _strFilename, uint8_t* _pFileBuffer, int _iRomSize)
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

	if (false == pFile->writeBuffer(_pFileBuffer, _iRomSize))
	{
		printf("Unable to write to source file.\n");

		return	false;
	}

	pFile->close();

	return	true;
}
