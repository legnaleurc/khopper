/**
 * @file threads.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KHOPPER_UI_THREADS_HPP
#define KHOPPER_UI_THREADS_HPP

#include "track.hpp"
#include "encoder.hpp"

#include <QThread>

namespace Khopper {

	/**
	 * @brief Thread of converter
	 */
	class ConverterThread : public QThread {
		Q_OBJECT

	public:
		/**
		 * @brief Default constructor
		 * @param [in] parent parent object
		 */
		ConverterThread( QObject * parent = 0 );

		/**
		 * @brief Set output option
		 * @param [in] output Encoder object
		 * @param [in] outDir Output directory
		 */
		void setOutput( EncoderSP output, const QString & outDir );

		/**
		 * @brief Set tracks to convert
		 * @param [in] tracks Tracks information
		 */
		void setTracks( const std::vector< Track > & tracks );

	signals:
		/**
		 * @brief Send error message
		 * @param errMsg Error message
		 */
		void error( const QString & errMsg );

		/**
		 * @brief Progress notifyer
		 * @param task Task number
		 */
		void step( int task );

	protected:
		/**
		 * @brief Action function
		 */
		virtual void run();

	private:
		EncoderSP encoder_;
		std::vector< Track > tracks_;
		std::vector< QString > paths_;
		QString outDir_;
	};

}

#endif
