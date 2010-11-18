#ifndef KHOPPER_WIDGET_URIINPUTDIALOG_HPP
#define KHOPPER_WIDGET_URIINPUTDIALOG_HPP

#include <QtCore/QUrl>
#include <QtGui/QDialog>

#include <memory>

namespace Ui {
	class UriInputDialog;
}

namespace khopper {
	namespace widget {

		class UriInputDialog : public QDialog {
		public:
			explicit UriInputDialog( QWidget * parent );

			QList< QUrl > getUriList() const;

		protected:
			virtual void setVisible( bool visible );

		private:
			std::shared_ptr< Ui::UriInputDialog > ui_;
		};

	}
}

#endif
