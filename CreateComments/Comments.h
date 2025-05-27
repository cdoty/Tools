#pragma once

#include "Macros.h"

class Comments
{
	public:
		PTR(Comments)

		// Constructor
		Comments(const std::string& _strSystem);

		// Destructor
		~Comments();

		// Create
		static Ptr create(const std::string& _strSystem, const std::string& _strFilename, const std::string& _strBinaryFilename);

		// Initialize
		bool initialize(const std::string& _strFilename, const std::string& _strBinaryFilename);

		// Close
		void close();

		// Save
		bool save(const std::string& _strDirectory, const std::string& _strCPUTag);

	private:
		struct Symbol
		{
			std::string	strName;		// Symbol name
			std::string	strFilename;	// Source filename
			uint32_t	address;		// Symbol address
		};

		struct Comment
		{
			std::string	strLine;	// Line
			uint32_t	address;	// Symbol address
			uint32_t	crc;		// CRC
		};

		SETTYPE(std::string, Path);
		SETTYPE(std::string, File);

		SHAREDTYPE(uint8_t)		m_pBinaryBuffer;	// Binary buffer

		std::string				m_strSystem;	// System
		uint32_t				m_csegStart;	// CSEG start
		uint32_t				m_csegEnd;		// CSEG end
		std::vector<Symbol>		m_symbols;		// Symbols
		PathSet					m_paths;		// Paths
		FileSet					m_files;		// Files
		std::vector<Comment>	m_comments;		// Comments

		// Load binary
		bool loadBinary(const std::string& _strFilename);

		// Load symbols
		bool loadSymbols(const std::string& _strFilename);

		// Parse symbol
		bool parseSymbol(const std::string& _strLine);

		// Find lists
		bool findLists();

		// Find CSEG
		bool findCSEG(const std::string& _strFilename);

		// Parse comments
		bool parseComments(const std::string& _strFilename, int _iStartAddress);

		// Parse comment
		bool parseComment(const std::string& _strLine, Comment& _comment, int _iStartAddress, int& _iCurrentAddress);

		// Remove the marker at the beginning of the file
		void removeMarker(std::string& _strString);

		// Get file start address
		int getFileStartAddress(const std::string& _strFilename);

		// Check hex
		bool checkHex(const std::string& _strInstruction);

		// Calculate CRC
		uint32_t calculateCRC(int _iOffset, int _iLength);

		// Find directories
		bool findDirectories(const std::string& _strDirectory);
};
