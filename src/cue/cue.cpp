#include "cue.hpp"

namespace Khopper {

	const char * const CUESheet::Track::Header[] = {
		"Arranger",
		"Composer",
		"Genre",
		"ISRC",
		"Message",
		"Number",
		"Perfomer",
		"Title",
		"CD_Text",
		NULL
	};

	CUESheet::CUESheet( const std::string & filename ) {
	}
	
	std::pair< std::string, std::string > CUESheet::getSheetName() const {
		return _sheetName_;
	}
	
	std::pair< std::string, std::string > CUESheet::getAudioName() const {
		return _audioName_;
	}
	
	CUESheet::FieldType CUESheet::getDiscInfo() const {
		return _discInfo_;
	}
	
	std::vector< CUESheet::FieldType > CUESheet::getTrackInfo() const {
		return _TrackInfo_;
	}

}
