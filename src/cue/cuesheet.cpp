#include "cue/cuesheet.hpp"
#include "cue/audiofile.hpp"
#include "cue/track.hpp"
#include "util/error.hpp"
#include "util/os.hpp"
#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

namespace {
	const boost::regex COMMENT( "\\s*REM\\s+(.*?)\\s+(.*?)\\s*" );
	const boost::regex SINGLE( "\\s*(CATALOG|CDTEXTFILE|ISRC|PERFORMER|SONGWRITER|TITLE)\\s+(.*?)\\s*" );
	const boost::regex FILES( "\\s*FILE\\s+(.*?)\\s+(WAVE)\\s*" );
	const boost::regex FLAGS( "\\s*FLAGS\\s+(DATA|DCP|4CH|PRE|SCMS)\\s*" );
	const boost::regex TRACK( "\\s*TRACK\\s+(\\d+)\\s+(AUDIO)\\s*" );
	const boost::regex INDEX( "\\s*(INDEX|PREGAP|POSTGAP)\\s+((\\d+)\\s+)?(\\d+):(\\d+):(\\d+)\\s*" );
	const boost::regex DIRNAME( "(.*)/.*" );
	
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
	
	CUESheet::CUESheet() {}
	
	CUESheet::CUESheet( const std::vector< std::string > & content, const std::string & dirPath ) {
		parseCUE_( content, dirPath );
	}

	void CUESheet::open( const std::vector< std::string > & content, const std::string & dirPath ) {
		parseCUE_( content, dirPath );
	}
	
	void CUESheet::parseCUE_( const std::vector< std::string > & cueLines, const std::string & dirPath ) {
		boost::smatch result;
		int trackNO = -1;
		AudioFile currentFile;
		for( std::size_t i = 0; i < cueLines.size(); ++i ) {
			if( boost::regex_match( cueLines[i], result, SINGLE ) ) {
				parseSingle_( result[1], result[2], trackNO );
			} else if( boost::regex_match( cueLines[i], result, FILES ) ) {
				currentFile = parseFile_( result[1], result[2], dirPath );
			} else if( boost::regex_match( cueLines[i], result, FLAGS ) ) {
				parseFlags_( result[1], trackNO );
			} else if( boost::regex_match( cueLines[i], result, INDEX ) ) {
				parseIndex_( result[1], result[3], result[4], result[5], result[6], trackNO );
			} else if( boost::regex_match( cueLines[i], result, COMMENT ) ) {
				parseComment_( result[1], result[2], trackNO );
			} else if( boost::regex_match( cueLines[i], result, TRACK ) ) {
				++trackNO;
				parseTrack_( result[1], currentFile, result[2], trackNO );
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
				tracks_[trackNO].setISRC( content );
			}
		} else if( c == "PERFORMER" ) {
			if( trackNO == -1 ) {
				performer_ = content;
			} else {
				tracks_[trackNO].setPerformer( content );
			}
		} else if( c == "SONGWRITER" ) {
			if( trackNO == -1 ) {
				songWriter_ = content;
			} else {
				tracks_[trackNO].setSongWriter( content );
			}
		} else if( c == "TITLE" ) {
			if( trackNO == -1 ) {
				title_ = content;
			} else {
				tracks_[trackNO].setTitle( content );
			}
		}
	}
	
	AudioFile CUESheet::parseFile_( const std::string & fileName, const std::string & type, const std::string & dirPath ) {
		if( type == "BINARY" ) {
			return AudioFile( os::join( dirPath, stripQuote( fileName ) ), AudioFile::BINARY );
		} else if( type == "MOTOROLA" ) {
			return AudioFile( os::join( dirPath, stripQuote( fileName ) ), AudioFile::MOTOROLA );
		} else if( type == "AIFF" ) {
			return AudioFile( os::join( dirPath, stripQuote( fileName ) ), AudioFile::AIFF );
		} else if( type == "WAVE" ) {
			return AudioFile( os::join( dirPath, stripQuote( fileName ) ), AudioFile::WAVE );
		} else if( type == "MP3" ) {
			return AudioFile( os::join( dirPath, stripQuote( fileName ) ), AudioFile::MP3 );
		} else {
			return AudioFile( os::join( dirPath, stripQuote( fileName ) ), AudioFile::BINARY );
		}
	}
	
	void CUESheet::parseFlags_( const std::string & flag, int trackNO ) {
		if( trackNO != -1 ) {
			if( flag == "DATA" ) {
				tracks_[trackNO].addFlag( Track::DATA );
			} else if( flag == "DCP" ) {
				tracks_[trackNO].addFlag( Track::DCP );
			} else if( flag == "4CH" ) {
				tracks_[trackNO].addFlag( Track::CH4 );
			} else if( flag == "PRE" ) {
				tracks_[trackNO].addFlag( Track::PRE );
			} else if( flag == "SCMS" ) {
				tracks_[trackNO].addFlag( Track::SCMS );
			}
		}
	}
	
	void CUESheet::parseIndex_( const std::string & type, const std::string & num, const std::string & m, const std::string & s, const std::string & f, int trackNO ) {
		unsigned short int minute = boost::lexical_cast< unsigned short int >( m );
		unsigned short int second = boost::lexical_cast< unsigned short int >( s );
		unsigned short int frame = boost::lexical_cast< unsigned short int >( f );
		
		if( type == "INDEX" ) {
			if( boost::lexical_cast< unsigned short int >( num ) ) {
				tracks_[trackNO].setEndIndex( Index( minute, second, frame ) );
			} else {
				tracks_[trackNO].setBeginIndex( Index( minute, second, frame ) );
			}
		} else if( type == "PREGAP" ) {
			tracks_[trackNO].setPreGap( Index( minute, second, frame ) );
		} else if( type == "POSTGAP" ) {
			tracks_[trackNO].setPostGap( Index( minute, second, frame ) );
		}
	}
	
	void CUESheet::parseComment_( const std::string & key, const std::string & value, int trackNO ) {
		if( trackNO == -1 ) {
			comments_[key] = value;
		} else {
			tracks_[trackNO].addComment( key, value );
		}
	}
	
	void CUESheet::parseTrack_( const std::string & num, const AudioFile & audioData, const std::string & type, int trackNO ) {
		tracks_.push_back( Track( boost::lexical_cast< unsigned short int >( num ), audioData ) );
		if( type == "AUDIO" ) {
			tracks_[trackNO].setDataType( Track::AUDIO );
		} else if( type == "CDG" ) {
			tracks_[trackNO].setDataType( Track::CDG );
		} else if( type == "MODE1/2048" ) {
			tracks_[trackNO].setDataType( Track::MODE1_2048 );
		} else if( type == "MODE1/2352" ) {
			tracks_[trackNO].setDataType( Track::MODE1_2352 );
		} else if( type == "MODE2/2336" ) {
			tracks_[trackNO].setDataType( Track::MODE2_2336 );
		} else if( type == "MODE2/2352" ) {
			tracks_[trackNO].setDataType( Track::MODE2_2352 );
		} else if( type == "CDI/2336" ) {
			tracks_[trackNO].setDataType( Track::CDI_2336 );
		} else if( type == "CDI/2352" ) {
			tracks_[trackNO].setDataType( Track::CDI_2352 );
		} else {
			tracks_[trackNO].setDataType( Track::AUDIO );
		}
	}
	
	void CUESheet::parseGarbage_( const std::string & line, int trackNO ) {
		if( trackNO == -1 ) {
			garbage_.push_back( line );
		} else {
			tracks_[trackNO].addGarbage( line );
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
	
	std::size_t CUESheet::size() const {
		return tracks_.size();
	}
	
	std::string CUESheet::toString() const {
		std::ostringstream sout;
		sout << "Title:\t" << title_ << "\n";
		sout << "Performer:\t" << performer_ << "\n";
		sout << "Song Writer:\t" << songWriter_ << "\n";
		sout << "Catalog:\t" << catalog_ << "\n";
		sout << "CD text:\t" << cdTextFile_ << "\n";
		sout << "Track number:\t" << tracks_.size() << "\n";
		sout << "Comments:\n";
		for( std::map< std::string, std::string >::const_iterator it = comments_.begin(); it != comments_.end(); ++it  ) {
			sout << "\t" << it->first << ":\t" << it->second << "\n";
		}
		sout << "Gargages:\n";
		for( std::vector< std::string >::const_iterator it = garbage_.begin(); it != garbage_.end(); ++it ) {
			sout << "\t" << *it << "\n";
		}
		sout << "Tracks:\n";
		for( std::vector< Track >::const_iterator it = tracks_.begin(); it != tracks_.end(); ++it ) {
			sout << "\t" << it->toString() << "\n";
		}
		return sout.str();
	}
	
	const Track & CUESheet::operator []( std::size_t index ) const {
		return tracks_[index];
	}

}
