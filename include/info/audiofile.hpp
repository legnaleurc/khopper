#ifndef KHOPPER_INFO_AUDIOFILE_HPP
#define KHOPPER_INFO_AUDIOFILE_HPP

#include <QString>

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
			AudioFile( const QString & filePath, FileType fileType );

			/**
			 * @brief Get audio file path
			 * @return File path
			 */
			const QString & getFilePath() const;

			/**
			 * @brief Get audio format
			 * @return format string
			 */
			const QString & getFormat() const;

			/**
			 * @brief String representation
			 * @return A formated content string
			 */
			QString toString() const;

		private:
			QString path_;
			FileType type_;
			QString format_;
	};
}

#endif
