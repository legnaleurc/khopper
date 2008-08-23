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
			 * @brief Initialize with file path and file type by enum
			 * @param [in] filePath File path
			 * @param [in] fileType File type specify by enum
			 */
			AudioFile( const std::string & filePath, FileType fileType );

			/**
			 * @brief Get audio file path
			 * @return File path
			 */
			const std::string & getFilePath() const;

			/**
			 * @brief Get audio format
			 * @return format string
			 */
			const std::string & getFormat() const;

			/**
			 * @brief String representation
			 * @return A formated content string
			 */
			std::string toString() const;

		private:
			std::string path_;
			std::string format_;
			FileType type_;
	};
}

#endif
