#include "cuesheet.hpp"

namespace {
	const boost::regex COMMENT( "\\s*REM\\s+(.*?)\\s+(.*?)" );
	const boost::regex SINGLE( "\\s*(CATALOG|CDTEXTFILE|ISRC|PERFORMER|SONGWRITER|TITLE)\\s+(.*)" );
	const boost::regex FILES( "\\s*FILE\\s(.*?)\\s(WAVE)" );
	const boost::regex FLAGS( "\\s*FLAGS\\s+(DATA|DCP|4CH|PRE|SCMS)" );
	const boost::regex TRACK( "\\s*TRACK\\s+(\\d+)\\s+(AUDIO)" );
	const boost::regex INDEX( "\\s*(INDEX|PREGAP|POSTGAP)\\s+((\\d+)\\s+)?(\\d+):(\\d+):(\\d+)" );
	
	std::string stripQuote( const std::string & s ) {
		if( s[0] == '\"' && s[s.length()-1] == '\"' ) {
			return s.substr( 1, s.length() - 2 );
		} else {
			return s;
		}
	}
	
	std::vector< std::string > createHeader() {
		std::vector< std::string > vs;
		vs.push_back( "Title" );
		vs.push_back( "Performer" );
		return vs;
	}
	
}

namespace Khopper {
	
	const std::vector< std::string > CUESheet::headers = createHeader();
	
	CUESheet::CUESheet( const std::string & filename ) {
		std::ifstream fin( filename.c_str() );
		if( !fin.is_open() ) {
			throw Error< System >( ( boost::format( "Can not open %1% !" ) % filename ).str() );
		}
		std::string temp;
		std::vector< std::string > file;
		while( getline( fin, temp ) ) {
			file.push_back( temp );
		}
		fin.close();
		
		parseCUE_( file );
	}
	
	void CUESheet::parseCUE_( const std::vector< std::string > & cueLines ) {
		boost::smatch result;
		int trackNO = -1;
		AudioFile currentFile;
		for( std::size_t i = 0; i < cueLines.size(); ++i ) {
			if( boost::regex_match( cueLines[i], result, SINGLE ) ) {
				parseSingle_( result[1], result[2], trackNO );
			} else if( boost::regex_match( cueLines[i], result, FILES ) ) {
				currentFile = parseFile_( result[1], result[2] );
			} else if( boost::regex_match( cueLines[i], result, FLAGS ) ) {
				parseFlags_( result[1], trackNO );
			} else if( boost::regex_match( cueLines[i], result, INDEX ) ) {
				parseIndex_( result[1], result[3], result[4], result[5], result[6], trackNO );
			} else if( boost::regex_match( cueLines[i], result, COMMENT ) ) {
				parseComment_( result[1], result[2], trackNO );
			} else if( boost::regex_match( cueLines[i], result, TRACK ) ) {
				++trackNO;
				parseTrack_( result[1], result[2], trackNO );
				if( currentFile.name != "" ) {
					tracks_[trackNO].dataFile = currentFile;
					currentFile = AudioFile();
				}
			} else {
				parseGarbage_( cueLines[i], trackNO );
			}
		}
	}
	
	void CUESheet::parseSingle_( const std::string & c, const std::string & s, int trackNO ) {
		std::string content = stripQuote( s );
		if( c == "CATALOG" ) {
			if( trackNO == -1 ) {
				catalog_ = content;
			}
		} else if( c == "CDTEXTFILE" ) {
			if( trackNO == -1 ) {
				cdTextFile_ = content;
			}
		} else if( c == "ISRC" ) {
			if( trackNO != -1 ) {
				tracks_[trackNO].isrc = content;
			}
		} else if( c == "PERFORMER" ) {
			if( trackNO == -1 ) {
				performer_ = content;
			} else {
				tracks_[trackNO].performer = content;
			}
		} else if( c == "SONGWRITER" ) {
			if( trackNO == -1 ) {
				songWriter_ = content;
			} else {
				tracks_[trackNO].songWriter = content;
			}
		} else if( c == "TITLE" ) {
			if( trackNO == -1 ) {
				title_ = content;
			} else {
				tracks_[trackNO].title = content;
			}
		}
	}
	
	AudioFile CUESheet::parseFile_( const std::string & fileName, const std::string & type ) {
		return AudioFile( stripQuote( fileName ), type );
	}
	
	void CUESheet::parseFlags_( const std::string & flag, int trackNO ) {
		if( trackNO != -1 ) {
			tracks_[trackNO].addFlag( flag );
		}
	}
	
	void CUESheet::parseIndex_( const std::string & type, const std::string & num, const std::string & m, const std::string & s, const std::string & f, int trackNO ) {
		unsigned short int minute = boost::lexical_cast< unsigned short int >( m );
		unsigned short int second = boost::lexical_cast< unsigned short int >( s );
		unsigned short int frame = boost::lexical_cast< unsigned short int >( f );
		
		if( type == "INDEX" ) {
			tracks_[trackNO].indices[boost::lexical_cast< short int >( num )] = Index( minute, second, frame );
		} else if( type == "PREGAP" ) {
			tracks_[trackNO].preGap = Index( minute, second, frame );
		} else if( type == "POSTGAP" ) {
			tracks_[trackNO].postGap = Index( minute, second, frame );
		}
	}
	
	void CUESheet::parseComment_( const std::string & key, const std::string & value, int trackNO ) {
		if( trackNO == -1 ) {
			comments_[key] = value;
		} else {
			tracks_[trackNO].comments[key] = value;
		}
	}
	
	void CUESheet::parseTrack_( const std::string & num, const std::string & type, int trackNO ) {
		tracks_.push_back( Track( boost::lexical_cast< unsigned short int >( num ), type ) );
	}
	
	void CUESheet::parseGarbage_( const std::string & line, int trackNO ) {
		if( trackNO == -1 ) {
			garbage_.push_back( line );
		} else {
			tracks_[trackNO].garbage.push_back( line );
		}
	}
	const std::string & CUESheet::getCatalog() const {
		return catalog_;
	}
	
	const std::string & CUESheet::getCDTextFile() const {
		return cdTextFile_;
	}
	
	const std::map< std::string, std::string > & CUESheet::getComments() const {
		return comments_;
	}
	
	const std::vector< std::string > & CUESheet::getGarbage() const {
		return garbage_;
	}
	
	const std::string & CUESheet::getPerformer() const {
		return performer_;
	}
	
	const std::string & CUESheet::getSongWriter() const {
		return songWriter_;
	}
	
	const std::string & CUESheet::getTitle() const {
		return title_;
	}
	
	const Track & CUESheet::operator []( std::size_t index ) const {
		return tracks_[index];
	}
	
	std::size_t CUESheet::size() const {
		return tracks_.size();
	}

}
