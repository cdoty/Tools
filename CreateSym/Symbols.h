#pragma once

#include "Macros.h"

class Symbols
{
	public:
		PTR(Symbols)

		// Constructor
		Symbols();

		// Destructor
		~Symbols();

		// Create
		static Ptr create(const std::string& _strFilename);

		// Initialize
		bool initialize(const std::string& _strFilename);

		// Close
		void close();

		// Save
		bool save(const std::string& _strFilename);

	private:
		struct Symbol
		{
			std::string	strName;		// Symbol name
			std::string	strFilename;	// Source filename
			uint32_t	address;		// Symbol address
		};

		SETTYPE(std::string, Path);
		SETTYPE(std::string, File);

		PathSet				m_paths;	// Paths
		FileSet				m_files;	// Files

		std::vector<Symbol>	m_symbols;	// Symbols

		// Load symbols
		bool loadSymbols(const std::string& _strFilename);

		// Parse symbol
		bool parseSymbol(const std::string& _strLine);

		// Find lists
		bool findLists();

		// Parse Symbols
		bool parseSymbols(const std::string& _strFilename, int _iStartAddress);

		// Remove the marker at the beginning of the file
		void removeMarker(std::string& _strString);

		// Find directories
		bool findDirectories(const std::string& _strDirectory);

		// Get file start address
		int getFileStartAddress(const std::string& _strFilename);

		// Check hex
		bool checkHex(const std::string& _strInstruction);
};
