#include "Chars.h"
#include "Defines.h"
#include "File.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Chars::Chars()	:
	m_iDepth(0)
{
}

Chars::~Chars()
{
	close();
}

Chars::Ptr Chars::create(const std::string& _strFilename)
{
	INSTANCE(pChars, Chars())

	if (false == pChars->initialize(_strFilename))
	{
		pChars.reset();
	}

	return	pChars;
}

bool Chars::initialize(const std::string& _strFilename)
{
	if (false == loadChars(_strFilename))
	{
		printf("Unable to load chars.\n");

		return	false;
	}

	return	true;
}

void Chars::close()
{
}

bool Chars::save(const std::string& _strFilename)
{
	int	t_c	= (int)m_chars.size();

	if (0 == t_c)
	{
		printf("No chars defined.\n");

		return	false;
	}

	File::Ptr	pFile	= File::create();

	if (false == pFile->create(_strFilename, true))
	{
		printf("Unable to create file %s/\n", _strFilename.c_str());

		return	false;
	}

	for (int iLoop = 0; iLoop < t_c; ++iLoop)
	{
		pFile->writeBuffer(m_chars[iLoop].get(), gsc_iCharWidth / 8 * gsc_iCharHeight);
	}

	pFile->close();

	return	true;
}

bool Chars::loadChars(const std::string& _strFilename)
{
	int	iWidth	= 0;
	int	iHeight	= 0;

	uint8_t*	pPixels	= stbi_load(_strFilename.c_str(), &iWidth, &iHeight, &m_iDepth, 0);

	if (NULL == pPixels)
	{
		printf("Unable to find or load char image.\n");

		return	false;
	}

	if ((iWidth % gsc_iCharWidth) != 0 || (iHeight % gsc_iCharHeight) != 0)
	{
		printf("Image must be a multiple of %d width and %d height.\n", gsc_iCharWidth, gsc_iCharHeight);

		return	false;
	}

	if (false == extractTiles(pPixels, iWidth, iHeight))
	{
		return	false;
	}

	stbi_image_free(pPixels);

	return	true;
}

bool Chars::extractTiles(uint8_t* _pSource, int _iWidth, int _iHeight)
{
	int	iCharWidth	= _iWidth / gsc_iCharWidth;
	int	iCharHeight	= _iHeight / gsc_iCharHeight;

	DEFINESHAREDBUFFER(pCharBuffer, uint8_t, gsc_iCharWidth * gsc_iCharHeight);

	for (int iYLoop = 0; iYLoop < iCharHeight; ++iYLoop)
	{
		for (int iXLoop = 0; iXLoop < iCharWidth; ++iXLoop)
		{
			if (false == extractTile(_pSource, _iWidth, _iHeight, pCharBuffer.get(), iXLoop * gsc_iCharWidth, iYLoop * gsc_iCharHeight))
			{
				printf("Unable to extract char\n");

				return	false;
			}

			DEFINESHAREDBUFFER(pFinalBuffer, uint8_t, gsc_iCharWidth / 8 * gsc_iCharHeight);

			uint8_t*	pSrc	= pCharBuffer.get();
			uint8_t*	pDest	= pFinalBuffer.get();

			for (int iTileY = 0; iTileY < gsc_iCharHeight; ++iTileY)
			{
				*pDest	= 0;

				for (int iTileX = 0; iTileX < gsc_iCharWidth; ++iTileX)
				{
					if (*pSrc > 0)
					{
						*pDest	|= (1 << (7 - iTileX));
					}

					pSrc++;
				}

				pDest++;
			}

			m_chars.push_back(pFinalBuffer);
		}
	}

	return	true;
}

bool Chars::extractTile(uint8_t* _pSource, int _iWidth, int _iHeight, uint8_t* _pChar, int _iX, int _iY)
{
	uint8_t*	pSrc	= _pSource + (_iY * _iWidth + _iX) * m_iDepth;
	uint8_t*	pDest	= _pChar;

	for (int iYLoop = 0; iYLoop < gsc_iCharHeight; ++iYLoop)
	{
		for (int iXLoop = 0; iXLoop < gsc_iCharWidth; ++iXLoop)
		{
			uint8_t	red		= *(pSrc++);
			uint8_t	green	= *(pSrc++);
			uint8_t	blue	= *(pSrc++);

			if (4 == m_iDepth)
			{
				pSrc++;
			}

			bool	bPixel	= (red > 0 || blue > 0 || green > 0);

			*pDest	= true == bPixel ? 1 : 0;

			pDest++;
		}

		pSrc	+= (_iWidth - gsc_iCharWidth) * m_iDepth;
	}

	return	true;
}
