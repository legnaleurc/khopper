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

#include "khopper/playlist.hpp"

#include <Phonon/MediaObject>
#include <QtCore/QModelIndex>
#include <QtGui/QWidget>

namespace Ui {
	class Player;
}

namespace khopper {

	namespace utility {
		class PlayListModel;
	}

	namespace widget {

		class PropertyDialog;

		/// Audio player
		class Player : public QWidget {
			Q_OBJECT

		public:
			/// Default constructor
			explicit Player( QWidget * parent );

			/// Append tracks
			void append( const album::PlayList & playList );
			bool isPlayable() const;
			/// Get selected tracks
			album::PlayList getSelectedTracks() const;
			/// Get all tracks
			const album::PlayList & getTracks() const;

		signals:
			/**
			 * @brief Emitted when drop an openable file
			 * @param paths File path
			 */
			void fileDropped( const QList< QUrl > & paths );
			/**
			 * @brief Emmited when convert action is required.
			 */
			void requireConvert( const khopper::album::PlayList & playList );
			/**
			 * @brief Error message
			 * @param title Error title
			 * @param message Error masage
			 */
			void errorOccured( const QString & title, const QString & message );

		private slots:
			void convertHelper_();
			void play_();
			void playOrPause_();
			void propertyHelper_( const QModelIndex & );
			void stop_();
			void handleState_( Phonon::State, Phonon::State );
			void onTick_( qint64 );
			void onVolumeChanged_( qreal );
			void onFinished_();

		private:
			void setQueue_( const album::PlayList & );

			album::TrackSP currentTrack_;
			qint64 duration_;
			utility::PlayListModel * model_;
			bool playable_;
			Phonon::MediaObject * player_;
			PropertyDialog * prop_;
			std::shared_ptr< Ui::Player > ui_;
		};

	}

}

#endif
