/**
 * @file player.hpp
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
#ifndef KHOPPER_WIDGET_PLAYER_HPP
#define KHOPPER_WIDGET_PLAYER_HPP

#include "track.hpp"

#include <Phonon/MediaObject>
#include <Phonon/VolumeSlider>
#include <QtCore/QSharedPointer>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

namespace Ui {
	class Player;
}

namespace khopper {

	namespace album {
		class PlayList;
	}

	namespace widget {

		class SeekSlider;
		class SongList;

		/// Audio player
		class Player : public QWidget {
			Q_OBJECT

		public:
			/// Default constructor
			explicit Player( QWidget * parent );

			/// Get selected tracks
			album::TrackList getSelectedTracks() const;
			/// Get all tracks
			const album::TrackList & getTracks() const;
			/// Append tracks
			void append( const album::PlayList & playList );

		signals:
			/**
			 * @brief Emitted when drop an openable file
			 * @param paths File path
			 */
			void fileDropped( const QList< QUrl > & paths );
			/**
			 * @brief Emmited when convert action is required.
			 */
			void requireConvert( const album::TrackList & );
			/**
			 * @brief Error message
			 * @param title Error title
			 * @param errMsg Error masage
			 */
			void error( const QString & title, const QString & errMsg );

		private slots:
			void play_();
			void playOrPause_();
			void stop_();
			void handleState_( Phonon::State, Phonon::State );
			void tick_( qint64 );
			void updateTimestamp_( int ms );

		private:
			QSharedPointer< Ui::Player > ui_;
			Phonon::MediaObject * player_;
			album::Timestamp currentTimeStamp_;
			album::Timestamp duration_;
			album::TrackSP currentTrack_;
			qint64 currentBeginTime_;
			qint64 currentEndTime_;
			bool starting_;
		};

	}

}

#endif
