/**
 * @file runtimeerror.cpp
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
#include "runtimeerror.hpp"

using khopper::error::RunTimeError;

RunTimeError::RunTimeError( int code, const char * file, long line ):
BaseError( code, file, line ) {
}

RunTimeError::RunTimeError( const char * msg, const char * file, long line ):
BaseError( msg, file, line ) {
}

RunTimeError::RunTimeError( const std::string & msg, const char * file, long line ):
BaseError( msg, file, line ) {
}

RunTimeError::RunTimeError( const wchar_t * msg, const char * file, long line ):
BaseError( msg, file, line ) {
}

RunTimeError::RunTimeError( const std::wstring & msg, const char * file, long line ):
BaseError( msg, file, line ) {
}

RunTimeError::RunTimeError( const QString & msg, const char * file, long line ):
BaseError( msg, file, line ) {
}

RunTimeError::~RunTimeError() throw() {
}
