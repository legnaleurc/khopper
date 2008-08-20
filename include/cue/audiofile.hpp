#ifndef AUDIOFILE_HPP
#define AUDIOFILE_HPP

#include <string>

namespace Khopper {

	/**
	 * @brief Data structure of audio file
	 */
	class AudioFile {
		public:
			/**
			* @brief Type of audio file
			*/
			enum FileType {
				/// Binary
				BINARY,
				/// Motorola
				MOTOROLA,
				/// AIFF
				AIFF,
				/// wave
				WAVE,
				/// Mpeg 1 Layer 3
				MP3
			};

			/**
			* @brief Default constructor
			*/
			AudioFile();

			/**
			* @brief Initialize with file path and file type by string
			* @param [in] fileName File path
			* @param [in] fileType File type writen in string
			*/
			AudioFile( const std::string & fileName, const std::string & fileType );

			/**
			* @brief Initialize with file path and file type by enum
			* @param [in] fileName File path
			* @param [in] fileType File type specify by enum
			*/
			AudioFile( const std::string & fileName, FileType fileType );

			const std::string & getFileName() const;

		private:
			std::string name_;
			FileType type_;
	};
}

#endif
