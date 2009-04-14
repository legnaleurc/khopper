/**
 * @file os.hpp
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
#ifndef KHOPPER_OS_HPP
#define KHOPPER_OS_HPP

#include "common/tr1.hpp"

#include <QDir>

namespace khopper {

	/**
	 * @brief Operating System dependant system code
	 */
	namespace os {

		/**
		 * @brief Join paths to one path
		 * @param [in] front front part
		 * @param [in] back back part
		 * @return Complete path
		 */
		KHOPPER_DLL_EXPORT QString join( const QString & front, const QString & back );

	}

	namespace plugin {

		/**
		 * @brief Plug-in context
		 */
		class KHOPPER_DLL_EXPORT PluginContext {
		public:
			/// Default constructor
			PluginContext();

			/// get plugin's directory
			const QDir & getDir() const;
			/**
			 * @brief Load plugin by plugin name
			 * @param name plugin name
			 */
			QObject * load( QString name ) const;
		private:
			QDir d_;
		};

	}

}

#endif
