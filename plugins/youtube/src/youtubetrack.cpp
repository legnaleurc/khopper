#include "youtubetrack.hpp"
#include "youtubeloader.hpp"
#include "youtubereader.hpp"

#include <QtCore/QEventLoop>

using namespace khopper::album;
using khopper::album::Timestamp;
using khopper::codec::ReaderSP;
using khopper::codec::YouTubeReader;
using khopper::plugin::YouTubeLoader;

YouTubeTrack::YouTubeTrack( const QUrl & uri, const QString & title, const QString & format ):
Track( uri ),
format_( format ) {
	this->setAlbum( AlbumSP( new Album ) );
	this->setIndex( 1 );
	this->setTitle( title );
	ReaderSP reader( this->createReader() );
	reader->open( QIODevice::ReadOnly );
	this->setDuration( Timestamp::fromMillisecond( reader->getDuration() ) );
	reader->close();
}

ReaderSP YouTubeTrack::createReader() const {
	YouTubeLoader loader( this->getURI() );
	loader.parseHeader( false );
	return ReaderSP( new YouTubeReader( loader.getRealURI( this->format_ ) ) );
}
