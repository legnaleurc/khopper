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
#include "baseerror.hpp"

namespace khopper {
namespace error {
class BaseError::Private {
public:
	Private( const QString & msg, const std::string & file, long line );

	QString msg;
	std::string file;
	long line;
};
}
}

using khopper::error::BaseError;

BaseError::Private::Private( const QString & msg, const std::string & file, long line ):
msg( msg ),
file( file ),
line( line ) {
}

BaseError::BaseError( int code, const char * file, long line ):
#ifndef Q_CC_MSVC
p_( new Private( QString::fromUtf8( strerror( code ) ), file, line ) ) {
#else
p_( new Private( "", file, line ) ) {
	wchar_t tmp[1024];
	_wcserror_s( tmp, code );
	this->msg_ = QString::fromWCharArray( tmp );
#endif
}

BaseError::BaseError( const char * msg, const char * file, long line ):
p_( new Private( QString::fromUtf8( msg ), file, line ) ) {
}

BaseError::BaseError( const std::string & msg, const char * file, long line ):
p_( new Private( QString::fromUtf8( msg.c_str() ), file, line ) ) {
}

BaseError::BaseError( const wchar_t * msg, const char * file, long line ):
p_( new Private( QString::fromWCharArray( msg ), file, line ) ) {
}

BaseError::BaseError( const std::wstring & msg, const char * file, long line ):
p_( new Private( QString::fromStdWString( msg ), file, line ) ) {
}

BaseError::BaseError( const QString & msg, const char * file, long line ):
p_( new Private( msg, file, line ) ) {
}

const char * BaseError::what() const throw() {
	return this->p_->msg.toUtf8().constData();
}

const QString & BaseError::getMessage() const {
	return this->p_->msg;
}
