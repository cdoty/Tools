#pragma once

#include "Macros.h"

class Aquarius
{
	public:
		PTR(Aquarius)

		// Constructor
		Aquarius();

		// Destructor
		~Aquarius();

		// Create
		static Ptr create(const std::string& _strFilename);

		// Initialize
		bool initialize(const std::string& _strFilename);

		// Close
		void close();

		// Save
		bool save(int _iSpriteWidth, int _iSpriteHeight, int _iSpriteCount, const std::string& _strPrefix);
		
	private:
		static const int	msc_iScreenWidth	= 40;	// Size of aquarius screen, in tiles
		static const int	msc_iScreenHeight	= 25;

		std::shared_ptr<uint8_t>	m_pScreenBuffer;		// Screen buffer
		std::shared_ptr<uint8_t>	m_pScreenColorBuffer;	// Screen color buffer

		std::shared_ptr<uint8_t>	m_pSpriteBuffer;		// Sprite buffer
		std::shared_ptr<uint8_t>	m_pSpriteColorBuffer;	// Sprite color buffer

		// Load screen
		bool loadScreen(const std::string& _strFilename);

		// Extract sprites
		bool extractSprites(int _iSpriteWidth, int _iSpriteHeight, int _iSpriteCount);

		// Extract sprite
		bool extractSprite(int _iStartX, int _iStartY, int _iSpriteWidth, int _iSpriteHeight, uint8_t* _pSpriteBuffer, uint8_t* _pColorBuffer);
};
