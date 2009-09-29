#ifndef KHOPPER_PLUGIN_ABSTRACTPLUGIN_HPP
#define KHOPPER_PLUGIN_ABSTRACTPLUGIN_HPP

#include <QString>

namespace khopper {

	namespace plugin {

		class AbstractPlugin {
		public:
			virtual ~AbstractPlugin() {}

			virtual QString getID() const = 0;
			virtual QString getVersion() const = 0;
		};

	}

}

#endif
