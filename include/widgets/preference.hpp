#ifndef KHOPPER_PREFERENCE_HPP
#define KHOPPER_PREFERENCE_HPP

#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>

namespace Khopper {

	class Preference : public QDialog {
		Q_OBJECT

	public:
		Preference( QWidget * parent = 0 );

		QString getTemplate() const;

	private slots:
		void perform_( QAbstractButton * );

	private:
		QLineEdit * fnTpl_;
		QDialogButtonBox * buttons_;

		void apply_();
		void revert_();
	};

}

#endif
