#pragma once

#include <random>

class Functions
{
	public:
		static std::random_device	ms_randomDevice;	// Random device

		// Convert RGBA to ARGB
		static void convertRGBAToARGB(const uint32_t* _pSource, uint32_t* _pDestination, int _iPixels, bool _bTransparent);

		// String hash
		static int getHash(const std::string& _strString);
	
		// Convert to big endian
		static uint16_t convertToBigEndian(uint16_t _value);
		static uint32_t convertToBigEndian(uint32_t _value);

		// Convert string to lower case
		static void convertToLowerCase(std::string& _strString);
		static void convertToLowerCase(std::wstring& _strString);

		// Convert string to upper case
		static void convertToUpperCase(std::string& _strString);
		static void convertToUpperCase(std::wstring& _strString);

		// Convert int string to int
		static int convertStringToInt(const std::string& _strString);
		static int convertStringToInt(const std::wstring& _strString);

		// Convert float string to float
		static float convertStringToFloat(const std::string& _strString);
		static float convertStringToFloat(const std::wstring& _strString);

		// Convert hex string to 32 bit unsigned int
		static uint32_t convertHexStringToUnsigned32(const std::string& _strString);
		static uint32_t convertHexStringToUnsigned32(const std::wstring& _strString);

		// Convert hex string to 64 bit unsigned int
		static uint64_t convertHexStringToUnsigned64(const std::string& _strString);
		static uint64_t convertHexStringToUnsigned64(const std::wstring& _strString);

		// Convert string to bool
		static bool convertStringToBool(const std::string& _strString);
		static bool convertStringToBool(const std::wstring& _strString);

		// Convert bool to string
		static std::string convertBoolToString(bool _bValue);
		static std::wstring convertBoolToWideString(bool _bValue);

		// Delete string from string
		static void deleteString(std::string& _strSource, const std::string& _strDelete);
		static void deleteString(std::wstring& _strSource, const std::wstring& _strDelete);

		// Remove quotes
		static void removeQuotes(std::string& _strSource);
		static void removeQuotes(std::wstring& _strSource);

		// Get nearest higher power of two
		static uint32_t getNextPowerOfTwo(uint32_t _value);

		// Tokenize
		static std::vector<std::string> tokenize(const std::string& _strSource, char _delimiter);
		static std::vector<std::wstring> tokenize(const std::wstring& _strSource, wchar_t _delimiter);

		// INI String
		static void createINIString(std::string& _strINI, const std::string& _strName, const std::string& _strValue, const std::string& _strPrefix = "");
		static void createINIString(std::string& _strINI, const std::string& _strName, int _iValue, const std::string& _strPrefix = "");
		static void createINIString(std::string& _strINI, const std::string& _strName, float _fValue, const std::string& _strPrefix = "", bool _bTruncate = true);
		static void createINIString(std::string& _strINI, const std::string& _strName, bool _bValue, const std::string& _strPrefix = "");

		// Get next lowest
		static float getNextLowest(float _fValue);

		// Get next highest
		static float getNextHighest(float _fValue);
};
