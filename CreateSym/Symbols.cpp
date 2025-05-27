#define	_CRT_SECURE_NO_WARNINGS

#include "File.h"
#include "Functions.h"
#include "Symbols.h"

static const char*	gsc_szFileExtension	= ".sym";

Symbols::Symbols()
{
}

Symbols::~Symbols()
{
	close();
}

Symbols::Ptr Symbols::create(const std::string& _strFilename)
{
	INSTANCE(pSymbols, Symbols())

	if (false == pSymbols->initialize(_strFilename))
	{
		pSymbols.reset();
	}

	return	pSymbols;
}

bool Symbols::initialize(const std::string& _strFilename)
{
	if (false == loadSymbols(_strFilename))
	{
		return	false;
	}

	return	true;
}

void Symbols::close()
{
}

bool Symbols::save(const std::string& _strFilename)
{
	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		printf("Unable to create file interface.\n");

		return	false;
	}

	if (false == pFile->create(_strFilename))
	{
		printf("Unable to update source file.\n");

		return	false;
	}

	pFile->writeLine("[labels]");

	int	t_c	= (int)m_symbols.size();

	for (int iLoop = 0; iLoop < t_c; ++iLoop)
	{
		const Symbol&	symbol	= m_symbols[iLoop];

		char	szAddress[5];

		sprintf(szAddress, "%04x", (uint16_t)symbol.address);

		std::string	strSymbol;

		strSymbol	= "00:";
		strSymbol	+= szAddress;
		strSymbol	+= " ";
		strSymbol	+= symbol.strName;

		pFile->writeLine(strSymbol);
	}

	pFile->close();

	return	true;
}

bool Symbols::loadSymbols(const std::string& _strFilename)
{
	m_paths.clear();
	m_files.clear();
	m_symbols.clear();

	File::Ptr	pFile	= File::create();

	if (false == pFile->open(_strFilename, true))
	{
		printf("Unable to open file %s.\n", _strFilename.c_str());

		return	false;
	}

	std::vector<std::string>	lines;
	std::string					strRead;

	while (true == pFile->readLine(strRead))
	{
		if (false == strRead.empty() && strRead[0] != '=')
		{
			removeMarker(strRead);

			if (false == strRead.empty())
			{
				lines.push_back(strRead);
			}
		}
	}

	pFile->close();

	int	t_c	= (int)lines.size();

	if (0 == t_c)
	{
		printf("Symbol file does not contain any lines\n");

		return	false;
	}

	for (int iLoop = 0; iLoop < t_c; ++iLoop)
	{
		if (false == parseSymbol(lines[iLoop]))
		{
			return	false;
		}
	}

	std::sort(m_symbols.begin(), m_symbols.end(), [](const Symbol& _first, const Symbol& _second)
	{
		return	_second.address > _first.address;
	});

	if (false == findLists())
	{
		return	false;
	}

	return	true;
}

bool Symbols::parseSymbol(const std::string& _strLine)
{
	std::vector<std::string>&	strTokens	= Functions::tokenize(_strLine, ' ');

	strTokens.erase(std::remove_if(strTokens.begin(), strTokens.end(), [](const std::string& _string)
	{
		return	_string.empty();
	}), strTokens.end());

	if (3 == (int)strTokens.size())
	{
		if (true == checkHex(strTokens[1]))
		{
			Symbol	symbol;

			symbol.strName		= strTokens[0];
			symbol.address		= Functions::convertHexStringToUnsigned32(strTokens[1]);

			m_symbols.push_back(symbol);
		}
	}

	return	true;
}

bool Symbols::findLists()
{
	std::string	strDirectory;

	strDirectory	= ".";

	findDirectories(strDirectory);

	int	iDirectories	= (int)m_paths.size();

	for (PathIterator iterator = m_paths.begin(); iterator != m_paths.end(); ++iterator)
	{
		std::string	strDirectory;
		std::string	strPath;

		strDirectory	= *iterator;

		strPath	= strDirectory;
		strPath	+= "//*.lst";

		std::vector<std::string>	listFiles;

		if (false == File::findFiles(strPath, listFiles))
		{
			continue;
		}
	
		int	iListFiles	= (int)listFiles.size();

		for (int iListFile = 0; iListFile < iListFiles; ++iListFile)
		{
			std::string	strFilename;

			strFilename	= strDirectory;
			strFilename	+= "/";
			strFilename	+= listFiles[iListFile];

			int	iStartAddress	= getFileStartAddress(listFiles[iListFile]);

			if (-1 == iStartAddress || false == parseSymbols(strFilename, iStartAddress))
			{
				continue;
			}
		}
	}

	return	true;
}

bool Symbols::parseSymbols(const std::string& _strFilename, int _iStartAddress)
{
	File::Ptr	pFile	= File::create();

	if (false == pFile->open(_strFilename, true))
	{
		printf("Unable to open file %s.\n", _strFilename.c_str());

		return	false;
	}

	std::vector<std::string>	lines;
	std::string					strRead;

	while (true == pFile->readLine(strRead))
	{
		if ('=' == strRead[0])
		{
			continue;
		}
		
		removeMarker(strRead);

		if (false == strRead.empty())
		{
			lines.push_back(strRead);
		}
	}

	pFile->close();

	int	t_c	= (int)lines.size();

	if (0 == t_c)
	{
		printf("List file does not contain any lines\n");

		return	false;
	}

	return	true;
}

void Symbols::removeMarker(std::string& _strString)
{
	if (0xEF == (uint8_t)_strString[0])
	{
		_strString.erase(0, 1);
	}

	if (0xBB == (uint8_t)_strString[0])
	{
		_strString.erase(0, 1);
	}

	if (0xBF == (uint8_t)_strString[0])
	{
		_strString.erase(0, 1);
	}
}

bool Symbols::findDirectories(const std::string& _strDirectory)
{
	m_paths.insert(_strDirectory);

	std::vector<std::string>	directories;

	File::findDirectories(_strDirectory, directories);
	
	int	t_c	= (int)directories.size();

	for (int iLoop = 0; iLoop < t_c; ++iLoop)
	{
		std::string	strDirectory;

		strDirectory	= _strDirectory;
		strDirectory	+= "/";
		strDirectory	+= directories[iLoop];

		findDirectories(strDirectory);
	}

	return	true;
}

int Symbols::getFileStartAddress(const std::string& _strFilename)
{
	size_t	iObjOffset	= _strFilename.find_last_of(".lst");

	if (std::string::npos != iObjOffset)
	{
		std::string	strListName;

		strListName	= _strFilename.substr(0, iObjOffset - 3);
		strListName	+= ".obj";

		int	t_c	= (int)m_symbols.size();

		for (int iLoop = 0; iLoop < t_c; ++iLoop)
		{
			const Symbol&	symbol	= m_symbols[iLoop];

			if (strListName == symbol.strFilename)
			{
				return	symbol.address;
			}
		}
	}

	return	-1;
}

bool Symbols::checkHex(const std::string& _strInstruction)
{
	int	t_c	= (int)_strInstruction.length();

	for (int iLoop = 0; iLoop < t_c; ++iLoop)
	{
		if (false == isxdigit(_strInstruction[iLoop]))
		{
			return	false;
		}
	}

	return	true;
}
