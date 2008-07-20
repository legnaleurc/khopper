/**
 * @file Threads.hpp
 */
#ifndef THREADS_HPP
#define THREADS_HPP

#include "converter.hpp"

#include <QThread>

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
			
			void setAudio( const std::string & audio );
			
			void setSheet( const std::string & sheet );
			
			void setIndex( const std::vector< int > & index );
			
			void setInput( const InputSP & input );
			
			void setOutput( const OutputSP & output );
			
		signals:
			void error( const QString & );
		
		protected:
			virtual void run();
		
		private:
			InputSP input_;
			OutputSP output_;
			std::string audio_;
			std::string sheet_;
			std::vector< int > index_;
	};
	
}

#endif
