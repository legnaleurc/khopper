#include "mp3option.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QVariant>
#include <QtDebug>

namespace {

	Khopper::MP3Option * CreateMp3() {
		return new Khopper::MP3Option;
	}

	const char * const MP3 = "mp3";

	const bool reg_type = Khopper::EncoderList::Instance().insert( Khopper::EncoderList::ObjectType::value_type( MP3, "mp3" ) ).second;
	const bool reg_fac = Khopper::UIFactory::Instance().Register( MP3, CreateMp3 );

}

namespace Khopper {

	MP3Option::MP3Option( QWidget * parent, Qt::WindowFlags f ):
	AbstractOption( parent, f ),
	brChoise_( new QButtonGroup( this ) ),
	bitRate_( new QComboBox( this ) ),
	level_( new QComboBox( this ) ),
	sampleRate_( new QComboBox( this ) ),
	channels_( new QComboBox( this ) ) {
		QVBoxLayout * vbox = new QVBoxLayout( this );
		this->setLayout( vbox );

		QGroupBox * brGroup = new QGroupBox( tr( "Bit Rate" ), this );
		vbox->addWidget( brGroup );
		QVBoxLayout * brBox = new QVBoxLayout( brGroup );
		brGroup->setLayout( brBox );

		QHBoxLayout * cbrBox = new QHBoxLayout();
		brBox->addLayout( cbrBox );
		QRadioButton * cbr = new QRadioButton( tr( "CBR" ), this );
		cbrBox->addWidget( cbr );
		this->bitRate_->addItem( "64", QVariant( 64000 ) );
		this->bitRate_->addItem( "128", QVariant( 128000 ) );
		this->bitRate_->addItem( "256", QVariant( 256000 ) );
		this->bitRate_->addItem( "320", QVariant( 320000 ) );
		this->bitRate_->setCurrentIndex( 3 );
		cbrBox->addWidget( this->bitRate_ );

		QHBoxLayout * vbrBox = new QHBoxLayout();
		brBox->addLayout( vbrBox );
		QRadioButton * vbr = new QRadioButton( tr( "VBR" ), this );
		vbrBox->addWidget( vbr );
		this->level_->addItem( "0 (Highest)", QVariant( 0 ) );
		for( int i = 1; i < 9; ++i ) {
			this->level_->addItem( QString::number( i ), QVariant( i ) );
		}
		this->level_->addItem( "9 (Lowest)", QVariant( 9 ) );
		vbrBox->addWidget( this->level_ );

		this->brChoise_->addButton( cbr );
		this->brChoise_->setId( cbr, 0 );
		this->brChoise_->addButton( vbr );
		this->brChoise_->setId( vbr, 1 );
		connect( cbr, SIGNAL( toggled( bool ) ), this->bitRate_, SLOT( setEnabled( bool ) ) );
		connect( vbr, SIGNAL( toggled( bool ) ), this->level_, SLOT( setEnabled( bool ) ) );
		cbr->setChecked( true );
		this->level_->setEnabled( false );

		vbox->addWidget( this->sampleRate_ );
		this->sampleRate_->addItem( "44100", QVariant( 44100 ) );
		this->sampleRate_->addItem( "48000", QVariant( 48000 ) );

		vbox->addWidget( this->channels_ );
		this->channels_->addItem( "Mono", QVariant( 1 ) );
		this->channels_->addItem( "Streao", QVariant( 2 ) );
		this->channels_->setCurrentIndex( 1 );
	}

	EncoderSP MP3Option::getEncoder() const {
		Encoder * encoder = new Encoder;

		switch( this->brChoise_->checkedId() ) {
		case 0:
			encoder->setBitRate( this->bitRate_->itemData( this->bitRate_->currentIndex() ).toInt() );
			break;
		case 1:
			encoder->setQuality( this->level_->itemData( this->level_->currentIndex() ).toInt() );
			break;
		default:
			;
		}
		encoder->setSampleRate( this->sampleRate_->itemData( this->sampleRate_->currentIndex() ).toInt() );
		encoder->setChannels( this->channels_->itemData( this->channels_->currentIndex() ).toInt() );

		return EncoderSP( encoder );
	}

}
