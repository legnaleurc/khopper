/**
 * @file youtubeplugin.hpp
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
#ifndef KHOPPER_PLUGIN_YOUTUBEPLUGIN_HPP
#define KHOPPER_PLUGIN_YOUTUBEPLUGIN_HPP

#include "khopper/abstractplugin.hpp"

class QProgressDialog;

namespace khopper {

	namespace widget {
		class YouTubeDialog;
	}

	namespace plugin {

		class YouTubePlugin : public AbstractPlugin {
		public:
			YouTubePlugin();

			std::shared_ptr< widget::YouTubeDialog > getDialog() const;
			std::shared_ptr< QProgressDialog > getProgress() const;

		protected:
			virtual void doInstall();
			virtual void doUninstall();

		private:
			std::shared_ptr< widget::YouTubeDialog > dialog_;
			std::shared_ptr< QProgressDialog > progress_;
		};

	}

}

#endif
