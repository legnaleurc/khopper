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
		
		protected:
			virtual void run();
		
		private:
			InputSP _input_;
			OutputSP _output_;
			std::string _audio_;
			std::string _sheet_;
			std::vector< int > _index_;
	};
	
}

#endif
