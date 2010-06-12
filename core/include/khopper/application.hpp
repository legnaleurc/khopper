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

#include "tr1.hpp"

#include <QtGui/QApplication>

namespace khopper {

	namespace plugin {
		class AbstractPlugin;
	}

	namespace widget {
		class AbstractPanel;
	}

	class KHOPPER_DLL Application : public QApplication {
		Q_OBJECT

	public:
		Application( int & argc, char * * argv );

		void addPanel( khopper::widget::AbstractPanel * panel );
		void removePanel( khopper::widget::AbstractPanel * panel );
		void reloadPlugins();
		plugin::AbstractPlugin * getPluginInstance( const QString & pluginID ) const;

	signals:
		void panelAdded( khopper::widget::AbstractPanel * panel );
		void panelRemoved( khopper::widget::AbstractPanel * panel );

	private:
		struct ApplicationPrivate;
		std::tr1::shared_ptr< ApplicationPrivate > p_;
	};

}

#define KHOPPER_APPLICATION static_cast< khopper::Application * >( QCoreApplication::instance() )

#endif
