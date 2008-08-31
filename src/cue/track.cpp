#include "cue/track.hpp"

#include <QTextStream>

namespace {
	QStringList createHeader() {
		QStringList vs;
		vs.push_back( "Title" );
		vs.push_back( "Performer" );
		return vs;
	}
}

namespace Khopper {

	Index::Index() : minute_( 0 ), second_( 0 ), frame_( 0 ) {
	}

	Index::Index( unsigned short int m, unsigned short int s, unsigned short int f ) : minute_( m ), second_( s ), frame_( f ) {
	}
	
	QString Index::toString() const {
		return QString( "%1:%2.%3" ).arg( minute_ ).arg( second_, 2, 10, QChar( '0' ) ).arg( frame_, 2, 10, QChar( '0' ) );
	}

	Track::Track() : number_( -1 ) {
	}
	
	Track::Track( unsigned short int trackNO, const AudioFile & data ) : number_( trackNO ), dataFile_( data ) {
	}
	
	void Track::addComment( const QString & key, const QString & value ) {
		comments_[key] = value;
	}
	
	void Track::addFlag( Flag f ) {
		flags_.insert( f );
	}
	
	void Track::addGarbage( const QString & gi ) {
		garbage_.push_back( gi );
	}
	
	const AudioFile & Track::getAudioData() const {
		return dataFile_;
	}

	QString Track::getIndicesString() const {
		if( endIndex_.getMinute() == 0 && endIndex_.getSecond() == 0 && endIndex_.getFrame() == 0 ) {
			return beginIndex_.toString() + "\n";
		} else {
			return beginIndex_.toString() + "\n" + endIndex_.toString() + "\n";
		}
	}

	unsigned short int Track::getNumber() const {
		return number_;
	}
	
	const QString & Track::getPerformer() const {
		return performer_;
	}

	const QString & Track::getTitle() const {
		return title_;
	}
	
	void Track::setAudioData( const AudioFile & audio ) {
		dataFile_ = audio;
	}
	
	void Track::setBeginIndex( const Index & begin ) {
		beginIndex_ = begin;
	}
	
	void Track::setDataType( DataType type ) {
		dataType_ = type;
	}
	
	void Track::setEndIndex( const Index & end ) {
		endIndex_ = end;
	}
	
	void Track::setISRC( const QString & isrc ) {
		isrc_ = isrc;
	}
	
	void Track::setPerformer( const QString & performer ) {
		performer_ = performer;
	}
	
	void Track::setPostGap( const Index & post ) {
		postGap_ = post;
	}
	
	void Track::setPreGap( const Index & pre ) {
		preGap_ = pre;
	}

	void Track::setSongWriter( const QString & songWriter ) {
		songWriter_ = songWriter;
	}
	
	void Track::setTitle( const QString & title ) {
		title_ = title;
	}

	QString Track::toString() const {
		QTextStream sout;
		sout << "Title:\t" << title_ << "\n";
		sout << "Performer:\t" << performer_ << "\n";
		sout << "Song Writer:\t" << songWriter_ << "\n";
		sout << "Track number:\t" << number_ << "\n";
		sout << "ISRC:\t" << isrc_ << "\n";
		sout << "Begin:\t" << beginIndex_.toString() << "\n";
		sout << "End:\t" << endIndex_.toString() << "\n";
		sout << "Pregap:\t" << preGap_.toString() << "\n";
		sout << "Postgap:\t" << postGap_.toString() << "\n";
		sout << "Type:\t" << dataType_ << "\n";
		sout << "Audio Data:\t" << dataFile_.toString() << "\n";
		sout << "Comments:\n";
		for( QMap< QString, QString >::const_iterator it = comments_.begin(); it != comments_.end(); ++it ) {
			sout << "\t" << it.key() << ":\t" << it.value() << "\n";
		}
		sout << "Garbage:\n";
		foreach( QString s, garbage_ ) {
			sout << "\t" << s << "\n";
		}
		foreach( Flag f, flags_ ) {
			sout << f << "\n";
		}
		return sout.readAll();
	}

	const QStringList Track::headers = createHeader();
	
}
