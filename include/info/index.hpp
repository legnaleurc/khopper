/**
 * @file index.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KHOPPER_INFO_INDEX_HPP
#define KHOPPER_INFO_INDEX_HPP

#include <string>

namespace Khopper {

	/**
	 * @brief Track index information
	 */
	class Index {
		public:
			/**
			 * @brief Default constructor
			 *
			 * Initialize to 0:0.0.
			 */
			Index();

			/**
			 * @brief Custom constructor
			 * @param [in] m minute
			 * @param [in] s second
			 * @param [in] f frame
			 *
			 * Initialize to m:s.f.
			 */
			Index( short int m, short int s, short int f );

			/**
			 * @brief Convert from double
			 * @param [in] seconds
			 */
			Index( double d );

			/// minite
			unsigned short int minute;
			/// second
			unsigned short int second;
			/// frame
			unsigned short int frame;

			const Index & operator -=( const Index & that );

			Index operator -( const Index & that ) const;

			/**
			 * @brief Convert to double
			 * @return seconds in double
			 */
			double toDouble() const;

			/**
			 * @brief Convert to std::wstring
			 * @return mm:ss.ff
			 */
			std::wstring toStdWString() const;
	};

}

#endif
