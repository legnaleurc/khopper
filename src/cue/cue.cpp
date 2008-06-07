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
		return std::pair< std::string, std::string >();
	}
	
	std::pair< std::string, std::string > CUESheet::getAudioName() const {
		return std::pair< std::string, std::string >();
	}
	
	CUESheet::FieldType CUESheet::getDiscInfo() const {
		return CUESheet::FieldType();
	}
	
	std::vector< CUESheet::FieldType > CUESheet::getTrackInfo() const {
		return std::vector< CUESheet::FieldType >();
	}

}
