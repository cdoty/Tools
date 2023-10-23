#pragma once

#include "Macros.h"

class BinFile
{
	public:
		PTR(BinFile)

		// Constructor
		BinFile();

		// Destructor
		~BinFile();

		// Create
		static Ptr create(const std::string& _strFilename);

		// Initialize
		bool initialize(const std::string& _strFilename);

		// Close
		void close();

		// Save
		bool save(const std::string& _strFilename);

	private:
		std::shared_ptr<uint8_t>	m_pFileBuffer;	// File buffer
		int							m_iFileLength;	// File length

		// Write value
		void writeValue(std::string& _strValue, uint8_t _uValue, bool _bEndLine);

		// Write length
		void writeLength(std::string& _strValue, uint16_t _uValue, bool _bEndLine);
};
