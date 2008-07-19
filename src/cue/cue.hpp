/**
 * @file cue.hpp
 */
#ifndef CUE_HPP
#define CUE_HPP

#include <string>
#include <vector>
#include <utility>

namespace Khopper {

	/**
	 * @brief The CUE parser
	 */
	class CUESheet {
		public:
			/**
			 * @brief Fields of each track or disc
			 */
			typedef std::vector< std::string > FieldType;
			
			/**
			 * @brief Information of disc
			 *
			 * This is simply a place holder class.
			 */
			struct Disc {
				/// Disc fields
				enum Field {
					/// arranger
					Arranger,
					/// composer
					Composer,
					/// genre
					Genre,
					/// message
					Message,
					/// tracks
					Tracks,
					/// performer
					Performer,
					/// song writer
					Songwriter,
					/// title
					Title,
					/// upc ean
					UPC_EAN,
					/// Field's amount
					SIZE
				};
			};
			
			/**
			 * @brief Information of tracks
			 *
			 * This is simply a place holder class.
			 */
			struct Track {
				/// Track fileds
				enum Field {
					/// arranger
					Arranger,
					/// composer
					Composer,
					/// genre
					Genre,
					/// ISRC
					ISRC,
					/// message
					Message,
					/// number
					Number,
					/// performer
					Performer,
					/// title
					Title,
					/// cd text
					CD_Text,
					/// Field's amount
					SIZE
				};
				/// Headers of fields
				static const char * const Header[];
			};
			
			/**
			 * @brief Construct with a CUE sheet
			 * @param [in] filename path of CUE sheet
			 */
			CUESheet( const std::string & filename );
			
			/**
			 * @brief Get the sheet path
			 */
			const std::pair< std::string, std::string > & getSheetName() const;
			
			/**
			 * @brief Get the audio path
			 */
			const std::pair< std::string, std::string > & getAudioName() const;
			
			/**
			 * @brief Get the disc infomation
			 * @return Fields that contains information of disc
			 */
			const FieldType & getDiscInfo() const;
			
			/**
			 * @brief Get the track infomation
			 * @return An array of fields that contains information of tracks
			 */
			const std::vector< FieldType > & getTrackInfo() const;
		private:
			std::pair< std::string, std::string > sheetName_;
			std::pair< std::string, std::string > audioName_;
			FieldType discInfo_;
			std::vector< FieldType > trackInfo_;
	};

}

#endif
