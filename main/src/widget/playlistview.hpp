/**
 * @file playlistview.hpp
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

#include "khopper/playlist.hpp"

#include <QtGui/QTableView>

namespace khopper {

	namespace album {
		class PlayList;
	}

	namespace widget {

		/**
		 * @brief The song list view in the center widget
		 */
		class PlayListView : public QTableView {
			Q_OBJECT

		public:
			/**
			 * @brief Default constructor
			 * @param [in] parent Parent widget
			 */
			explicit PlayListView( QWidget * parent );

		signals:
			void errorOccured( const QString & title, const QString & message );
			/**
			 * @brief Emitted when drop an openable file
			 * @param paths File path
			 */
			void fileDropped( const QList< QUrl > & uris );
			/**
			 * @brief Emitted when convert action is required.
			 */
			void requireConvert();
			/**
			 * @brief Emitted when play is required.
			 * @todo Add parameter.
			 */
			void requirePlay();
			void requireProperty();

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

		private slots:
			//void changeTextCodec_( int );
			void dropFiles_();

		private:
			QMenu * contextMenu_;
			QList< QUrl > droppingFiles_;
		};

	}

}

#endif
