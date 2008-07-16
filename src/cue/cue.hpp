/**
 * @file cue.hpp
 */
#ifndef CUE_HPP
#define CUE_HPP

#include "error.hpp"

#include <string>
#include <vector>
#include <utility>
#include <boost/regex.hpp>
#include <boost/format.hpp>

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
				/**
				 * @brief Headers of fields
				 * 
				 * This is an NULL terminated char * array.
				 */
				static const char * const Header[];
			};
			
			/**
			 * @brief Give a CUE Sheet path name and create this object
			 * @param [in] filename path of CUE sheet
			 */
			CUESheet( const std::string & filename );
			
			/**
			 * @brief Get the sheet path
			 * @todo May be I don't want this return type.
			 */
			const std::pair< std::string, std::string > & getSheetName() const;
			
			/**
			 * @brief Get the audio path
			 * @todo May be I don't want this return type.
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
			std::pair< std::string, std::string > _sheetName_;
			std::pair< std::string, std::string > _audioName_;
			FieldType _discInfo_;
			std::vector< FieldType > _trackInfo_;
			
			// protection
			CUESheet();
	};

}

#endif
