#ifndef TRACK_HPP
#define TRACK_HPP

#include "cue/audiofile.hpp"

#include <set>
#include <vector>
#include <string>
#include <map>

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
			 * @brief Convert to std::string
			 * @return mm:ss.ff
			 */
			std::string toString() const;

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
			 * @brief Initialize track type form string
			 * @param [in] trackNO Track number
			 * @param [in] type Track type (written in string)
			 *
			 * The string will be parsing to DataType.
			 */
			Track( unsigned short int trackNO, const std::string & type );

			/**
			 * @brief Initialize track type form DataType
			 * @param [in] trackNO Track number
			 * @param [in] type Track type
			 */
			Track( unsigned short int trackNO, DataType type );

			std::string getIndicesString() const;

			const std::string & getTitle() const;

			const std::string & getPerformer() const;

			/**
			 * @brief Add flag information form string
			 * @param [in] f Flag string
			 */
			void addFlag( const std::string & f );

			/**
			 * @brief Add flag information
			 * @param [in] f Flag enum
			 */
			void addFlag( Flag f );

			void addComment( const std::string & key, const std::string & value );

			void setAudioData( const AudioFile & audio );
			
			const AudioFile & getAudioData() const;

			void addGarbage( const std::string & gi );

			void setBeginIndex( const Index & begin );

			void setEndIndex( const Index & end );

			void setISRC( const std::string & isrc );

			void setPerformer( const std::string & performer );

			void setPostGap( const Index & post );

			void setPreGap( const Index & pre );

			void setSongWriter( const std::string & songWriter );

			void setTitle( const std::string & title );

			void setDataType( DataType type );

		private:
			unsigned short int number_;
			DataType dataType_;
			std::map< std::string, std::string > comments_;
			AudioFile dataFile_;
			std::vector< std::string > garbage_;
			Index beginIndex_;
			Index endIndex_;
			std::string isrc_;
			std::string performer_;
			Index postGap_;
			Index preGap_;
			std::string songWriter_;
			std::string title_;
			std::set< Flag > flags_;
	};

}

#endif
