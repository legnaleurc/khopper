/**
 * @file error.cpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include "error.hpp"

using namespace khopper::error;

#ifndef Q_CC_MSVC
BaseError::BaseError( int code ) : msg_( QString::fromUtf8( strerror( code ) ) ) {
#else
BaseError::BaseError( int code ) : msg_() {
	wchar_t tmp[1024];
	_wcserror_s( tmp, code );
	this->msg_ = QString::fromWCharArray( tmp );
#endif
}

BaseError::BaseError( const char * msg ) : msg_( QString::fromUtf8( msg ) ) {
}

BaseError::BaseError( const std::string & msg ) : msg_( QString::fromUtf8( msg.c_str() ) ) {
}

BaseError::BaseError( const wchar_t * msg ) : msg_( QString::fromWCharArray( msg ) ) {
}

BaseError::BaseError( const std::wstring & msg ) : msg_( QString::fromStdWString( msg ) ) {
}

BaseError::BaseError( const QString & msg ) : msg_( msg ) {
}

BaseError::~BaseError() throw() {
}

const char * BaseError::what() const throw() {
	return this->msg_.toUtf8().constData();
}

const QString & BaseError::getMessage() const {
	return this->msg_;
}
