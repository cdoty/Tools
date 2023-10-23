#pragma once

#include "Macros.h"

class PadFile
{
	public:
		PTR(PadFile)

		// Constructor
		PadFile();

		// Destructor
		~PadFile();

		// Create
		static Ptr create(const std::string& _strPadByte, const std::string& _strPadSize, const std::string& _strFilename);

		// Initialize
		bool initialize(const std::string& _strPadByte, const std::string& _strPadSize, const std::string& _strFilename);

		// Close
		void close();

		// Save
		bool save(const std::string& _strFilename);

	private:
		std::shared_ptr<uint8_t>	m_pFileBuffer;	// File buffer

		int	m_iPadByte;	// Pad byte
		int	m_iPadSize;	// Pad size
};
