/**
 * @file cuesheet.cpp
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
#include "cuesheet.hpp"

using namespace khopper::album;

const QStringList & CueSheet::getComments() const {
	return this->comments_;
}

const QSet< QString > & CueSheet::getFlags() const {
	return this->flags_;
}

const QStringList & CueSheet::getGarbage() const {
	return this->garbage_;
}

void CueSheet::setCatalog( const QString & catalog ) {
	this->catalog_ = catalog;
}

void CueSheet::setCDTextFile( const QString & cdTextFile ) {
	this->cdTextFile_ = cdTextFile;
}

void CueSheet::setComments( const QStringList & comments ) {
	this->comments_ = comments;
}

void CueSheet::setFlags( const QSet< QString > & flags ) {
	this->flags_ = flags_;
}

void CueSheet::setGarbage( const QStringList & garbage ) {
	this->garbage_ = garbage;
}
