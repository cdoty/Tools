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
		bool save(const std::string& _strPrefix);
		
	private:
		static const int	msc_iScreenWidth	= 40;	// Size of aquarius screen, in tiles
		static const int	msc_iScreenHeight	= 25;

		std::shared_ptr<uint8_t>	m_pScreenBuffer;	// Screen buffer
		std::shared_ptr<uint8_t>	m_pColorBuffer;		// Color buffer

		// Load screen
		bool loadScreen(const std::string& _strFilename);
};
