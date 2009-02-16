/**
 * @file track.cpp
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
#include "track.hpp"
#include "decoder.hpp"
#include "text.hpp"
#include "error.hpp"

#include <QtDebug>

#include <sstream>

namespace {

	std::vector< std::wstring > createHeader() {
		std::vector< std::wstring > vs;
		vs.push_back( L"Title" );
		vs.push_back( L"Performer" );
		vs.push_back( L"Album" );
		vs.push_back( L"Duration" );
		vs.push_back( L"Bit Rate" );
		vs.push_back( L"Sample Rate" );
		vs.push_back( L"Channels" );
		return vs;
	}

}

namespace Khopper {

	Track::Track():
	album(),
	artist(),
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
	postGap(),
	preGap(),
	sampleRate(),
	songWriter(),
	startTime(),
	title() {
	}

	void Track::load( const std::wstring & filePath ) {
		this->filePath = filePath;

		DecoderSP decoder( new Decoder );
		decoder->open( this->filePath );
		if( decoder->is_open() ) {
			this->album = text::fromUTF8( decoder->getAlbum() );
			this->artist = text::fromUTF8( decoder->getArtist() );
			this->bitRate = decoder->getBitRate();
			this->channels = decoder->getChannels();
			this->duration = decoder->getDuration();
			this->sampleRate = decoder->getSampleRate();
			this->title = text::fromUTF8( decoder->getTitle() );

			decoder->close();
		} else {
			throw Error< Codec >( "Can not open file!" );
		}
	}

	std::wstring Track::toStdWString() const {
		std::wostringstream sout;
		sout << L"Title:\t" << this->title << L"\n";
		sout << L"Artist:\t" << this->artist << L"\n";
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
