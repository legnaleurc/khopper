#ifndef KHOPPER_ALBUM_YOUTUBETRACK_HPP
#define KHOPPER_ALBUM_YOUTUBETRACK_HPP

#include "khopper/track.hpp"

namespace khopper {
	namespace album {

		class YouTubeTrack : public Track {
		public:
			YouTubeTrack( const QUrl & uri, const QString & title, const QString & format );

			virtual codec::ReaderSP createReader() const;

		private:
			QString format_;
		};

	}
}

#endif
