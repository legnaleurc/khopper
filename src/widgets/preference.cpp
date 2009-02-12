#include "preference.hpp"

#include <QRegExp>
#include <QSettings>
#include <QGroupBox>
#include <QVBoxLayout>

namespace Khopper {

	Preference::Preference( QWidget * parent ):
	QDialog( parent ),
	fnTpl_( new QLineEdit( this ) ),
	buttons_( new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel, Qt::Horizontal, this ) ) {
		QSettings setting;
		setting.beginGroup( "preference" );

		this->setWindowTitle( tr( "Preference" ) );

		QVBoxLayout * mainBox = new QVBoxLayout( this );
		this->setLayout( mainBox );

		QGroupBox * fnGroup = new QGroupBox( tr( "File name template" ), this );
		mainBox->addWidget( fnGroup );

		QVBoxLayout * fnBox = new QVBoxLayout( fnGroup );
		fnGroup->setLayout( fnBox );

		// Output name template
		this->fnTpl_->setText( setting.value( "filename", "%2i_%t" ).toString() );
		this->fnTpl_->setToolTip( tr(
			"Keywords:\n"
			"%t: title\n"
			"%a: artist\n"
			"%Ni: index of track, N means width\n"
			"%%: \'%\' literal\n"
			"Extensions will automaticaly append."
		) );
		fnBox->addWidget( this->fnTpl_ );

		mainBox->addWidget( this->buttons_ );
		connect( this->buttons_, SIGNAL( clicked( QAbstractButton * ) ), this, SLOT( perform_( QAbstractButton * ) ) );

		setting.endGroup();
	}

	QString Preference::getTemplate() const {
		QString tmp = this->fnTpl_->text();
		tmp.replace( "%t", "%1%" );
		tmp.replace( "%a", "%2%" );
		tmp.replace( QRegExp( "%(\\d*)i" ), "%|3$0\\1|" );
		tmp.replace( "%%", "%" );
		return tmp;
	}

	void Preference::perform_( QAbstractButton * button ) {
		switch( this->buttons_->buttonRole( button ) ) {
		case QDialogButtonBox::AcceptRole:
			this->apply_();
			this->accept();
			break;
		case QDialogButtonBox::RejectRole:
			this->revert_();
			this->reject();
			break;
		case QDialogButtonBox::ApplyRole:
			this->apply_();
			break;
		default:
			;
		};
	}

	void Preference::apply_() {
		QSettings setting;
		setting.beginGroup( "preference" );

		setting.setValue( "filename", this->fnTpl_->text() );

		setting.endGroup();
	}

	void Preference::revert_() {
		QSettings setting;
		setting.beginGroup( "preference" );

		this->fnTpl_->setText( setting.value( "filename", "%2i_%t" ).toString() );

		setting.endGroup();
	}

}
