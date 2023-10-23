#pragma once

#include "Macros.h"

class Micom
{
	public:
		PTR(Micom)

		// Constructor
		Micom();

		// Destructor
		~Micom();

		// Create
		static Ptr create(const std::string& _strFilename);

		// Initialize
		bool initialize(const std::string& _strFilename);

		// Close
		void close();

		// Save
		bool save(int _iRomSize, const std::string& _strFilename1, const std::string& _strFilename2, const std::string& _strFilename3, const std::string& _strFilename4);

	private:
		std::shared_ptr<uint8_t>	m_pFileBuffer;	// File buffer
		int							m_iLength;		// File length

		// Write rom
		bool writeRom(const std::string& _strFilename, uint8_t* _pFileBuffer, int _iRomSize);
};
