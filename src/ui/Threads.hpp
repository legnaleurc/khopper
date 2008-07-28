/**
 * @file Threads.hpp
 */
#ifndef THREADS_HPP
#define THREADS_HPP

#include "converter.hpp"
#include "cuesheet.hpp"

#include <QThread>
#include <QFileInfo>

namespace Khopper {
	
	/**
	 * @brief Thread of converter
	 */
	class ConverterThread : public QThread {
		
		Q_OBJECT
		
		public:
			/**
			 * @brief Initialize with a converter
			 * @param [in] conv Converter object
			 * @param parent parent object
			 */
			ConverterThread( QObject * parent = 0 );
			
			void setSheet( const CUESheet & sheet );
			
			void setIndex( const std::vector< int > & track );
			
			void setOutput( const OutputSP & output );
			
		signals:
			void error( const QString & );
			void step( int );
		
		protected:
			virtual void run();
		
		private:
			OutputSP output_;
			CUESheet sheet_;
			std::vector< int > index_;
	};
	
}

#endif
