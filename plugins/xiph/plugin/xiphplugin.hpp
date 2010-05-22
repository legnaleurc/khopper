/**
 * @file xiphplugin.hpp
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
#ifndef KHOPPER_PLUGIN_XIPHPLUGIN_HPP
#define KHOPPER_PLUGIN_XIPHPLUGIN_HPP

#include "khopper/abstractplugin.hpp"

namespace khopper {

	namespace widget {
		class FlacPanel;
		class OggPanel;
	}

	namespace plugin {

		/**
		 * @brief flac option widget
		 */
		class XiphPlugin : public AbstractPlugin {
		public:
			XiphPlugin();
			virtual ~XiphPlugin();

		protected:
			virtual void doInstall( const QFileInfo & fileInfo );
			virtual void doUninstall();

		private:
			widget::FlacPanel * flacPanel_;
			widget::OggPanel * oggPanel_;
		};

	}

}

#endif
