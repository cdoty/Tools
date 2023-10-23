#pragma once

#include "Macros.h"

class Functions
{
	public:
		// Convert string to lower case
		static void convertToLowerCase(std::string& _strString);

		// Convert string to upper case
		static void convertToUpperCase(std::string& _strString);

		// Convert int string to int
		static int convertStringToInt(const std::string& _strString);

		// Convert float string to float
		static float convertStringToFloat(const std::string& _strString);

		// Convert hex string to 32 bit unsigned int
		static uint32_t convertHexStringToUnsigned32(const std::string& _strString);

		// Convert hex string to 64 bit unsigned int
		static uint64_t convertHexStringToUnsigned64(const std::string& _strString);

		// Convert string to bool
		static bool convertStringToBool(const std::string& _strString);

		// Convert bool to string
		static std::string convertBoolToString(bool _bValue);

		// Delete string from string
		static void deleteString(std::string& _strSource, const std::string& _strDelete);
		static void deleteString(std::wstring& _strSource, const std::wstring& _strDelete);

		// Get nearest higher power of two
		static uint32_t getNextPowerOfTwo(uint32_t _value);
};
