#include "File.h"
#include "Functions.h"
#include "Micom.h"

static const int	gsc_iAquariusWidth	= 320 / 8;
static const int	gsc_iMicomWidth		= 256 / 8;
static const int	gsc_iHeight			= 192 / 8;

Micom::Micom()
{
}

Micom::~Micom()
{
	close();
}

Micom::Ptr Micom::create(const std::string& _strOrigin, const std::string& _strFilename)
{
	INSTANCE(pMicom, Micom())

	if (false == pMicom->initialize(_strOrigin, _strFilename))
	{
		pMicom.reset();
	}

	return	pMicom;
}

bool Micom::initialize(const std::string& _strOrigin, const std::string& _strFilename)
{
	SHAREDBUFFER(m_pFileBuffer, uint8_t, gsc_iAquariusWidth * gsc_iHeight);

	uint8_t*	pAquariusBuffer	= m_pFileBuffer.get();

	memset(pAquariusBuffer, 0, gsc_iAquariusWidth * gsc_iHeight);

	SHAREDBUFFER(m_pWriteBuffer, uint8_t, gsc_iMicomWidth * gsc_iHeight);

	uint8_t*	pMicomBuffer	= m_pWriteBuffer.get();

	memset(pMicomBuffer, 0, gsc_iMicomWidth * gsc_iHeight);

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

	if (false == pFile->readBuffer(&pAquariusBuffer, gsc_iAquariusWidth * gsc_iHeight))
	{
		printf("Unable to read from file.\n");

		return	false;
	}

	pFile->close();

	if (false == createMicomBuffer(_strFilename, _strOrigin))
	{
		printf("Unable to convert buffer.\n");

		return	false;
	}

	return	true;
}

void Micom::close()
{
}

bool Micom::save(const std::string& _strFilename)
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

	if (false == pFile->writeBuffer(m_pWriteBuffer.get(), gsc_iMicomWidth * gsc_iHeight))
	{
		printf("Unable to write to source file.\n");

		return	false;
	}

	pFile->close();

	return	true;
}

bool Micom::createMicomBuffer(const std::string& _strFilename, const std::string& _strOrigin)
{
	Origin	origin	= parseOrigin(_strOrigin);

	int	iStartX	= 0;

	if (Origin::Center == origin)
	{
		iStartX	= (gsc_iAquariusWidth - gsc_iMicomWidth) / 2;
	}

	else if (Origin::Right == origin)
	{
		iStartX	= (gsc_iAquariusWidth - gsc_iMicomWidth);
	}

	uint8_t*	pAquariusBuffer	= m_pFileBuffer.get() + iStartX;
	uint8_t*	pMicomBuffer	= m_pWriteBuffer.get();

	for (int iLoop = 0; iLoop < gsc_iHeight; ++iLoop)
	{
		memcpy(pMicomBuffer, pAquariusBuffer, gsc_iMicomWidth);

		pAquariusBuffer	+= gsc_iAquariusWidth;
		pMicomBuffer	+= gsc_iMicomWidth;
	}

	return	true;
}

Micom::Origin Micom::parseOrigin(const std::string& _strOrigin)
{
	if ("Left" == _strOrigin)
	{
		return	Origin::Left;
	}

	else if ("Right" == _strOrigin)
	{
		return	Origin::Right;
	}

	return	Origin::Center;
}
