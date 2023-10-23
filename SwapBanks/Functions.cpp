// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>

#include "File.h"
#include "Functions.h"

void Functions::convertToLowerCase(std::string& _strString)
{
	if (false == _strString.empty())
	{
		std::transform(_strString.begin(), _strString.end(), _strString.begin(), ::tolower);
	}
}

void Functions::convertToUpperCase(std::string& _strString)
{
	if (false == _strString.empty())
	{
		std::transform(_strString.begin(), _strString.end(), _strString.begin(), ::toupper);
	}
}

int Functions::convertStringToInt(const std::string& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			return	std::stoi(_strString);
		}

		catch (const std::exception& _exception)
		{
			printf("convertStringToInt: Error %s\n", _exception.what());
		}
	}

	return	0;
}

float Functions::convertStringToFloat(const std::string& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			return	(float)(std::stod(_strString, NULL));
		}

		catch (const std::exception& _exception)
		{
			printf("convertStringToFloat: Error %s\n", _exception.what());
		}
	}

	return	0.0f;
}

uint32_t Functions::convertHexStringToUnsigned32(const std::string& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			std::string	strTemp;
			int			iBase = 16;

			if (_strString.find("0x") == _strString.npos && _strString.find("0X") == _strString.npos)
			{
				strTemp	= "0x";
			}

			strTemp	+= _strString;

			return	std::stoul(strTemp, NULL, iBase);
		}

		catch (const std::exception& _exception)
		{
			printf("convertHexStringToUnsigned32: Error %s\n", _exception.what());
		}
	}

	return	0;
}

uint64_t Functions::convertHexStringToUnsigned64(const std::string& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			std::string	strTemp;
			int			iBase = 16;

			if (_strString.find("0x") == _strString.npos && _strString.find("0X") == _strString.npos)
			{
				strTemp	= "0x";
			}

			strTemp	+= _strString;

			return	std::stoull(strTemp, NULL, iBase);
		}

		catch (const std::exception& _exception)
		{
			printf("convertHexStringToUnsigned64: Error %s\n", _exception.what());
		}
	}

	return	0;
}

bool Functions::convertStringToBool(const std::string& _strString)
{
	std::string	strTemp;

	if (false == _strString.empty())
	{
		strTemp	= _strString;

		convertToLowerCase(strTemp);
	}

	return	"true" == strTemp;
}

std::string Functions::convertBoolToString(bool _bValue)
{
	return	true == _bValue ? "true" : "false";
}

void Functions::deleteString(std::string& _strSource, const std::string& _strDelete)
{
	if (false == _strSource.empty())
	{
		size_t	strPos	= _strSource.find(_strDelete);

		if (strPos != std::string::npos)
		{
			_strSource.erase(strPos, _strDelete.length());
		}
	}
}
