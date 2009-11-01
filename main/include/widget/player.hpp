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

#include <QWidget>
#include <QPushButton>
#include <Phonon/MediaObject>
#include <Phonon/VolumeSlider>

namespace khopper {

	namespace widget {

		class SeekSlider;
		class SongList;

		/// Audio player
		class Player : public QWidget {
			Q_OBJECT

		public:
			/// Default constructor
			Player( QWidget * parent = 0, Qt::WindowFlags f = 0 );

			/// Get selected tracks
			std::vector< album::TrackSP > getSelectedTracks() const;
			/// Get all tracks
			const std::vector< album::TrackSP > & getTracks() const;
			/// Append tracks
			void appendTracks( const std::vector< album::TrackSP > & tracks );

		signals:
			/**
			 * @brief Emitted when drop an openable file
			 * @param paths File path
			 */
			void fileDropped( const QStringList & paths );
			/**
			 * @brief Emmited when convert action is required.
			 */
			void requireConvert();
			/**
			 * @brief Error message
			 * @param title Error title
			 * @param errMsg Error masage
			 */
			void error( const QString & title, const QString & errMsg );

		private slots:
			void playOrPause_();
			void stop_();
			void handleState_( Phonon::State, Phonon::State );
			void tick_( qint64 );

		private:
			void play_();

			Phonon::MediaObject * player_;
			SeekSlider * seeker_;
			Phonon::VolumeSlider * volume_;
			QPushButton * ppb_;
			SongList * songList_;
			album::TrackSP currentTrack_;
			qint64 currentBeginTime_;
			qint64 currentEndTime_;
			bool starting_;
		};

	}

}

#endif
