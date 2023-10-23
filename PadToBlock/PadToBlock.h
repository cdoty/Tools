#pragma once

#include "Macros.h"

class PadToBlock
{
	public:
		PTR(PadToBlock)

		// Constructor
		PadToBlock();

		// Destructor
		~PadToBlock();

		// Create
		static Ptr create(const std::string& _strBlockSize, const std::string& _strFilename);

		// Initialize
		bool initialize(const std::string& _strBlockSize, const std::string& _strFilename);

		// Close
		void close();

		// Save
		bool save(const std::string& _strFilename);

	private:
		int	m_iFinalSize;	// Final file size

		std::shared_ptr<uint8_t>	m_pFileBuffer;	// File buffer
};
