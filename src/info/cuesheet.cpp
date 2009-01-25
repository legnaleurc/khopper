#include "cuesheet.hpp"
// #include "audiofile.hpp"
#include "track.hpp"
#include "error.hpp"
#include "os.hpp"
#include "tr1.hpp"

#include <QtDebug>

#include <sstream>

using std::wstring;
using std::tr1::wregex;
using std::tr1::wsmatch;
using std::tr1::regex_match;

namespace {

	const wregex COMMENT( L"\\s*REM\\s+(.*)\\s+(.*)\\s*" );
	const wregex SINGLE( L"\\s*(CATALOG|CDTEXTFILE|ISRC|PERFORMER|SONGWRITER|TITLE)\\s+(.*)\\s*" );
	const wregex FILES( L"\\s*FILE\\s+(.*)\\s+(WAVE)\\s*" );
	const wregex FLAGS( L"\\s*FLAGS\\s+(DATA|DCP|4CH|PRE|SCMS)\\s*" );
	const wregex TRACK( L"\\s*TRACK\\s+(\\d+)\\s+(AUDIO)\\s*" );
	const wregex INDEX( L"\\s*(INDEX|PREGAP|POSTGAP)\\s+((\\d+)\\s+)?(\\d+):(\\d+):(\\d+)\\s*" );
	
	wstring stripQuote( const wstring & s ) {
		if( s[0] == '\"' && s[s.length()-1] == '\"' ) {
			return s.substr( 1, s.length() - 2 );
		} else {
			return s;
		}
	}
	
	std::vector< wstring > createHeader() {
		std::vector< wstring > vs;
		vs.push_back( L"Title" );
		vs.push_back( L"Performer" );
		return vs;
	}

	unsigned short int toUShort( const std::wstring & s ) {
		std::wistringstream sin( s );
		unsigned short int usi;
		sin >> usi;
		return usi;
	}

}

namespace Khopper {
	
	const std::vector< wstring > CUESheet::Headers = createHeader();
	
	CUESheet::CUESheet() {}
	
	CUESheet::CUESheet( const wstring & content, const wstring & dirPath ) {
		qDebug() << "Parsing CUE sheet:" << dirPath;
		this->parseCUE_( content, dirPath );
	}

	void CUESheet::open( const wstring & content, const wstring & dirPath ) {
		qDebug() << "Parsing CUE sheet:" << dirPath;
		this->parseCUE_( content, dirPath );
	}

	void CUESheet::parseCUE_( const wstring & content, const wstring & dirPath ) {
		int trackNO = -1;
		std::pair< wstring, FileType > currentFile;
		std::wistringstream sin( content );
		wsmatch result;
		wstring line;

		while( getline( sin, line ) ) {
			qDebug() << line;
			if( regex_match( line, result, SINGLE ) ) {
// 				qDebug() << "Single matched:" << SINGLE.cap( 1 ) << SINGLE.cap( 2 );
				parseSingle_( result[1], result[2], trackNO );
			} else if( regex_match( line, result, FILES ) ) {
// 				qDebug() << "File matched:" << FILES.cap( 1 ) << FILES.cap( 2 );
				currentFile = parseFile_( result[1], result[2], dirPath );
			} else if( regex_match( line, result, FLAGS ) ) {
// 				qDebug() << "Flag matched:" << FLAGS.cap( 1 );
				parseFlags_( result[1], trackNO );
			} else if( regex_match( line, result, INDEX ) ) {
// 				qDebug() << "Index matched:" << INDEX.cap( 3 );
				parseIndex_( result[1], result[3], result[4], result[5], result[6], trackNO );
			} else if( regex_match( line, result, COMMENT ) ) {
// 				qDebug() << "Comment matched:" << COMMENT.cap( 1 ) << COMMENT.cap( 2 );
				parseComment_( result[1], result[2], trackNO );
			} else if( regex_match( line, result, TRACK ) ) {
// 				qDebug() << "Track matched:" << TRACK.cap( 1 ) << TRACK.cap( 2 );
				++trackNO;
				parseTrack_( result[1], currentFile, result[2] );
			} else {
				qDebug( "Nothing matched: garbage" );
				parseGarbage_( line, trackNO );
			}
		}
	}

	void CUESheet::parseSingle_( const wstring & c, const wstring & s, int trackNO ) {
		wstring content = stripQuote( s );
		if( c == L"CATALOG" ) {
			if( trackNO == -1 ) {
				this->catalog = content;
			}
		} else if( c == L"CDTEXTFILE" ) {
			if( trackNO == -1 ) {
				this->cdTextFile = content;
			}
		} else if( c == L"ISRC" ) {
			if( trackNO != -1 ) {
				this->tracks[trackNO].isrc = content;
			}
		} else if( c == L"PERFORMER" ) {
			if( trackNO == -1 ) {
				this->performer = content;
			} else {
				this->tracks[trackNO].performer = content;
			}
		} else if( c == L"SONGWRITER" ) {
			if( trackNO == -1 ) {
				this->songWriter = content;
			} else {
				this->tracks[trackNO].songWriter = content;
			}
		} else if( c == L"TITLE" ) {
			if( trackNO == -1 ) {
				this->title = content;
			} else {
				this->tracks[trackNO].title = content;
			}
		}
	}
	
	std::pair< wstring, CUESheet::FileType > CUESheet::parseFile_( const wstring & fileName, const wstring & type, const wstring & dirPath ) {
		wstring filePath = os::join( dirPath, stripQuote( fileName ) );
		if( type == L"BINARY" ) {
			return std::make_pair( filePath, BINARY );
		} else if( type == L"MOTOROLA" ) {
			return std::make_pair( filePath, MOTOROLA );
		} else if( type == L"AIFF" ) {
			return std::make_pair( filePath, AIFF );
		} else if( type == L"WAVE" ) {
			return std::make_pair( filePath, WAVE );
		} else if( type == L"MP3" ) {
			return std::make_pair( filePath, MP3 );
		} else {
			return std::make_pair( filePath, BINARY );
		}
	}
	
	void CUESheet::parseFlags_( const wstring & flag, int trackNO ) {
		if( trackNO != -1 ) {
			if( flag == L"DATA" ) {
				this->tracks[trackNO].flags = static_cast< Track::Flag >( this->tracks[trackNO].flags | Track::DATA );
			} else if( flag == L"DCP" ) {
				this->tracks[trackNO].flags = static_cast< Track::Flag >( this->tracks[trackNO].flags | Track::DCP );
			} else if( flag == L"4CH" ) {
				this->tracks[trackNO].flags = static_cast< Track::Flag >( this->tracks[trackNO].flags | Track::CH4 );
			} else if( flag == L"PRE" ) {
				this->tracks[trackNO].flags = static_cast< Track::Flag >( this->tracks[trackNO].flags | Track::PRE );
			} else if( flag == L"SCMS" ) {
				this->tracks[trackNO].flags = static_cast< Track::Flag >( this->tracks[trackNO].flags | Track::SCMS );
			}
		}
	}
	
	void CUESheet::parseIndex_( const wstring & type, const wstring & num, const wstring & m, const wstring & s, const wstring & f, int trackNO ) {
		unsigned short int minute = toUShort( m );
		unsigned short int second = toUShort( s );
		unsigned short int frame = toUShort( f );

		if( type == L"INDEX" ) {
			if( toUShort( num ) == 1 ) {
				this->tracks[trackNO].startTime = Index( minute, second, frame );
			} else {
				this->tracks[trackNO-1].duration = Index( minute, second, frame ) - this->tracks[trackNO].startTime;
			}
		} else if( type == L"PREGAP" ) {
			this->tracks[trackNO].preGap = Index( minute, second, frame );
		} else if( type == L"POSTGAP" ) {
			this->tracks[trackNO].postGap = Index( minute, second, frame );
		}
	}
	
	void CUESheet::parseComment_( const wstring & key, const wstring & value, int trackNO ) {
		if( trackNO == -1 ) {
			this->comments[key] = value;
		} else {
			this->tracks[trackNO].comments[key] = value;
		}
	}
	
	void CUESheet::parseTrack_( const wstring & num, const std::pair< wstring, FileType > & audioData, const wstring & type ) {
		Track track;
		track.index = toUShort( num );
		track.filePath = audioData.first;
		if( type == L"AUDIO" ) {
			track.dataType = Track::AUDIO;
		} else if( type == L"CDG" ) {
			track.dataType = Track::CDG;
		} else if( type == L"MODE1/2048" ) {
			track.dataType = Track::MODE1_2048;
		} else if( type == L"MODE1/2352" ) {
			track.dataType = Track::MODE1_2352;
		} else if( type == L"MODE2/2336" ) {
			track.dataType = Track::MODE2_2336;
		} else if( type == L"MODE2/2352" ) {
			track.dataType = Track::MODE2_2352;
		} else if( type == L"CDI/2336" ) {
			track.dataType = Track::CDI_2336;
		} else if( type == L"CDI/2352" ) {
			track.dataType = Track::CDI_2352;
		} else {
			track.dataType = Track::AUDIO;
		}
		this->tracks.push_back( track );
	}
	
	void CUESheet::parseGarbage_( const wstring & line, int trackNO ) {
		if( trackNO == -1 ) {
			this->garbage.push_back( line );
		} else {
			this->tracks[trackNO].garbage.push_back( line );
		}
	}

	wstring CUESheet::toStdWString() const {
		std::wostringstream sout;
		sout << L"Title:\t" << this->title << L"\n";
		sout << L"Performer:\t" << this->performer << L"\n";
		sout << L"Song Writer:\t" << this->songWriter << L"\n";
		sout << L"Catalog:\t" << this->catalog << L"\n";
		sout << L"CD text:\t" << this->cdTextFile << L"\n";
		sout << L"Track number:\t" << this->tracks.size() << L"\n";
		sout << L"Comments:\n";
		for( std::map< wstring, wstring >::const_iterator it = this->comments.begin(); it != this->comments.end(); ++it  ) {
			sout << L"\t" << it->first << L":\t" << it->second << L"\n";
		}
		sout << L"Garbage:\n";
		for( std::vector< wstring >::const_iterator it = this->garbage.begin(); it != this->garbage.end(); ++it ) {
			sout << L"\t" << *it << L"\n";
		}
		sout << L"Tracks:\n";
		for( std::vector< Track >::const_iterator it = this->tracks.begin(); it != this->tracks.end(); ++it ) {
			sout << L"\t" << it->toStdWString() << L"\n";
		}
		return sout.str();
	}

}
