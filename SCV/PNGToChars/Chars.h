#pragma once

#include "Macros.h"

class Chars
{
	public:
		PTR(Chars)

		// Destructor
		~Chars();

		// Create
		static Ptr create(const std::string& _strFilename);

		// Initialize
		bool initialize(const std::string& _strFilename);

		// Close
		void close();

		// Save
		bool save(const std::string& _strFilename);

	private:
		std::vector<std::shared_ptr<uint8_t>>	m_chars;	// Chars
		int										m_iDepth;	// Depth

		// Constructor
		Chars();

		// Load chars
		bool loadChars(const std::string& _strFilename);

		// Extract tiles
		bool extractTiles(uint8_t* _pSource, int _iWidth, int _iHeight);

		// Extract tile
		bool extractTile(uint8_t* _pSource, int _iWidth, int _iHeight, uint8_t* _pChar, int _iX, int _iY);
};
