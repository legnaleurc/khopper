/**
 * @file cuesheet.hpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * Khopper is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Khopper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KHOPPER_ALBUM_CUESHEET_HPP
#define KHOPPER_ALBUM_CUESHEET_HPP

#include "khopper/album.hpp"

#include <QtCore/QSet>
#include <QtCore/QStringList>

namespace khopper {
	namespace album {

		class CueSheet : public Album {
		public:
			const QStringList & getComments() const;
			const QSet< QString > & getFlags() const;
			const QStringList & getGarbage() const;

			void setCatalog( const QString & catalog );
			void setCDTextFile( const QString & cdTextFile );
			void setComments( const QStringList & comments );
			void setFlags( const QSet< QString > & flags );
			void setGarbage( const QStringList & garbage );

		private:
			QString catalog_;
			QString cdTextFile_;
			QStringList comments_;
			QSet< QString > flags_;
			QStringList garbage_;
		};

	}
}

#endif
