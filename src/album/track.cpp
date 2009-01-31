#include "track.hpp"
#include "error.hpp"

#include <QtDebug>

#include <sstream>

namespace {

	std::vector< std::wstring > createHeader() {
		std::vector< std::wstring > vs;
		vs.push_back( L"Title" );
		vs.push_back( L"Performer" );
		vs.push_back( L"Duration" );
		vs.push_back( L"Bit Rate" );
		vs.push_back( L"Sample Rate" );
		vs.push_back( L"Channels" );
		return vs;
	}

}

namespace Khopper {

	Track::Track():
	bitRate( 0 ),
	channels( 0 ),
	comments(),
	dataType( AUDIO ),
	duration(),
	filePath(),
	fileType( BINARY ),
	flags( NONE ),
	garbage(),
	index( 0 ),
	isrc(),
	performer(),
	postGap(),
	preGap(),
	sampleRate(),
	songWriter(),
	startTime(),
	title(),
	decoder_( new Decoder ) {
	}

	void Track::convert( const std::wstring & targetPath, EncoderSP encoder ) const {
		this->decoder_->open( this->filePath );
		encoder->open( targetPath );

		if( !this->decoder_->is_open() || !encoder->is_open() ) {
			throw Error< RunTime >( "Can not open decoder or encoder!" );
		}

		double begin = this->startTime.toDouble();
		double end = begin + this->duration.toDouble();
		this->decoder_->setRange( begin, end );

		double decoded;
		while( this->decoder_->hasNext() ) {
			encoder->write( this->decoder_->read( decoded ) );
			emit this->decodedTime( static_cast< int >( decoded * 100 ) );
		}

		encoder->close();
		this->decoder_->close();
	}

	std::wstring Track::toStdWString() const {
		std::wostringstream sout;
		sout << L"Title:\t" << this->title << L"\n";
		sout << L"Performer:\t" << this->performer << L"\n";
		sout << L"Song Writer:\t" << this->songWriter << L"\n";
		sout << L"Track index:\t" << this->index << L"\n";
		sout << L"ISRC:\t" << this->isrc << L"\n";
		sout << L"Start:\t" << this->startTime.toStdWString() << L"\n";
		sout << L"End:\t" << this->duration.toStdWString() << L"\n";
		sout << L"Pregap:\t" << this->preGap.toStdWString() << L"\n";
		sout << L"Postgap:\t" << this->postGap.toStdWString() << L"\n";
		sout << L"Type:\t" << this->dataType << L"\n";
		sout << L"Comments:\n";
		for( std::map< std::wstring, std::wstring >::const_iterator it = this->comments.begin(); it != this->comments.end(); ++it ) {
			sout << L"\t" << it->first << L":\t" << it->second << L"\n";
		}
		sout << L"Garbage:\n";
		for( std::vector< std::wstring >::const_iterator it = this->garbage.begin(); it != this->garbage.end(); ++it ) {
			sout << L"\t" << *it << L"\n";
		}
		sout << L"Flags:\t" << this->flags << L"\n";
		return sout.str();
	}

	const std::vector< std::wstring > Track::Headers = createHeader();

}
