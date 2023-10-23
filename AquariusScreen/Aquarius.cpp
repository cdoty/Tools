#include "Aquarius.h"
#include "File.h"
#include "jsonCpp\json.h"

static const int	gsc_iTileLimit			= 256;		// Tile limit
static const char*	gsc_szMapExtension		= ".bin";	// Map extension;
static const char*	gsc_szPaletteExtension	= ".pal";	// Palette extension;
static const char*	gsc_szScreenExtension	= ".scn";	// Screen extension;
static const char*	gsc_szColorExtension	= ".col";	// Color extension;

Aquarius::Aquarius()
{
}

Aquarius::~Aquarius()
{
	close();
}

Aquarius::Ptr Aquarius::create(const std::string& _strFilename)
{
	INSTANCE(pAquarius, Aquarius())

	if (false == pAquarius->initialize(_strFilename))
	{
		pAquarius.reset();
	}

	return	pAquarius;
}

bool Aquarius::initialize(const std::string& _strFilename)
{
	SHAREDBUFFER(m_pScreenBuffer, uint8_t, msc_iScreenWidth * msc_iScreenHeight);

	if (nullptr == m_pScreenBuffer)
	{
		printf("Unable to allocate memory for the screen buffer.");

		return	false;
	}

	memset(m_pScreenBuffer.get(), 0, msc_iScreenWidth * msc_iScreenHeight);

	SHAREDBUFFER(m_pColorBuffer, uint8_t, msc_iScreenWidth * msc_iScreenHeight);

	if (nullptr == m_pColorBuffer)
	{
		printf("Unable to allocate memory for the color buffer.");

		return	false;
	}

	memset(m_pColorBuffer.get(), 0, msc_iScreenWidth * msc_iScreenHeight);

	if (false == loadScreen(_strFilename))
	{
		return	false;
	}

	return	true;
}

void Aquarius::close()
{
}

bool Aquarius::save(const std::string& _strPrefix)
{
	int	iLength	= msc_iScreenWidth * msc_iScreenHeight;

	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		printf("Unable to create file interface.\n");

		return	false;
	}

	std::string	strFilename;

	strFilename	= _strPrefix;
	strFilename	+= gsc_szScreenExtension;

	if (false == pFile->create(strFilename, true))
	{
		printf("Unable to create screen file %s.\n", strFilename.c_str());

		return	false;
	}

	// Write screen
	if (false == pFile->writeBuffer(m_pScreenBuffer.get(), iLength))
	{
		printf("Unable to write screen data.\n");

		return	false;
	}
			
	pFile->close();

	strFilename	= _strPrefix;
	strFilename	+= gsc_szColorExtension;

	if (false == pFile->create(strFilename, true))
	{
		printf("Unable to create color file %s.\n", strFilename.c_str());

		return	false;
	}

	// Write color
	if (false == pFile->writeBuffer(m_pColorBuffer.get(), iLength))
	{
		printf("Unable to write color data.\n");

		return	false;
	}
			
	pFile->close();

	return	true;
}

bool Aquarius::loadScreen(const std::string& _strFilename)
{
	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		printf("Unable to create file interface.\n");

		return	false;
	}

	if (false == pFile->open(_strFilename, true))
	{
		printf("Unable to open screen file.\n");

		return	false;
	}

	std::string	strJSON;

	if (false == pFile->readString(strJSON))
	{
		printf("Unable to read from screen file.\n");

		return	false;
	}

	pFile->close();

	int	iCharacters	= strJSON.length();

	Json::Value		root;
	Json::Reader	reader;
			
	reader.parse(strJSON, root);

	Json::Value	entries	= root;

	if (entries != Json::Value::nullRef && true == entries.isArray())
	{
		Json::ArrayIndex	t_c	= entries.size();

		uint8_t*	screenBuffer	= m_pScreenBuffer.get();
		uint8_t*	colorBuffer		= m_pColorBuffer.get();

		for (Json::ArrayIndex iLoop = 0; iLoop < t_c; ++iLoop)
		{
			Json::Value	character	= entries.get(iLoop, Json::Value::nullRef);

			if (character != Json::Value::nullRef && true == character.isObject())
			{
				Json::Value	index		= character.get("index", Json::Value::nullRef);
				Json::Value	bgColor		= character.get("bg", Json::Value::nullRef);
				Json::Value	fgColor		= character.get("fg", Json::Value::nullRef);
				Json::Value	charTile	= character.get("char", Json::Value::nullRef);

				if (index != Json::Value::nullRef && bgColor != Json::Value::nullRef && bgColor != Json::Value::nullRef && charTile != Json::Value::nullRef)
				{
					int	iIndex	= index.asInt();

					if (iIndex >= 0 && iIndex < msc_iScreenWidth * msc_iScreenHeight)
					{
						screenBuffer[iIndex]	= charTile.asInt();
						
						int	iFGColor	= fgColor.asInt() & 0x0F;
						int	iBGColor	= bgColor.asInt() & 0x0F;

						colorBuffer[iIndex]		= (iFGColor << 4) | iBGColor;
					}
				}
			}
		}
	}

	return	true;
}
