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
		static Ptr create(const std::string& _strSpriteWidth, const std::string& _strFilename);

		// Initialize
		bool initialize(const std::string& _strSpriteWidth, const std::string& _strFilename);

		// Close
		void close();

		// Save
		bool save(const std::string& _strPrefix);

	private:
		std::vector<std::shared_ptr<uint8_t>>	m_patterns;			// Patterns

		uint8_t*	m_pBuffer;		// Buffer
		int			m_iSpriteSize;	// Sprite size
		int			m_iWidth;		// Width
		int			m_iHeight;		// Height

		// Process sprites
		bool processSprites();
		
		// Grab tile
		bool grabTile(uint8_t* _pBuffer, int _iX, int _iY);

		// Encode tile
		bool encodeTile(uint8_t* _pSrc);

		// Add sprite
		int addSprite(std::shared_ptr<uint8_t>& _pPattern);

		// Find palette
		int findPalette(int _iRed, int _iGreen, int _iBlue, int _iAlpha);

		// Buffers same?
		bool bufferSame(uint8_t* _pBuffer1, uint8_t* _pBuffer2, int _iBufferSize);
};
