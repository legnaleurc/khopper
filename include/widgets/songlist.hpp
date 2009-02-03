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
#ifndef KHOPPER_SONGLIST_HPP
#define KHOPPER_SONGLIST_HPP

#include <QTableView>

namespace Khopper {

	/**
	 * @brief The song list view in the center widget
	 */
	class SongListView : public QTableView {
		Q_OBJECT

	public:
		/**
		 * @brief Default constructor
		 * @param [in] parent Parent widget
		 */
		SongListView( QWidget * parent = 0 );

	signals:
		/**
		 * @brief Open a file by path
		 * @param [in] path File path
		 */
		void openFile( const QString & path );

	protected:
		/**
		 * @brief Mouse drag enter event handler
		 * @param event Drag enter event
		 */
		virtual void dragEnterEvent( QDragEnterEvent * event );

		/**
		 * @brief Mouse drag move event handler
		 * @param event Drag move event
		 */
		virtual void dragMoveEvent( QDragMoveEvent * event );

		/**
		 * @brief Mouse drop event handler
		 * @param event Drop event
		 */
		virtual void dropEvent( QDropEvent * event );
	};

}

#endif
