/**
 * @file threads.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KHOPPER_THREADS_HPP
#define KHOPPER_THREADS_HPP

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
		void setTracks( const std::vector< TrackSP > & tracks );

	signals:
		/**
		 * @brief Send error message
		 * @param title Message box title
		 * @param errMsg Error message
		 */
		void error( const QString & title, const QString & errMsg );

		/**
		 * @brief Progress notifyer
		 * @param duration Convert duration
		 */
		void step( int duration );

	protected:
		/**
		 * @brief Action function
		 */
		virtual void run();

	private:
		EncoderSP encoder_;
		std::vector< TrackSP > tracks_;
		std::vector< QString > paths_;
		QString outDir_;
	};

}

#endif
