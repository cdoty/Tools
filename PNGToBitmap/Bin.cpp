#include <algorithm>

#include "Bin.h"
#include "File.h"
#include "Palette.h"
#include "stb_image.h"

static const int	gsc_iTileSize			= 8;		// Tile size, must be a power of two
static const int	gsc_iTileLimit			= 256;		// Tile limit
static const char*	gsc_szPatternExtension	= ".pat";	// Pattern extension;
static const char*	gsc_szColorExtension	= ".col";	// Color extension;
static const char*	gsc_szScreenExtension	= ".scn";	// Screen extension;

Bin::Bin()	:
	m_pBuffer(NULL),
	m_iWidth(0),
	m_iHeight(0),
	m_iStartY(0),
	m_iEndY(0)
{
}

Bin::~Bin()
{
	close();
}

Bin::Ptr Bin::create(const std::string& _strFilename, const std::string& _strStartY, const std::string& _strEndY, const std::string& _strTiles, int _iStartingTile)
{
	INSTANCE(pBin, Bin())

	if (false == pBin->initialize(_strFilename, _strStartY, _strEndY, _strTiles, _iStartingTile))
	{
		pBin.reset();
	}

	return	pBin;
}

bool Bin::initialize(const std::string& _strFilename, const std::string& _strStartY, const std::string& _strEndY, const std::string& _strTiles, int _iStartingTile)
{
	m_iStartY	= std::stoi(_strStartY);
	m_iEndY		= std::stoi(_strEndY);
	
	int	iBpp;

	m_pBuffer	= stbi_load(_strFilename.c_str(), &m_iWidth, &m_iHeight, &iBpp, 4);

	if (NULL == m_pBuffer)
	{
		printf("Unable to load image %s.\n", _strFilename.c_str());
	
		return	false;
	}

	if (m_iStartY >= m_iHeight || m_iEndY > m_iHeight)
	{
		printf("Start or end Y is invalid.\n");
	
		return	false;
	}

	if (false == createScreen())
	{
		return	false;
	}

	if (false == _strTiles.empty())
	{
		if (_iStartingTile > 0)
		{
			if (false == loadPatterns(_strTiles))
			{
				return	false;
			}
		}

		m_strTiles	= _strTiles;
	}

	if (false == processTiles())
	{
		return	false;
	}

	return	true;
}

void Bin::close()
{
	if (m_pBuffer != NULL)
	{
		stbi_image_free(m_pBuffer);

		m_pBuffer	= NULL;
	}
}

bool Bin::loadPatterns(const std::string& _strPrefix)
{
	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		printf("Unable to create file interface.\n");

		return	false;
	}

	std::string	strFilename;

	strFilename	= _strPrefix;
	strFilename	+= gsc_szPatternExtension;

	if (false == pFile->open(strFilename, true))
	{
		return	true;
	}

	int	iTiles	= pFile->getLength() / gsc_iTileSize;

	for (int iLoop = 0; iLoop < iTiles; ++iLoop)
	{
		DEFINESHAREDBUFFER(pBuffer, uint8_t, gsc_iTileSize);

		uint8_t*	pPatterns	= pBuffer.get();

		if (false == pFile->readBuffer(&pPatterns, gsc_iTileSize))
		{
			printf("Unable to read from pattern file.\n");

			return	false;
		}

		m_patterns.push_back(pBuffer);
	}

	pFile->close();

	strFilename	= _strPrefix;
	strFilename	+= gsc_szColorExtension;

	if (false == pFile->open(strFilename, true))
	{
		printf("Unable to load color file %s.\n", strFilename.c_str());

		return	false;
	}

	int	iColors	= pFile->getLength() / gsc_iTileSize;

	for (int iLoop = 0; iLoop < iColors; ++iLoop)
	{
		DEFINESHAREDBUFFER(pBuffer, uint8_t, gsc_iTileSize);

		uint8_t*	pColors	= pBuffer.get();

		if (false == pFile->readBuffer(&pColors, gsc_iTileSize))
		{
			printf("Unable to read from color file.\n");

			return	false;
		}

		m_colors.push_back(pBuffer);
	}

	pFile->close();

	return	true;
}

bool Bin::save(const std::string& _strPrefix, int _iStartingTile, bool _bSavePatterns)
{
	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		printf("Unable to create file interface.\n");

		return	false;
	}

	std::string	strFilename;

	if (true == _bSavePatterns)
	{
		int	iTileCount	= (int)m_patterns.size();

		if (iTileCount > gsc_iTileLimit)
		{
			printf("Too many tiles. Bin files are limited to %d.\n", gsc_iTileLimit);

			return	false;
		}

		strFilename	= m_strTiles;
		strFilename	+= gsc_szPatternExtension;

		if (false == pFile->create(strFilename, true))
		{
			printf("Unable to create pattern file %s.\n", strFilename.c_str());

			return	false;
		}

		int	t_c	= (int)m_patterns.size();

		for (int iLoop = 0; iLoop < t_c; ++iLoop)
		{
			// Write pattern
			if (false == pFile->writeBuffer(m_patterns[iLoop].get(), gsc_iTileSize))
			{
				printf("Unable to write pattern data.\n");

				return	false;
			}
		}
			
		pFile->close();

		strFilename	= m_strTiles;
		strFilename	+= gsc_szColorExtension;

		if (false == pFile->create(strFilename, true))
		{
			printf("Unable to create color file %s.\n", strFilename.c_str());

			return	false;
		}

		t_c	= (int)m_colors.size();

		for (int iLoop = 0; iLoop < t_c; ++iLoop)
		{
			// Write color
			if (false == pFile->writeBuffer(m_colors[iLoop].get(), gsc_iTileSize))
			{
				printf("Unable to write color data.\n");

				return	false;
			}
		}
			
		pFile->close();

		printf("Wrote %d patterns.\n", iTileCount);
	}

	strFilename	= _strPrefix;
	strFilename	+= gsc_szScreenExtension;

	if (false == pFile->create(strFilename, true))
	{
		printf("Unable to create screen file %s.\n", strFilename.c_str());

		return	false;
	}

	int	iLength	= (m_iWidth / gsc_iTileSize) * ((m_iEndY - m_iStartY) / gsc_iTileSize);

	uint8_t*	pScreenBuffer	= m_pScreenBuffer.get();

	for (int iLoop = 0; iLoop < iLength; ++iLoop)
	{
		pScreenBuffer[iLoop]	+= _iStartingTile;
	}

	// Write screen
	if (false == pFile->writeBuffer(pScreenBuffer, iLength))
	{
		printf("Unable to write screen data.\n");

		return	false;
	}
			
	pFile->close();

	return	true;
}

bool Bin::processTiles()
{
	if ((m_iWidth & (gsc_iTileSize - 1)) != 0 || ((m_iEndY - m_iStartY) & (gsc_iTileSize - 1)) != 0)
	{
		printf("File width and selected height must be a multiple of %d.\n", gsc_iTileSize);

		return	false;
	}
	
	DEFINESHAREDBUFFER(pTile, uint8_t, gsc_iTileSize * gsc_iTileSize);

	uint8_t*	pScreen	= m_pScreenBuffer.get();

	for (int iYLoop = m_iStartY; iYLoop < m_iEndY; iYLoop += gsc_iTileSize)
	{
		for (int iXLoop = 0; iXLoop < m_iWidth; iXLoop += gsc_iTileSize)
		{
			int	iTileIndex	= 0;

			grabTile(pTile.get(), iXLoop, iYLoop);
			
			if (true == encodeTile(pTile.get(), iTileIndex))
			{
				*pScreen	= iTileIndex;

				pScreen++;
			}
		}
	}

	return	true;
}

bool Bin::createScreen()
{
	int	iSize	= (m_iWidth / gsc_iTileSize) * ((m_iEndY - m_iStartY) / gsc_iTileSize);

	SHAREDBUFFER(m_pScreenBuffer, uint8_t, iSize);

	return	true;
}

bool Bin::grabTile(uint8_t* _pBuffer, int _iX, int _iY)
{
	uint8_t*	pSource	= m_pBuffer	+ ((_iY * m_iWidth + _iX) * 4);
	uint8_t*	pDest	= _pBuffer;

	for (int iYLoop = 0; iYLoop < gsc_iTileSize; ++iYLoop)
	{
		for (int iXLoop = 0; iXLoop < gsc_iTileSize; ++iXLoop)
		{
			uint8_t	red		= *(pSource++);
			uint8_t	green	= *(pSource++);
			uint8_t	blue	= *(pSource++);
			uint8_t	alpha	= *(pSource++);

			int	iPalette	= findPalette(red, green, blue, alpha);

			*pDest	= iPalette;

			pDest++;
		}

		pSource	+= (m_iWidth - gsc_iTileSize) * 4;
	}

	return	true;
}

bool Bin::encodeTile(uint8_t* _pSrc, int& _iTileIndex)
{
	uint8_t*	pSrc	= _pSrc;

	DEFINESHAREDBUFFER(pPatternBuffer, uint8_t, gsc_iTileSize);
	DEFINESHAREDBUFFER(pColorBuffer, uint8_t, gsc_iTileSize);

	memset(pPatternBuffer.get(), 0, gsc_iTileSize);
	memset(pColorBuffer.get(), 0, gsc_iTileSize);

	uint8_t*	pPattern	= pPatternBuffer.get();
	uint8_t*	pColor		= pColorBuffer.get();

	for (int iYLoop = 0; iYLoop < gsc_iTileSize; ++iYLoop)
	{
		*pPattern	= 0;
			
		std::vector<uint8_t>	colors;
		int						iFirstPalette	= -1;

		for (int iXLoop = 0; iXLoop < gsc_iTileSize; ++iXLoop)
		{
			uint8_t	pixel	= *(pSrc++);

			if (-1 == iFirstPalette)
			{
				iFirstPalette	= pixel;
			}

			if (pixel != iFirstPalette)
			{
				*pPattern	|= 1 << (7 - iXLoop);
			}

			if (colors.end() == std::find(colors.begin(), colors.end(), pixel))
			{
				colors.push_back(pixel);
			}
		}

		pPattern++;

		int	iColors	= colors.size();

		if (iColors > 2)
		{
			printf("Too many colors in tile.\n");

			return	false;
		}

		if (1 == iColors)
		{
			*pColor	= colors[0] & 0x0F | 0xF0;
		}

		else
		{
			*pColor	= ((colors[1] & 0x0F) << 4);
		}

		pColor++;
	}

	_iTileIndex	= addTile(pPatternBuffer, pColorBuffer);
	 
	return	true;
}

int Bin::addTile(std::shared_ptr<uint8_t>& _pPattern, std::shared_ptr<uint8_t>& _pColor)
{
	int	t_c	= (int)m_patterns.size();

	m_patterns.push_back(_pPattern);
	m_colors.push_back(_pColor);

	return	t_c;
}

int Bin::findPalette(int _iRed, int _iGreen, int _iBlue, int _iAlpha)
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
