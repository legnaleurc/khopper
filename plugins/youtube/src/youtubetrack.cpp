#include "youtubetrack.hpp"

using namespace khopper::album;

YouTubeTrack::YouTubeTrack( const QUrl & uri, const QString & format ):
Track( uri ),
format_( format ) {
}
