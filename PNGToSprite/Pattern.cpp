#include <algorithm>

#include "File.h"
#include "Palette.h"
#include "Pattern.h"
#include "stb_image.h"

static const int	gsc_iTileLimit			= 256;		// Tile limit
static const char*	gsc_szPatternExtension	= ".spr";	// Sprite extension;

Pattern::Pattern()	:
	m_pBuffer(NULL),
	m_iSpriteSize(0),
	m_iWidth(0),
	m_iHeight(0)
{
}

Pattern::~Pattern()
{
	close();
}

Pattern::Ptr Pattern::create(const std::string& _strSpriteWidth, const std::string& _strFilename)
{
	INSTANCE(pPattern, Pattern())

	if (false == pPattern->initialize(_strSpriteWidth, _strFilename))
	{
		pPattern.reset();
	}

	return	pPattern;
}

bool Pattern::initialize(const std::string& _strSpriteWidth, const std::string& _strFilename)
{
	m_iSpriteSize	= std::stoi(_strSpriteWidth);

	if (m_iSpriteSize != 8 && m_iSpriteSize != 16)
	{
		printf("Sprite width must be 8 or 16.\n");
	
		return	false;
	}

	int	iBpp;

	m_pBuffer	= stbi_load(_strFilename.c_str(), &m_iWidth, &m_iHeight, &iBpp, 4);

	if (NULL == m_pBuffer)
	{
		printf("Unable to load image %s.\n", _strFilename.c_str());
	
		return	false;
	}

	if (false == processSprites())
	{
		return	false;
	}

	return	true;
}

void Pattern::close()
{
	if (m_pBuffer != NULL)
	{
		stbi_image_free(m_pBuffer);

		m_pBuffer	= NULL;
	}
}

bool Pattern::save(const std::string& _strPrefix)
{
	int	iTileCount	= (int)m_patterns.size();

	if (iTileCount > gsc_iTileLimit)
	{
		printf("Too many tiles. Pattern files are limited to %d.\n", gsc_iTileLimit);

		return	false;
	}

	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		printf("Unable to create file interface.\n");

		return	false;
	}

	std::string	strFilename;

	strFilename	= _strPrefix;
	strFilename	+= gsc_szPatternExtension;

	if (false == pFile->create(strFilename, true))
	{
		printf("Unable to create pattern file %s.\n", strFilename.c_str());

		return	false;
	}

	for (int iLoop = 0; iLoop < iTileCount; ++iLoop)
	{
		// Write pattern
		if (false == pFile->writeBuffer(m_patterns[iLoop].get(), m_iSpriteSize * m_iSpriteSize / 8))
		{
			printf("Unable to write pattern data.\n");

			return	false;
		}
	}
			
	pFile->close();

	printf("Wrote %d patterns.\n", iTileCount);

	return	true;
}

bool Pattern::processSprites()
{
	if ((m_iWidth & (m_iSpriteSize - 1)) != 0 || (m_iHeight & (m_iSpriteSize - 1)) != 0)
	{
		printf("File width and selected height must be a multiple of %d.\n", m_iSpriteSize);

		return	false;
	}
	
	DEFINESHAREDBUFFER(pTile, uint8_t, m_iSpriteSize * m_iSpriteSize);

	for (int iYLoop = 0; iYLoop < m_iHeight; iYLoop += m_iSpriteSize)
	{
		for (int iXLoop = 0; iXLoop < m_iWidth; iXLoop += m_iSpriteSize)
		{
			grabTile(pTile.get(), iXLoop, iYLoop);
			
			encodeTile(pTile.get());
		}
	}

	return	true;
}

bool Pattern::grabTile(uint8_t* _pBuffer, int _iX, int _iY)
{
	uint8_t*	pSource	= m_pBuffer	+ ((_iY * m_iWidth + _iX) * 4);
	uint8_t*	pDest	= _pBuffer;

	for (int iYLoop = 0; iYLoop < m_iSpriteSize; ++iYLoop)
	{
		for (int iXLoop = 0; iXLoop < m_iSpriteSize; ++iXLoop)
		{
			uint8_t	red		= *(pSource++);
			uint8_t	green	= *(pSource++);
			uint8_t	blue	= *(pSource++);
			uint8_t	alpha	= *(pSource++);

			int	iPalette	= findPalette(red, green, blue, alpha);

			*pDest	= iPalette;

			pDest++;
		}

		pSource	+= (m_iWidth - m_iSpriteSize) * 4;
	}

	return	true;
}

bool Pattern::encodeTile(uint8_t* _pSrc)
{
	uint8_t*	pSrc	= _pSrc;

	DEFINESHAREDBUFFER(pPatternBuffer, uint8_t, m_iSpriteSize * m_iSpriteSize / 8);

	memset(pPatternBuffer.get(), 0, m_iSpriteSize);

	uint8_t*	pPattern	= pPatternBuffer.get();

	int	iTilesWide	= m_iSpriteSize / 8;

	for (int iXLoop = 0; iXLoop < iTilesWide; ++iXLoop)
	{
		uint8_t*	pSrc	= _pSrc + iXLoop * m_iSpriteSize;

		for (int iYLoop = 0; iYLoop < m_iSpriteSize; ++iYLoop)
		{
			*pPattern	= 0;
			
			for (int iPixel = 0; iPixel < 8; ++iPixel)
			{
				uint8_t	pixel	= *(pSrc++);

				if (pixel != 0)
				{
					*pPattern	|= 1 << (7 - iPixel);
				}
			}

			pSrc	+= m_iSpriteSize - 8;

			pPattern++;
		}
	}

	addSprite(pPatternBuffer);
	 
	return	true;
}

int Pattern::addSprite(std::shared_ptr<uint8_t>& _pPattern)
{
#if 0
	int	t_c	= (int)m_patterns.size();

	for (int iLoop = 0; iLoop < t_c; ++iLoop)
	{
		if (true == bufferSame(_pPattern.get(), m_patterns[iLoop].get(), m_iSpriteSize * m_iSpriteSize / 8))
		{
			return	iLoop;
		}
	}
#endif

	m_patterns.push_back(_pPattern);

	return	0;
}

int Pattern::findPalette(int _iRed, int _iGreen, int _iBlue, int _iAlpha)
{
	if (0 == _iAlpha)
	{
		return	0;
	}

	for (int iLoop = 0; iLoop < gsc_iColors; ++iLoop)
	{
		if (_iRed == gsc_palette[iLoop].red && _iGreen == gsc_palette[iLoop].green && _iBlue == gsc_palette[iLoop].blue)
		{
			return	iLoop;
		}
	}

	printf("Unable to find color %02X %02X %02X.\n", _iRed, _iGreen, _iBlue);

	return	0;
}

bool Pattern::bufferSame(uint8_t* _pBuffer1, uint8_t* _pBuffer2, int _iBufferSize)
{
	uint8_t*	pBuffer1Ptr	= _pBuffer1;
	uint8_t*	pBuffer2Ptr	= _pBuffer2;

	for (int iLoop = 0; iLoop < _iBufferSize; ++iLoop)
	{
		if (*pBuffer1Ptr != *pBuffer2Ptr)
		{
			return	false;
		}

		pBuffer1Ptr++;
		pBuffer2Ptr++;
	}

	return	true;
}