#ifndef SONGLIST_HPP
#define SONGLIST_HPP

#include <QTableView>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QFileInfo>

namespace Khopper {
	
	class SongList : public QTableView {
		
		Q_OBJECT
		
		public:
			SongList( QWidget * parent = 0 );
		
		signals:
			void openFile( const QString & );
		
		protected:
			virtual void dragEnterEvent( QDragEnterEvent * event );
			virtual void dropEvent( QDropEvent * event );
	};
	
}

#endif
