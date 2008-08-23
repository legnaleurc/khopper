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
			 * @param [in] fileName File path
			 * @param [in] fileType File type specify by enum
			 */
			AudioFile( const std::string & filePath, FileType fileType );

			const std::string & getFilePath() const;

			const std::string & getFormat() const;

			std::string toString() const;

		private:
			std::string path_;
			std::string format_;
			FileType type_;
	};
}

#endif
