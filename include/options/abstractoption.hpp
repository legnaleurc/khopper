/**
 * @file abstractoption.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KHOPPER_ABSTRACTOPTION_HPP
#define KHOPPER_ABSTRACTOPTION_HPP

#include "encoder.hpp"

#include <loki/Factory.h>

#include <QWidget>

namespace Khopper {

	/**
	 * @brief Abstract option widget
	 *
	 * All output format option will inherit this class, \n
	 * and must register to factory. \n
	 */
	class AbstractOption : public QWidget {
	public:
		/**
		 * @brief Default constructor
		 * @param parent Parent widget
		 * @param f Window flags
		 */
		AbstractOption( QWidget * parent = 0, Qt::WindowFlags f = 0 );
		/**
		 * @brief Virtual destructor
		 */
		virtual ~AbstractOption();

		/**
		 * @brief Get encoder setting object
		 * @return Smart pointer contains Encoder
		 */
		virtual EncoderSP getEncoder() const = 0;
	};

	/**
	 * @brief AbstractOption factory
	 *
	 * Register by yourself.
	 */
	typedef Loki::SingletonHolder< Loki::Factory< AbstractOption, std::string >, Loki::CreateUsingNew, Loki::LongevityLifetime::DieAsSmallObjectChild > UIFactory;

}

#endif
