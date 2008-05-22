#ifndef CUE_HPP
#define CUE_HPP

#include <string>
#include <vector>

namespace Khopper {
	
	namespace DiscField {
		enum {
			Arranger,
			Composer,
			Genre,
			Message,
			Tracks,
			Perfomer,
			Songwriter,
			Title,
			UPC_EAN
		};
	}
	namespace TrackField {
		enum {
			Arranger,
			Composer,
			Genre,
			ISRC,
			Message,
			Number,
			Perfomer,
			Title,
			CD_Text
		};
	}
	
	const char * const HEADERS[] = {
		"Arranger",
		"Composer",
		"Genre",
		"ISRC",
		"Message",
		"Number",
		"Perfomer",
		"Title",
		"CD_Text"
	};

	std::vector< std::vector< std::string > > parseCUE( const std::string & filename );

}

#endif
