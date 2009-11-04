/**
 * @file album.hpp
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
#ifndef KHOPPER_ALBUM_ALBUM_HPP
#define KHOPPER_ALBUM_ALBUM_HPP

#include "track.hpp"

#include <QVariant>
#include <QVector>

namespace khopper {

	namespace album {

		class Album {
		public:
			typedef QVector< TrackSP >::iterator TrackIterator;
			typedef QVector< TrackSP >::const_iterator TrackConstIterator;

			QVariant get( const QString & key ) const;
			void set( const QString & key, const QVariant & value );

			TrackCSP operator []( unsigned int index ) const {
				return this->tracks_[index];
			}
			TrackSP operator []( unsigned int index ) {
				return this->tracks_[index];
			}
			unsigned int trackSize() const {
				return this->tracks_.size();
			}
			TrackIterator trackBegin() {
				return this->tracks_.begin();
			}
			TrackConstIterator trackBegin() const {
				return this->tracks_.begin();
			}
			TrackIterator trackEnd() {
				return this->tracks_.end();
			}
			TrackConstIterator trackEnd() const {
				return this->tracks_.end();
			}
			TrackSP trackBack() {
				return this->tracks_.back();
			}
			TrackCSP trackBack() const {
				return this->tracks_.back();
			}

			void pushBack( TrackSP track ) {
				this->tracks_.push_back( track );
			}

		private:
			QMap< QString, QVariant > fields_;
			QVector< TrackSP > tracks_;
		};

	}

}

#endif
