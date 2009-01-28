#ifndef KHOPPER_UI_ABSTRACTOPTION_HPP
#define KHOPPER_UI_ABSTRACTOPTION_HPP

#include "encoder.hpp"

#include <loki/Factory.h>

#include <QWidget>

namespace Khopper {

	class AbstractOption : public QWidget {
	public:
		AbstractOption( QWidget * parent = 0, Qt::WindowFlags f = 0 );
		virtual ~AbstractOption();

		virtual EncoderSP getEncoder() const = 0;
	};

	typedef Loki::SingletonHolder< Loki::Factory< AbstractOption, std::string >, Loki::CreateUsingNew, Loki::LongevityLifetime::DieAsSmallObjectChild > UIFactory;

}

#endif
