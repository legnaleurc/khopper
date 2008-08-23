#include "cue/track.hpp"
#include <boost/format.hpp>
#include <sstream>

namespace Khopper {

	Index::Index() : minute_( 0 ), second_( 0 ), frame_( 0 ) {
	}

	Index::Index( unsigned short int m, unsigned short int s, unsigned short int f ) : minute_( m ), second_( s ), frame_( f ) {
	}
	
	std::string Index::toString() const {
		return ( boost::format( "%||:%||.%||" ) % minute_ % second_ % frame_ ).str();
	}

	Track::Track() : number_( -1 ) {
	}
	
	Track::Track( unsigned short int trackNO, const AudioFile & data ) : number_( trackNO ), dataFile_( data ) {
	}
	
	void Track::addComment( const std::string & key, const std::string & value ) {
		comments_[key] = value;
	}
	
	void Track::addFlag( Flag f ) {
		flags_.insert( f );
	}
	
	void Track::addGarbage( const std::string & gi ) {
		garbage_.push_back( gi );
	}
	
	const AudioFile & Track::getAudioData() const {
		return dataFile_;
	}

	std::string Track::getIndicesString() const {
		return beginIndex_.toString() + "\n" + endIndex_.toString() + "\n";
	}
	
	const std::string & Track::getPerformer() const {
		return performer_;
	}

	const std::string & Track::getTitle() const {
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
	
	void Track::setISRC( const std::string & isrc ) {
		isrc_ = isrc;
	}
	
	void Track::setPerformer( const std::string & performer ) {
		performer_ = performer;
	}
	
	void Track::setPostGap( const Index & post ) {
		postGap_ = post;
	}
	
	void Track::setPreGap( const Index & pre ) {
		preGap_ = pre;
	}

	void Track::setSongWriter( const std::string & songWriter ) {
		songWriter_ = songWriter;
	}
	
	void Track::setTitle( const std::string & title ) {
		title_ = title;
	}

	std::string Track::toString() const {
		std::ostringstream sout;
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
		for( std::map< std::string, std::string >::const_iterator it = comments_.begin(); it != comments_.end(); ++it ) {
			sout << "\t" << it->first << ":\t" << it->second << "\n";
		}
		sout << "Garbage:\n";
		for( std::vector< std::string >::const_iterator it = garbage_.begin(); it != garbage_.end(); ++it ) {
			sout << "\t" << *it << "\n";
		}
		for( std::set< Flag >::const_iterator it = flags_.begin(); it != flags_.end(); ++it ) {
			sout << *it << "\n";
		}
		return sout.str();
	}
	
}
