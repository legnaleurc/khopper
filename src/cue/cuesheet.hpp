/**
 * @file cuesheet.hpp
 */
#ifndef CUESHEET_HPP
#define CUESHEET_HPP

#include "error.hpp"
#include "audiofile.hpp"
#include "track.hpp"

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

namespace Khopper {
	
	/**
	 * @brief The CUE parser
	 */
	class CUESheet {
		public:
			static const std::vector< std::string > headers;
			
			CUESheet();
			/**
			 * @brief Give a CUE Sheet path name and create this object
			 * @param [in] fileName path of CUE sheet
			 */
			CUESheet( const std::string & fileName );
			
			void open( const std::string & fileName );
			
			const std::string & getCatalog() const;
			const std::string & getCDTextFile() const;
			const std::map< std::string, std::string > & getComments() const;
			const std::vector< std::string > & getGarbage() const;
			const std::string & getPath() const;
			const std::string & getPerformer() const;
			const std::string & getSongWriter() const;
			const std::string & getTitle() const;
			
			const Track & operator []( std::size_t index ) const;
			std::size_t size() const;
		private:
			void parseCUE_( const std::vector< std::string > & );
			void parseSingle_( const std::string &, const std::string &, int );
			AudioFile parseFile_( const std::string &, const std::string & );
			void parseFlags_( const std::string &, int );
			void parseIndex_( const std::string &, const std::string &, const std::string &, const std::string &, const std::string &, int );
			void parseComment_( const std::string &, const std::string &, int );
			void parseTrack_( const std::string &, const std::string &, int );
			void parseGarbage_( const std::string &, int );
			
			std::string catalog_;
			std::string cdTextFile_;
			std::map< std::string, std::string > comments_;
			std::vector< std::string > garbage_;
			std::string path_;
			std::string performer_;
			std::string songWriter_;
			std::string title_;
			std::vector< Track > tracks_;
	};

}

#endif
