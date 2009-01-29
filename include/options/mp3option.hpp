/**
 * @file mp3option.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KHOPPER_MP3OPTION_HPP
#define KHOPPER_MP3OPTION_HPP

#include "abstractoption.hpp"

#include <QComboBox>

namespace Khopper {

	class MP3Option : public AbstractOption {
	public:
		MP3Option( QWidget * parent = 0, Qt::WindowFlags f = 0 );

		virtual EncoderSP getEncoder() const;

	private:
		QComboBox * bitRate_;
		QComboBox * sampleRate_;
		QComboBox * channels_;
	};

}

#endif
