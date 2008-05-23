#ifndef CUE_HPP
#define CUE_HPP

#include <string>
#include <vector>

namespace Khopper {

	typedef std::vector< std::string > FieldType;
	
	namespace Disc {

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

	}
	
	namespace Track {

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

		const char * const Header[] = {
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

	}
	

	std::vector< FieldType > parseCUE( const std::string & filename );

}

#endif
