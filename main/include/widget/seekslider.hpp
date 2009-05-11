#ifndef KHOPPER_WIDGET_SEEKSLIDER_HPP
#define KHOPPER_WIDGET_SEEKSLIDER_HPP

#include <QSlider>
#include <Phonon>

namespace khopper {

	namespace widget {

		class SeekSlider : public QWidget {
		public:
			SeekSlider( Phonon::MediaObject * media, QWidget * parent = 0 );

			void setMediaObject( Phonon::MediaObject * media );

			void setRealRange( double begin, double end );

		private slots:
			void changeState_( Phonon::State );
			void seek_( int );
			void tick_( qint64 );

		private:
			void play_();

			Phonon::MediaObject * media_;
			QSlider * slider_;
			bool tracking_;
			qint64 begin_;
			qint64 end_;
		};

	}

}

#endif
