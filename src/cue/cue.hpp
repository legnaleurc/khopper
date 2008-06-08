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
					Perfomer,
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
					Perfomer,
					Title,
					CD_Text,
					SIZE
				};
				static const char * const Header[];
			};
			
			CUESheet( const std::string & filename );
			std::pair< std::string, std::string > getSheetName() const;
			std::pair< std::string, std::string > getAudioName() const;
			FieldType getDiscInfo() const;
			std::vector< FieldType > getTrackInfo() const;
		private:
			std::pair< std::string, std::string > _sheetName_;
			std::pair< std::string, std::string > _audioName_;
			FieldType _discInfo_;
			std::vector< FieldType > _TrackInfo_;
	};

}

#endif
