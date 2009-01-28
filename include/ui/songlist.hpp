/**
 * @file songlist.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KHOPPER_UI_SONGLIST_HPP
#define KHOPPER_UI_SONGLIST_HPP

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
