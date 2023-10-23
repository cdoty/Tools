#pragma once

#include "Macros.h"

class PatchAdamBoot
{
	public:
		PTR(PatchAdamBoot)

		// Constructor
		PatchAdamBoot();

		// Destructor
		~PatchAdamBoot();

		// Create
		static Ptr create(const std::string& _strBaseFile);

		// Initialize
		bool initialize(const std::string& _strBaseFile);

		// Close
		void close();

		// Patch boot file
		bool patchBootFile(const std::string& _strBootFile);

	private:
		std::shared_ptr<uint8_t>	m_pFileBuffer;	// File buffer

		int	m_iBlocks;	// Blocks in base file
};
