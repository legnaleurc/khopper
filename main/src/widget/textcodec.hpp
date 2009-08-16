/**
 * @file textcodec.hpp
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
#ifndef KHOPPER_WIDGET_TEXTCODEC_HPP
#define KHOPPER_WIDGET_TEXTCODEC_HPP

#include <QDialog>
#include <QComboBox>
#include <QLabel>

namespace khopper {

	namespace widget {

		/**
		 * @brief Decode string dialog
		 */
		class TextCodec : public QDialog {
			Q_OBJECT

		public:
			/**
			 * @brief Default constructor
			 * @param parent Parent widget
			 * @param f Window flags
			 */
			TextCodec( QWidget * parent = 0, Qt::WindowFlags f = 0 );
			/**
			 * @brief Set encoded string
			 * @param [in] encoded Encoded string
			 */
			void setEncoded( const QByteArray & encoded );
			/**
			 * @brief Get decoded string
			 * @return Decoded string, in Unicode
			 */
			QString getDecoded() const;

		private slots:
			void update( int );

		private:
			QByteArray encoded_;
			QString decoded_;

			QComboBox * codecs_;
			QLabel * contents_;
		};

	}

}

#endif
