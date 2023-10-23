#pragma once

#include "Macros.h"

class Bin
{
	public:
		PTR(Bin)

		// Constructor
		Bin();

		// Destructor
		~Bin();

		// Create
		static Ptr create(const std::string& _strFilename, const std::string& _strStartY, const std::string& _strEndY, const std::string& _strTiles);

		// Initialize
		bool initialize(const std::string& _strFilename, const std::string& _strStartY, const std::string& _strEndY, const std::string& _strTiles);

		// Close
		void close();

		// Load patterns
		bool loadPatterns(const std::string& _strPrefix);

		// Save
		bool save(const std::string& _strPrefix, int _iStartingTile, bool _bSavePatterns);
		
	private:
		std::shared_ptr<uint8_t>				m_pScreenBuffer;	// Screen buffer
		std::vector<std::shared_ptr<uint8_t>>	m_patterns;			// Patterns
		std::vector<std::shared_ptr<uint8_t>>	m_colors;			// Colors

		std::string	m_strTiles;			// Tile filename
		uint8_t*	m_pBuffer;			// Buffer
		int			m_iWidth;			// Width
		int			m_iHeight;			// Height
		int			m_iStartY;			// Start y
		int			m_iEndY;			// End y

		// Process tiles
		bool processTiles();
		
		// Create screen
		bool createScreen();

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
