#ifndef KHOPPER_ALBUM_ALBUM_HPP
#define KHOPPER_ALBUM_ALBUM_HPP

#include <QVariant>

#include <string>
#include <map>

namespace khopper {

	namespace album {

		class Album {
		public:
			const QVariant & get( const std::string & key ) const;
			void set( const std::string & key, const QVariant & value );

		private:
			std::map< std::string, QVariant > fields_;
		};

	}

}

#endif
