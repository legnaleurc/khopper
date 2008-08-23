/**
 * @file threads.hpp
 */
#ifndef THREADS_HPP
#define THREADS_HPP

#include "cue/track.hpp"
#include "types.hpp"
#include <boost/shared_ptr.hpp>

#include <QThread>

namespace Khopper {

	class Converter;
	
	/**
	 * @brief Thread of converter
	 */
	class ConverterThread : public QThread {
		
		Q_OBJECT
		
		public:
			/**
			 * @brief Default constructor
			 * @param parent parent object
			 */
			ConverterThread( QObject * parent = 0 );

			/**
			 * @brief Set indeice of song to be convert
			 * @param [in] track Index array
			 */
			void setTracks( const std::vector< Track > & tracks );

			/**
			 * @brief Set output format
			 * @param [in] output Output format object
			 */
			void setOutput( const OutputSP & output );
			
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
			boost::shared_ptr< Converter > converter_;
			std::vector< Track > tracks_;
			std::vector< QString > paths_;
	};
	
}

#endif
