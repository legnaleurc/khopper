#include "mp3option.hpp"

#include <QVBoxLayout>
#include <QVariant>

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
	bitRate_( new QComboBox( this ) ),
	sampleRate_( new QComboBox( this ) ),
	channels_( new QComboBox( this ) ) {
		QVBoxLayout * vbox = new QVBoxLayout( this );

		vbox->addWidget( this->bitRate_ );
		this->bitRate_->addItem( "64", QVariant( 64000 ) );
		this->bitRate_->addItem( "128", QVariant( 128000 ) );
		this->bitRate_->addItem( "256", QVariant( 256000 ) );
		this->bitRate_->addItem( "320", QVariant( 320000 ) );
		this->bitRate_->setCurrentIndex( 3 );

		vbox->addWidget( this->sampleRate_ );
		this->sampleRate_->addItem( "44100", QVariant( 44100 ) );

		vbox->addWidget( this->channels_ );
		this->channels_->addItem( "Mono", QVariant( 1 ) );
		this->channels_->addItem( "Streao", QVariant( 2 ) );
		this->channels_->setCurrentIndex( 1 );
	}

	EncoderSP MP3Option::getEncoder() const {
		EncoderSP encoder( new Encoder );
		encoder->setBitRate( this->bitRate_->itemData( this->bitRate_->currentIndex() ).toInt() );
		encoder->setSampleRate( this->sampleRate_->itemData( this->sampleRate_->currentIndex() ).toInt() );
		encoder->setChannels( this->channels_->itemData( this->channels_->currentIndex() ).toInt() );
		return encoder;
	}

}
