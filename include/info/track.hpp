#ifndef KHOPPER_INFO_TRACK_HPP
#define KHOPPER_INFO_TRACK_HPP

#include "audiofile.hpp"

#include <QString>
#include <QStringList>
#include <QSet>
#include <QMap>

namespace Khopper {

	/**
	 * @brief Track index information
	 */
	class Index {
		public:
			/**
			 * @brief Default constructor
			 *
			 * Initialize to 0:0.0.
			 */
			Index();

			/**
			 * @brief Custom constructor
			 * @param [in] m minute
			 * @param [in] s second
			 * @param [in] f frame
			 *
			 * Initialize to m:s.f.
			 */
			Index( unsigned short int m, unsigned short int s, unsigned short int f );

			/**
			 * @brief Get minute
			 * @return minutes
			 */
			unsigned short int getMinute() const {
				return minute_;
			};
			/**
			 * @brief Get second
			 * @return seconds
			 */
			unsigned short int getSecond() const {
				return second_;
			}
			/**
			 * @brief Get frame
			 * @return frames
			 */
			unsigned short int getFrame() const {
				return frame_;
			}

			/**
			 * @brief Convert to QString
			 * @return mm:ss.ff
			 */
			QString toString() const;

		private:
			/// minite
			unsigned short int minute_;
			/// second
			unsigned short int second_;
			/// frame
			unsigned short int frame_;
	};
	
	/**
	 * @brief Track information
	 */
	class Track {
		public:
			/// Track type
			enum DataType {
				/// AUDIO
				AUDIO,
				/// CDG
				CDG,
				/// MODE1/2048
				MODE1_2048,
				/// MODE1/2352
				MODE1_2352,
				/// MODE1/2336
				MODE2_2336,
				/// MODE1/2352
				MODE2_2352,
				/// CDI/2336
				CDI_2336,
				/// CDI/2352
				CDI_2352
			};

			/// Track flag
			enum Flag {
				/// DCP
				DCP,
				/// CH4
				CH4,
				/// PRE
				PRE,
				/// SCMS
				SCMS,
				/// DATA
				DATA,
				/// NONE
				NONE
			};

			Track();

			/**
			 * @brief Initialize track type form DataType
			 * @param [in] trackNO Track number
			 * @param [in] data Audio information
			 */
			Track( unsigned short int trackNO, const AudioFile & data );

			/**
			 * @brief Add comment
			 * @param [in] key Comment key
			 * @param [in] value Comment value
			 */
			void addComment( const QString & key, const QString & value );
			
			/**
			 * @brief Add flag information
			 * @param [in] f Flag enum
			 */
			void addFlag( Flag f );

			/**
			 * @brief Add garbage information
			 * @param [in] gi Gargage string
			 */
			void addGarbage( const QString & gi );

			/**
			 * @brief Get audio information
			 * @return AudioFile object
			 */
			const AudioFile & getAudioData() const;

			/**
			 * @brief Get formated indices string
			 * @return a string
			 *
			 * The format is:
			 * MM:SS.FF
			 * MM:SS.FF
			 */
			QString getIndicesString() const;

			/**
			 * @brief Get track number
			 * @return track number
			 */
			unsigned short int getNumber() const;

			/**
			 * @brief Get performer
			 * @return performer
			 */
			const QString & getPerformer() const;

			/**
			 * @brief Get title
			 * @return title
			 */
			const QString & getTitle() const;

			/**
			 * @brief Set audio information
			 * @param audio audio information
			 */
			void setAudioData( const AudioFile & audio );

			/**
			 * @brief Set begin index
			 * @param begin track start point
			 */
			void setBeginIndex( const Index & begin );

			/**
			 * @brief Set track type
			 * @param type track data
			 */
			void setDataType( DataType type );

			/**
			 * @brief Set end index
			 * @param end track end point
			 */
			void setEndIndex( const Index & end );

			/**
			 * @brief Set ISRC
			 * @param isrc ISRC
			 */
			void setISRC( const QString & isrc );

			/**
			 * @brief Set performer
			 * @param performer performer
			 */
			void setPerformer( const QString & performer );

			/**
			 * @brief Set Postgap
			 * @param post postgap index
			 */
			void setPostGap( const Index & post );

			/**
			 * @brief Set Pregap
			 * @param pre pregap index
			 */
			void setPreGap( const Index & pre );

			/**
			 * @brief Set song writer
			 * @param songWriter song writer
			 */
			void setSongWriter( const QString & songWriter );

			/**
			 * @brief Set track title
			 * @param title title
			 */
			void setTitle( const QString & title );

			/**
			 * @brief Dump track information
			 * @return Formated string
			 */
			QString toString() const;
			
			/**
			 * @brief Headers of fields
			 */
			static const QStringList headers;

		private:
			unsigned short int number_;
			Index beginIndex_;
			QMap< QString, QString > comments_;
			AudioFile dataFile_;
			DataType dataType_;
			Index endIndex_;
			QSet< Flag > flags_;
			QStringList garbage_;
			QString isrc_;
			QString path_;
			QString performer_;
			Index postGap_;
			Index preGap_;
			QString songWriter_;
			QString title_;
	};

}

#endif
