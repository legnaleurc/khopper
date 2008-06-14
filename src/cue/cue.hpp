#ifndef CUE_HPP
#define CUE_HPP

#include <string>
#include <vector>
#include <utility>

namespace Khopper {
	
	class CUESheet {
		public:
			typedef std::vector< std::string > FieldType;
			
			struct Disc {
				enum Field {
					Arranger,
					Composer,
					Genre,
					Message,
					Tracks,
					Performer,
					Songwriter,
					Title,
					UPC_EAN,
					SIZE
				};
			};
			
			struct Track {
				enum Field {
					Arranger,
					Composer,
					Genre,
					ISRC,
					Message,
					Number,
					Performer,
					Title,
					CD_Text,
					SIZE
				};
				static const char * const Header[];
			};
			
			CUESheet( const std::string & filename );
			const std::pair< std::string, std::string > & getSheetName() const;
			const std::pair< std::string, std::string > & getAudioName() const;
			const FieldType & getDiscInfo() const;
			const std::vector< FieldType > & getTrackInfo() const;
		private:
			std::pair< std::string, std::string > _sheetName_;
			std::pair< std::string, std::string > _audioName_;
			FieldType _discInfo_;
			std::vector< FieldType > _trackInfo_;
	};

}

#endif
