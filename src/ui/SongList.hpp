/**
 * @file SongList.hpp
 */
#ifndef SONGLIST_HPP
#define SONGLIST_HPP

#include <QTableView>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QFileInfo>

namespace Khopper {
	
	/**
	 * @brief The song list in the center widget
	 */
	class SongList : public QTableView {
		
		Q_OBJECT
		
		public:
			/**
			 * @brief Default constructor
			 * @param parent Parent widget
			 */
			SongList( QWidget * parent = 0 );
		
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
			 * @brief Mouse drop event handler
			 * @param event Drop event
			 * @todo This drop action don't work
			 */
			virtual void dropEvent( QDropEvent * event );
	};
	
}

#endif
