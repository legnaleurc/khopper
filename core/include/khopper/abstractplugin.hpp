/**
 * @file abstractplugin.hpp
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
#ifndef KHOPPER_PLUGIN_ABSTRACTPLUGIN_HPP
#define KHOPPER_PLUGIN_ABSTRACTPLUGIN_HPP

#include "tr1.hpp"

#include <QtCore/QFileInfo>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

namespace khopper {
	namespace plugin {

		/**
		 * @brief Interface of plugins
		 *
		 * This interface is used to define some properties of plugins.
		 * Clients usally need not to inherit this class, because
		 * AbstractPanel, AbstractReaderCreator and AbstractWriterCreator do so.
		 * Use them instead of this interface.
		 */
		class KHOPPER_DLL AbstractPlugin : public QObject {
		public:
			AbstractPlugin();

			void install( const QFileInfo & fileInfo );
			void uninstall();
			bool isInstalled() const;

			const QFileInfo & getFileInfo() const;
			/// get the id of the plugin, must be unique
			const QString & getID() const;
			/// get version string of the plugin
			const QString & getVersion() const;

		protected:
			void setID( const QString & id );
			void setVersion( const QString & version );

			virtual void doInstall() = 0;
			virtual void doUninstall() = 0;

		private:
			AbstractPlugin( const AbstractPlugin & );
			AbstractPlugin & operator =( const AbstractPlugin & );

			struct AbstractPluginPrivate;
			AbstractPluginPrivate * p_;
		};

	}
}

Q_DECLARE_INTERFACE( khopper::plugin::AbstractPlugin, "org.FoolproofProject.Khopper.Plugin/0.2" )

#endif
