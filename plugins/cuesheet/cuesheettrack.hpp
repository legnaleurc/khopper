/**
 * @file cuesheettrack.hpp
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
#ifndef KHOPPER_ALBUM_CUESHEETTRACK_HPP
#define KHOPPER_ALBUM_CUESHEETTRACK_HPP

#include "rangedreader.hpp"

#include "khopper/track.hpp"

#include <QtCore/QStringList>

namespace khopper {
	namespace album {

		class CueSheetTrack : public Track {
		public:
			explicit CueSheetTrack( codec::RangedReader * reader );

			const QStringList & getComments() const;
			const QStringList & getGarbage() const;
			codec::RangedReader * getRangedReader() const;
			const Timestamp & getStartTime() const;

			void setComments( const QStringList & comments );
			void setDataType( const QString & dataType );
			void setFileType( const QString & fileType );
			void setGarbage( const QStringList & garbage );
			void setISRC( const QString & isrc );
			void setPostgap( const Timestamp & postgap );
			void setPregap( const Timestamp & pregap );
			void setStartTime( const Timestamp & startTime );

		private:
			QStringList comments_;
			QString dataType_;
			QString fileType_;
			QStringList garbage_;
			QString isrc_;
			Timestamp postgap_;
			Timestamp pregap_;
			codec::RangedReader * reader_;
			Timestamp startTime_;
		};

	}
}

#endif
