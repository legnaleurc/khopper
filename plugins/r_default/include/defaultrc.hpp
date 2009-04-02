/**
 * @file defaultrc.hpp
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
#ifndef KHOPPER_CODEC_DEFAULTRC_HPP
#define KHOPPER_CODEC_DEFAULTRC_HPP

#include "readerplugin.hpp"

namespace khopper {

	namespace plugin {

		/// Default reader creator
		class DefaultReaderCreator : public QObject, public ReaderCreator {
			Q_OBJECT
			Q_INTERFACES( khopper::plugin::ReaderCreator )

			/// Creates reader object
			virtual codec::ReaderSP create_() const;
		};

	}

}

#endif
