/**
 * @file textcodec.hpp
 */
#ifndef KHOPPER_UI_TEXTCODEC_HPP
#define KHOPPER_UI_TEXTCODEC_HPP

#include <QDialog>

QT_BEGIN_NAMESPACE
class QByteArray;
class QComboBox;
class QDialogButtonBox;
QT_END_NAMESPACE

namespace Khopper {

	class TextCodec : public QDialog {

		Q_OBJECT

		public:
			TextCodec( QWidget * parent = 0, Qt::WindowFlags f = 0 );
			void setEncoded( const QByteArray & encoded );
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
