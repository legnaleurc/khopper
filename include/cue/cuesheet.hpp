/**
 * @file cuesheet.hpp
 */
#ifndef CUESHEET_HPP
#define CUESHEET_HPP

#include "track.hpp"

#include <QString>
#include <QMap>
#include <QVector>

namespace Khopper {

	class AudioFile;
	
	/**
	 * @brief The CUE parser
	 */
	class CUESheet {
		public:
			/**
			 * @brief Headers of fields
			 */
			static const QStringList headers;

			/**
			 * @brief Default constructor
			 */
			CUESheet();
			
			/**
			 * @brief Give a CUE Sheet content and create this object
			 * @param [in] content CHESheet content
			 * @param [in] dirPath where the CUE sheet comes from
			 */
			CUESheet( const QStringList & content, const QString & dirPath = QString() );

			/**
			 * @brief Set CUE sheet by given string vector
			 * @param [in] content CHESheet content
			 * @param [in] dirPath where the CUE sheet comes from
			 */
			void open( const QStringList & content, const QString & dirPath = QString() );

			/**
			 * @brief Get catalog infomation
			 * @return catalog
			 */
			const QString & getCatalog() const;
			
			/**
			 * @brief Get CD text infomation
			 * @return CD text
			 */
			const QString & getCDTextFile() const;
			
			/**
			 * @brief Get comments
			 * @return comments
			 */
			const QMap< QString, QString > & getComments() const;
			
			/**
			 * @brief Get garbage infomation
			 * @return garbage
			 */
			const QStringList & getGarbage() const;
			
			/**
			 * @brief Get performer infomation
			 * @return performer
			 */
			const QString & getPerformer() const;

			/**
			 * @brief Get song writer infomation
			 * @return song writer
			 */
			const QString & getSongWriter() const;
			
			/**
			 * @brief Get disc title infomation
			 * @return disc title
			 */
			const QString & getTitle() const;

			/**
			 * @brief Get track amount of disc
			 * @return track amount
			 */
			std::size_t size() const;

			/**
			 * @brief Dump CUE Sheet to string
			 * @return Human readable information
			 */
			QString toString() const;

			/**
			 * @brief Get track infomation
			 * @param [in] index track index
			 * @return track information
			 */
			const Track & operator []( std::size_t index ) const;
		private:
			void parseCUE_( const QStringList &, const QString & );
			void parseSingle_( const QString &, const QString &, int );
			AudioFile parseFile_( const QString &, const QString &, const QString & );
			void parseFlags_( const QString &, int );
			void parseIndex_( const QString &, const QString &, const QString &, const QString &, const QString &, int );
			void parseComment_( const QString &, const QString &, int );
			void parseTrack_( const QString &, const AudioFile &, const QString &, int );
			void parseGarbage_( const QString &, int );
			
			QString catalog_;
			QString cdTextFile_;
			QMap< QString, QString > comments_;
			QStringList garbage_;
			QString performer_;
			QString songWriter_;
			QString title_;
			QVector< Track > tracks_;
	};

}

#endif
