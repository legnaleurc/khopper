#include "cue/cuesheet.hpp"
#include "cue/audiofile.hpp"
#include "cue/track.hpp"
#include "util/error.hpp"
#include "util/os.hpp"

#include <QRegExp>
#include <QTextStream>
#include <QMutex>
#include <QMutexLocker>
#include <QtDebug>

namespace {
	QMutex sheet;
	
	QRegExp COMMENT( "\\s*REM\\s+(.*)\\s+(.*)\\s*", Qt::CaseSensitive, QRegExp::RegExp2 );
	QRegExp SINGLE( "\\s*(CATALOG|CDTEXTFILE|ISRC|PERFORMER|SONGWRITER|TITLE)\\s+(.*)\\s*", Qt::CaseSensitive, QRegExp::RegExp2 );
	QRegExp FILES( "\\s*FILE\\s+(.*)\\s+(WAVE)\\s*", Qt::CaseSensitive, QRegExp::RegExp2 );
	QRegExp FLAGS( "\\s*FLAGS\\s+(DATA|DCP|4CH|PRE|SCMS)\\s*", Qt::CaseSensitive, QRegExp::RegExp2 );
	QRegExp TRACK( "\\s*TRACK\\s+(\\d+)\\s+(AUDIO)\\s*", Qt::CaseSensitive, QRegExp::RegExp2 );
	QRegExp INDEX( "\\s*(INDEX|PREGAP|POSTGAP)\\s+((\\d+)\\s+)?(\\d+):(\\d+):(\\d+)\\s*", Qt::CaseSensitive, QRegExp::RegExp2 );
	
	QString stripQuote( const QString & s ) {
		if( s[0] == '\"' && s[s.length()-1] == '\"' ) {
			return s.mid( 1, s.length() - 2 );
		} else {
			return s;
		}
	}
	
	QStringList createHeader() {
		QStringList vs;
		vs.push_back( "Title" );
		vs.push_back( "Performer" );
		return vs;
	}
	
}

namespace Khopper {
	
	const QStringList CUESheet::headers = createHeader();
	
	CUESheet::CUESheet() {}
	
	CUESheet::CUESheet( const QStringList & content, const QString & dirPath ) {
		qDebug() << "Parsing CUE sheet:" << dirPath;
		QMutexLocker lock( &sheet );
		parseCUE_( content, dirPath );
	}

	void CUESheet::open( const QStringList & content, const QString & dirPath ) {
		qDebug() << "Parsing CUE sheet:" << dirPath;
		QMutexLocker lock( &sheet );
		parseCUE_( content, dirPath );
	}
	
	void CUESheet::parseCUE_( const QStringList & cueLines, const QString & dirPath ) {
		int trackNO = -1;
		AudioFile currentFile;
		foreach( QString line, cueLines ) {
			qDebug() << line;
			if( SINGLE.exactMatch( line ) ) {
				qDebug() << "Single matched:" << SINGLE.cap( 1 ) << SINGLE.cap( 2 );
				parseSingle_( SINGLE.cap( 1 ), SINGLE.cap( 2 ), trackNO );
			} else if( FILES.exactMatch( line ) ) {
				qDebug() << "File matched:" << FILES.cap( 1 ) << FILES.cap( 2 );
				currentFile = parseFile_( FILES.cap( 1 ), FILES.cap( 2 ), dirPath );
			} else if( FLAGS.exactMatch( line ) ) {
				qDebug() << "Flag matched:" << FLAGS.cap( 1 );
				parseFlags_( FLAGS.cap( 1 ), trackNO );
			} else if( INDEX.exactMatch( line ) ) {
				qDebug() << "Index matched:" << INDEX.cap( 3 );
				parseIndex_( INDEX.cap( 1 ), INDEX.cap( 3 ), INDEX.cap( 4 ), INDEX.cap( 5 ), INDEX.cap( 6 ), trackNO );
			} else if( COMMENT.exactMatch( line ) ) {
				qDebug() << "Comment matched:" << COMMENT.cap( 1 ) << COMMENT.cap( 2 );
				parseComment_( COMMENT.cap( 1 ), COMMENT.cap( 2 ), trackNO );
			} else if( TRACK.exactMatch( line ) ) {
				qDebug() << "Track matched:" << TRACK.cap( 1 ) << TRACK.cap( 2 );
				++trackNO;
				parseTrack_( TRACK.cap( 1 ), currentFile, TRACK.cap( 2 ), trackNO );
			} else {
				qDebug( "Nothing matched: garbage" );
				parseGarbage_( line, trackNO );
			}
		}
	}
	
	void CUESheet::parseSingle_( const QString & c, const QString & s, int trackNO ) {
		QString content = stripQuote( s );
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
	
	AudioFile CUESheet::parseFile_( const QString & fileName, const QString & type, const QString & dirPath ) {
		if( type == "BINARY" ) {
			return AudioFile( os::join( dirPath.toUtf8().constData(), stripQuote( fileName ).toUtf8().constData() ).c_str(), AudioFile::BINARY );
		} else if( type == "MOTOROLA" ) {
			return AudioFile( os::join( dirPath.toUtf8().constData(), stripQuote( fileName ).toUtf8().constData() ).c_str(), AudioFile::MOTOROLA );
		} else if( type == "AIFF" ) {
			return AudioFile( os::join( dirPath.toUtf8().constData(), stripQuote( fileName ).toUtf8().constData() ).c_str(), AudioFile::AIFF );
		} else if( type == "WAVE" ) {
			return AudioFile( os::join( dirPath.toUtf8().constData(), stripQuote( fileName ).toUtf8().constData() ).c_str(), AudioFile::WAVE );
		} else if( type == "MP3" ) {
			return AudioFile( os::join( dirPath.toUtf8().constData(), stripQuote( fileName ).toUtf8().constData() ).c_str(), AudioFile::MP3 );
		} else {
			return AudioFile( os::join( dirPath.toUtf8().constData(), stripQuote( fileName ).toUtf8().constData() ).c_str(), AudioFile::BINARY );
		}
	}
	
	void CUESheet::parseFlags_( const QString & flag, int trackNO ) {
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
	
	void CUESheet::parseIndex_( const QString & type, const QString & num, const QString & m, const QString & s, const QString & f, int trackNO ) {
		unsigned short int minute = m.toUShort();
		unsigned short int second = s.toUShort();
		unsigned short int frame = f.toUShort();
		
		if( type == "INDEX" ) {
			if( num.toUShort() ) {
				tracks_[trackNO].setBeginIndex( Index( minute, second, frame ) );
			} else {
				tracks_[trackNO-1].setEndIndex( Index( minute, second, frame ) );
			}
		} else if( type == "PREGAP" ) {
			tracks_[trackNO].setPreGap( Index( minute, second, frame ) );
		} else if( type == "POSTGAP" ) {
			tracks_[trackNO].setPostGap( Index( minute, second, frame ) );
		}
	}
	
	void CUESheet::parseComment_( const QString & key, const QString & value, int trackNO ) {
		if( trackNO == -1 ) {
			comments_[key] = value;
		} else {
			tracks_[trackNO].addComment( key, value );
		}
	}
	
	void CUESheet::parseTrack_( const QString & num, const AudioFile & audioData, const QString & type, int trackNO ) {
		tracks_.push_back( Track( num.toUShort(), audioData ) );
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
	
	void CUESheet::parseGarbage_( const QString & line, int trackNO ) {
		if( trackNO == -1 ) {
			garbage_.push_back( line );
		} else {
			tracks_[trackNO].addGarbage( line );
		}
	}
	const QString & CUESheet::getCatalog() const {
		return catalog_;
	}
	
	const QString & CUESheet::getCDTextFile() const {
		return cdTextFile_;
	}
	
	const QMap< QString, QString > & CUESheet::getComments() const {
		return comments_;
	}
	
	const QStringList & CUESheet::getGarbage() const {
		return garbage_;
	}
	
	const QString & CUESheet::getPerformer() const {
		return performer_;
	}
	
	const QString & CUESheet::getSongWriter() const {
		return songWriter_;
	}
	
	const QString & CUESheet::getTitle() const {
		return title_;
	}
	
	std::size_t CUESheet::size() const {
		return tracks_.size();
	}
	
	QString CUESheet::toString() const {
		QTextStream sout;
		sout << "Title:\t" << title_ << "\n";
		sout << "Performer:\t" << performer_ << "\n";
		sout << "Song Writer:\t" << songWriter_ << "\n";
		sout << "Catalog:\t" << catalog_ << "\n";
		sout << "CD text:\t" << cdTextFile_ << "\n";
		sout << "Track number:\t" << tracks_.size() << "\n";
		sout << "Comments:\n";
		for( QMap< QString, QString >::const_iterator it = comments_.begin(); it != comments_.end(); ++it  ) {
			sout << "\t" << it.key() << ":\t" << it.value() << "\n";
		}
		sout << "Garbage:\n";
		foreach( QString s, garbage_ ) {
			sout << "\t" << s << "\n";
		}
		sout << "Tracks:\n";
		for( QVector< Track >::const_iterator it = tracks_.begin(); it != tracks_.end(); ++it ) {
			sout << "\t" << it->toString() << "\n";
		}
		return sout.readAll();
	}
	
	const Track & CUESheet::operator []( std::size_t index ) const {
		return tracks_[index];
	}

}
