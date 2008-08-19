/**
 * @file threads.hpp
 */
#ifndef THREADS_HPP
#define THREADS_HPP

#include "cue/cuesheet.hpp"
#include "types.hpp"

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
			 * @param parent parent object
			 */
			ConverterThread( QObject * parent = 0 );

			/**
			 * @brief Set a CUE sheet to this thread
			 * @param [in] sheet CUE Sheet
			 */
			void setSheet( const CUESheet & sheet );

			/**
			 * @brief Set indeice of song to be convert
			 * @param [in] track Index array
			 */
			void setIndex( const std::vector< int > & track );

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
			OutputSP output_;
			CUESheet sheet_;
			std::vector< int > index_;
	};
	
}

#endif