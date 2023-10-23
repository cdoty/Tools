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
		static Ptr create(const std::string& _strOrigin, const std::string& _strFilename);

		// Initialize
		bool initialize(const std::string& _strOrigin, const std::string& _strFilename);

		// Close
		void close();

		// Save
		bool save(const std::string& _strFilename);

	private:
		enum class Origin
		{
			Left	= 0,
			Center,
			Right,
		};

		std::shared_ptr<uint8_t>	m_pFileBuffer;	// File buffer
		std::shared_ptr<uint8_t>	m_pWriteBuffer;	// Write buffer

		// Create micom buffer
		bool createMicomBuffer(const std::string& _strFilename, const std::string& _strOrigin);

		// Parse origin
		Origin parseOrigin(const std::string& _strOrigin);
};
