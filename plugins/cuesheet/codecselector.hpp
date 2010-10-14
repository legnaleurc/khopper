/**
 * @file codecselector.hpp
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
#ifndef KHOPPER_WIDGET_CODECSELECTOR_HPP
#define KHOPPER_WIDGET_CODECSELECTOR_HPP

#include "khopper/tr1.hpp"

#include <QtGui/QDialog>

#include <memory>

namespace Ui {
	class CodecSelector;
}

namespace khopper {

	namespace widget {

		/**
		 * @brief Decode string dialog
		 */
		class CodecSelector : public QDialog {
			Q_OBJECT

		public:
			static QString selectTextCodec( const QByteArray & encoded );

		private slots:
			void update_( int );

		private:
			/**
			 * @brief Default constructor
			 * @param parent Parent widget
			 * @param f Window flags
			 */
			explicit CodecSelector( const QByteArray & encoded );

			std::shared_ptr< Ui::CodecSelector > ui_;
			QByteArray encoded_;
			QString decoded_;
		};

	}

}

#endif
