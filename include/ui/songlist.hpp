/**
 * @file songlist.hpp
 */
#ifndef SONGLIST_HPP
#define SONGLIST_HPP

#include <QTableView>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

namespace Khopper {
	
	/**
	 * @brief The song list view in the center widget
	 */
	class SongListView : public QTableView {
		
		Q_OBJECT
		
		public:
			/**
			 * @brief Default constructor
			 * @param parent Parent widget
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
			 * @brief Mouse drop event handler
			 * @param event Drop event
			 * @todo This drop action don't work
			 */
			virtual void dropEvent( QDropEvent * event );
	};
	
}

#endif
