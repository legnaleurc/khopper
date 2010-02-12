/**
 * @file songlist.hpp
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
#ifndef KHOPPER_WIDGET_SONGLIST_HPP
#define KHOPPER_WIDGET_SONGLIST_HPP

#include "track.hpp"

#include <QtCore/QList>
#include <QtCore/QUrl>
#include <QtGui/QStandardItemModel>
#include <QtGui/QTableView>

namespace khopper {
	namespace widget {

		class PropertiesWidget;

		/**
		 * @brief The song list view in the center widget
		 */
		class SongList : public QTableView {
			Q_OBJECT

		public:
			/**
			 * @brief Default constructor
			 * @param [in] parent Parent widget
			 */
			SongList( QWidget * parent = 0 );

			/**
			 * @brief Append tracks to the song list
			 * @param [in] tracks Tracks
			 */
			void appendTracks( const album::TrackList & tracks );
			/**
			 * @brief Get all tracks
			 */
			const album::TrackList & getTracks() const {
				return this->tracks_;
			}
			/**
			 * @brief Get selected tracks
			 *
			 * The cost may be expansive.
			 */
			album::TrackList getSelectedTracks() const;

			/// Test if no track
			bool isEmpty() const {
				return this->tracks_.empty();
			}

		signals:
			void error( const QString & title, const QString & message );
			/**
			 * @brief Emitted when drop an openable file
			 * @param paths File path
			 */
			void fileDropped( const QList< QUrl > & uris );
			/**
			 * @brief Emitted when convert action is required.
			 */
			void requireConvert( const album::TrackList & tracks );
			/**
			 * @brief Emitted when play is required.
			 * @todo Add parameter.
			 */
			void requirePlay();

		protected:
			/// See the documention of Qt toolkit
			virtual void contextMenuEvent( QContextMenuEvent * event );
			/// See the documention of Qt toolkit
			virtual void dragEnterEvent( QDragEnterEvent * event );
			/// See the documention of Qt toolkit
			virtual void dragMoveEvent( QDragMoveEvent * event );
			/// See the documention of Qt toolkit
			virtual void dropEvent( QDropEvent * event );
			/// See the documention of Qt toolkit
			virtual void mouseDoubleClickEvent( QMouseEvent * event );
// 			virtual void mousePressEvent( QMouseEvent * event );
// 			virtual void mouseReleaseEvent( QMouseEvent * event );

		private slots:
			void changeTextCodec_( int );
			void convertHelper_();
			void dropFiles_();
			void propertiesHelper_();
			void removeSelected_();

		private:
			QStandardItemModel * model_;
			QMenu * contextMenu_;
			PropertiesWidget * propWidget_;
			album::TrackList tracks_;
			QList< QUrl > droppingFiles_;
		};

	}
}

#endif
