/**
 * @file Threads.hpp
 */
#ifndef THREADS_HPP
#define THREADS_HPP

#include "converter.hpp"

#include <QThread>
#include <QProgressDialog>

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
			ConverterThread( const Converter & conv, const std::string & audio, const std::string & sheet, const std::vector< int > & index, QObject * parent = 0 );
		
		protected:
			virtual void run();
		
		private:
			const Converter & _conv_;
			const std::string & _audio_;
			const std::string & _sheet_;
			const std::vector< int > & _index_;
			// protection
			ConverterThread();
	};
	
}

#endif
