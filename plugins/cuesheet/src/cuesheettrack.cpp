/**
 * @file cuesheettrack.cpp
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
#include "cuesheettrack.hpp"
#include "rangedreader.hpp"

using khopper::album::CueSheetTrack;
using khopper::album::Timestamp;
using khopper::codec::ReaderSP;
using khopper::codec::RangedReader;

CueSheetTrack::CueSheetTrack( const QUrl & uri ):
Track( uri, [this]( const QUrl & uri )->ReaderSP {
	return ReaderSP( new RangedReader( uri, this->getStartTime().toMillisecond(), this->getDuration().toMillisecond() ) );
} ) {
}

// TODO: not implemented
void CueSheetTrack::save() const {
	return;
}

const QStringList & CueSheetTrack::getComments() const {
	return this->comments_;
}

const QStringList & CueSheetTrack::getGarbage() const {
	return this->garbage_;
}

const Timestamp & CueSheetTrack::getStartTime() const {
	return this->startTime_;
}

void CueSheetTrack::setComments( const QStringList & comments ) {
	this->comments_ = comments;
}

void CueSheetTrack::setDataType( const QString & dataType ) {
	this->dataType_ = dataType;
}

void CueSheetTrack::setFileType( const QString & fileType ) {
	this->fileType_ = fileType;
}

void CueSheetTrack::setGarbage( const QStringList & garbage ) {
	this->garbage_ = garbage;
}

void CueSheetTrack::setISRC( const QString & isrc ) {
	this->isrc_ = isrc;
}

void CueSheetTrack::setPostgap( const Timestamp & postgap ) {
	this->postgap_ = postgap;
}

void CueSheetTrack::setPregap( const Timestamp & pregap ) {
	this->pregap_ = pregap;
}

void CueSheetTrack::setStartTime( const Timestamp & startTime ) {
	this->startTime_ = startTime;
}
