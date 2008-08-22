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
			 * @brief Initialize track type form DataType
			 * @param [in] trackNO Track number
			 * @param [in] type Track type
			 */
			Track( unsigned short int trackNO, const AudioFile & data );
			
			void addComment( const std::string & key, const std::string & value );
			
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
			
			void addGarbage( const std::string & gi );
			
			const AudioFile & getAudioData() const;

			std::string getIndicesString() const;
			
			const std::string & getPerformer() const;

			const std::string & getTitle() const;
			
			void setAudioData( const AudioFile & audio );

			void setBeginIndex( const Index & begin );
			
			void setDataType( DataType type );

			void setEndIndex( const Index & end );

			void setISRC( const std::string & isrc );

			void setPerformer( const std::string & performer );

			void setPostGap( const Index & post );

			void setPreGap( const Index & pre );

			void setSongWriter( const std::string & songWriter );

			void setTitle( const std::string & title );

			std::string toString() const;

		private:
			unsigned short int number_;
			Index beginIndex_;
			std::map< std::string, std::string > comments_;
			AudioFile dataFile_;
			DataType dataType_;
			Index endIndex_;
			std::set< Flag > flags_;
			std::vector< std::string > garbage_;
			std::string isrc_;
			std::string path_;
			std::string performer_;
			Index postGap_;
			Index preGap_;
			std::string songWriter_;
			std::string title_;
	};

}

#endif
