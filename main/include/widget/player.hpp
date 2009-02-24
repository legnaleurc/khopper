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
#ifndef KHOPPER_PLAYER_HPP
#define KHOPPER_PLAYER_HPP

#include "track.hpp"

#include <QWidget>
#include <QPushButton>
#include <Phonon>

namespace Khopper {

	class SongList;

	class Player : public QWidget {
		Q_OBJECT

	public:
		Player( QWidget * parent = 0, Qt::WindowFlags f = 0 );

		std::vector< TrackSP > getSelectedTracks() const;
		const std::vector< TrackSP > & getTracks() const;
		void appendTracks( const std::vector< TrackSP > & tracks );

	signals:
		/**
		 * @brief Emitted when drop an openable file
		 * @param paths File path
		 */
		void dropFile( const QStringList & paths );
		/**
		 * @brief Emmited when convert action is required.
		 */
		void requireConvert();

	private slots:
		void playOrPause_();

	private:
		void play_();

		Phonon::MediaObject * player_;
		Phonon::SeekSlider * seeker_;
		Phonon::VolumeSlider * volume_;
		QPushButton * ppb_;
		SongList * songList_;
	};

}

#endif
