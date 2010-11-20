#include "youtubetrack.hpp"
#include "youtubeloader.hpp"
#include "youtubereader.hpp"

#include <QtCore/QEventLoop>

using namespace khopper::album;
using khopper::plugin::YouTubeLoader;
using khopper::plugin::VideoParameter;
using khopper::codec::ReaderSP;
using khopper::codec::YouTubeReader;

YouTubeTrack::YouTubeTrack( const QUrl & uri, const QString & format ):
Track( uri ),
format_( format ) {
}

ReaderSP YouTubeTrack::createReader() const {
	return ReaderSP( new YouTubeReader( YouTubeLoader::getRealUri( this->getURI(), this->format_ ) ) );
}
