/**
 * @file mp3option.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KHOPPER_MP3OPTION_HPP
#define KHOPPER_MP3OPTION_HPP

#include "abstractoption.hpp"

#include <QComboBox>

namespace Khopper {

	/**
	 * @brief Mp3 option widget
	 */
	class MP3Option : public AbstractOption {
	public:
		/**
		 * @brief Default constructor
		 * @param parent Parent widget
		 * @param f Window flags
		 */
		MP3Option( QWidget * parent = 0, Qt::WindowFlags f = 0 );

		/**
		 * @brief Get Encoder setting
		 * @return Smart pointer to Encoder
		 */
		virtual EncoderSP getEncoder() const;

	private:
		QComboBox * bitRate_;
		QComboBox * sampleRate_;
		QComboBox * channels_;
	};

}

#endif
