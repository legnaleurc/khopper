/**
 * @file textcodec.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KHOPPER_UI_TEXTCODEC_HPP
#define KHOPPER_UI_TEXTCODEC_HPP

#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>

namespace Khopper {

	class TextCodec : public QDialog {
		Q_OBJECT

	public:
		/**
		 * @brief Default constructor
		 */
		TextCodec( QWidget * parent = 0, Qt::WindowFlags f = 0 );
		/**
		 * @brief Set encoded string
		 * @param [in] encoded Encoded string
		 */
		void setEncoded( const QByteArray & encoded );
		/**
		 * @brief Get decoded string
		 * @return Decoded string, in Unicode
		 */
		QString getDecoded() const;

	private slots:
		void update( int );

	private:
		QByteArray encoded_;
		QString decoded_;

		QComboBox * codecs_;
		QDialogButtonBox * buttons_;
	};

}

#endif
