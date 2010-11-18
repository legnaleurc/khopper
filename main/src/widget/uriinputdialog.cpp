#include "uriinputdialog.hpp"
#include "ui_uriinputdialog.h"

#include <QtCore/QTextStream>

using namespace khopper::widget;

UriInputDialog::UriInputDialog( QWidget * parent ):
QDialog( parent ),
ui_( new Ui::UriInputDialog ) {
	this->ui_->setupUi( this );
}

void UriInputDialog::setVisible( bool visible ) {
	this->QDialog::setVisible( visible );
	if( visible ) {
		this->ui_->plainTextEdit->clear();
	}
}

QList< QUrl > UriInputDialog::getUriList() const {
	QString plainText( this->ui_->plainTextEdit->toPlainText() );
	QList< QUrl > uris;
	QTextStream sin( &plainText, QIODevice::ReadOnly | QIODevice::Text );
	for( QString line( sin.readLine() ); !sin.atEnd(); line = sin.readLine() ) {
		QUrl tmp( QUrl::fromUserInput( line ) );
		if( tmp.isValid() ) {
			uris.push_back( tmp );
		}
	}
	return uris;
}
