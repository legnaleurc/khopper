/**
 * @file error.cpp
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
#include "util/error.hpp"

namespace khopper {

	namespace error {

		namespace private_ {

			ErrorBase::ErrorBase( const char * msg ) : msg_( QString::fromUtf8( msg ) ) {
			}

			ErrorBase::ErrorBase( const std::string & msg ) : msg_( QString::fromUtf8( msg.c_str() ) ) {
			}

			ErrorBase::ErrorBase( const std::wstring & msg ) : msg_( QString::fromStdWString( msg ) ) {
			}

			ErrorBase::ErrorBase( const QString & msg ) : msg_( msg ) {
			}

			ErrorBase::~ErrorBase() throw() {
			}

			const char * ErrorBase::what() const throw() {
				return this->msg_.toUtf8().constData();
			}

			const QString & ErrorBase::getMessage() const {
				return this->msg_;
			}

		}

	}

}