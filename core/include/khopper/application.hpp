/**
 * @file application.hpp
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
#ifndef KHOPPER_APPLICATION_HPP
#define KHOPPER_APPLICATION_HPP

#include "config.hpp"

#include <QtGui/QApplication>

#include <memory>

namespace khopper {

	namespace plugin {
		class AbstractPlugin;
	}

	namespace widget {
		class AbstractPanel;
	}

	class ApplicationPrivate;

	/**
	 * @brief GUI application context
	 *
	 * This class provides application-wide services.
	 */
	class KHOPPER_DLL Application : public QApplication {
		Q_OBJECT

	public:
		/**
		 * @brief Constructor
		 * @param argc argument count
		 * @param argv argument array
		 */
		Application( int & argc, char * * argv );

		/**
		 * @brief Add panel to OptionWidget
		 * @param panel a panel create by plugin
		 */
		void addPanel( khopper::widget::AbstractPanel * panel );
		/**
		 * @brief Remove panel from OptionWidget
		 * @param panel which to be removed
		 */
		void removePanel( khopper::widget::AbstractPanel * panel );
		/// Reload all plugins
		void reloadPlugins();
		/**
		 * @brief Get instance of a plugin
		 * @param pluginID the plugin ID
		 */
		plugin::AbstractPlugin * getPluginInstance( const QString & pluginID ) const;

		int getCpuCount() const;

	signals:
		/// Emitted when addPanel called
		void panelAdded( khopper::widget::AbstractPanel * panel );
		/// Emitted when removePanel called
		void panelRemoved( khopper::widget::AbstractPanel * panel );
		/**
		 * @brief Emitted if a error occured
		 * @param title error title
		 * @param message error message
		 */
		void errorOccured( const QString & title, const QString & message );

	private:
		std::shared_ptr< ApplicationPrivate > p_;
	};

	KHOPPER_DLL Application * pApp();

}

#endif
