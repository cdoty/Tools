#pragma once

#include "Macros.h"

class SetBootFile
{
	public:
		PTR(SetBootFile)

		// Constructor
		SetBootFile();

		// Destructor
		~SetBootFile();

		// Create
		static Ptr create();

		// Initialize
		bool initialize();

		// Close
		void close();

		// Set boot file
		bool setBootFile(const std::string& _strFilename, const std::string& _strBootFile);
};
