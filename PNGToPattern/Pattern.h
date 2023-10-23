#pragma once

#include "Macros.h"

class Pattern
{
	public:
		PTR(Pattern)

		// Constructor
		Pattern();

		// Destructor
		~Pattern();

		// Create
		static Ptr create(const std::string& _strFilename);

		// Initialize
		bool initialize(const std::string& _strFilename);

		// Close
		void close();

		// Save
		bool save(const std::string& _strPrefix);

	private:
		std::vector<std::shared_ptr<uint8_t>>	m_patterns;			// Patterns
		std::vector<std::shared_ptr<uint8_t>>	m_colors;			// Colors

		uint8_t*	m_pBuffer;			// Buffer
		int			m_iWidth;			// Width
		int			m_iHeight;			// Height

		// Process tiles
		bool processTiles();
		
		// Grab tile
		bool grabTile(uint8_t* _pBuffer, int _iX, int _iY);

		// Encode tile
		bool encodeTile(uint8_t* _pSrc, int& _iTileIndex);

		// Add tile
		int addTile(std::shared_ptr<uint8_t>& _pPattern, std::shared_ptr<uint8_t>& _pColor);

		// Find palette
		int findPalette(int _iRed, int _iGreen, int _iBlue, int _iAlpha);

		// Buffers same?
		bool bufferSame(uint8_t* _pBuffer1, uint8_t* _pBuffer2, int _iBufferSize);
};
