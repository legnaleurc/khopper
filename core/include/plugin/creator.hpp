/**
 * @file creator.hpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * Khopper is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Khopper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KHOPPER_CREATOR_HPP
#define KHOPPER_CREATOR_HPP

/**
 * @defgroup Plugins Plugin System
 *
 * 這是 Plugin System 模粗，基於 <a href="http://doc.trolltech.com/4.5/plugins-howto.html">Qt Plugin System</a>。
 * 本模組允許動態擴充轉檔格式，以及編碼、解碼器。
 *
 * <h2>如何擴充轉檔格式</h2>
 *
 * <h3>輪出格式</h3>
 * 繼承 khopper::plugin::AbstractPanel 並覆寫所有 pure virtual functions，依照 Qt Plugin System 所寫的指示即可。
 *
 * <h3>解碼器</h3>
 * 繼承 khopper::plugin::ReaderCreator，並覆寫 khopper::plugin::ReaderCreator::create_，
 * 使之回傳你想要的解碼器。 \n
 * 並在全域呼叫 khopper::plugin::registerReader，以註冊 plugin。下例：
 * <pre>
 * const bool REGISTERED = khopper::plugin::registerReader( "mp3", "krp_mp3" );
 * </pre>
 * 其中，"mp3"為程式內使用之識別字，"krp_mp3"為 plugin 所使用的名稱。
 *
 * <h3>編碼器</h3>
 * 繼承 khopper::plugin::WriterCreator，並覆寫 khopper::plugin::WriterCreator::create_，
 * 使之回傳你想要的解碼器。 \n
 * 並在全域呼叫 khopper::plugin::registerWriter，以註冊 plugin。下例：
 * <pre>
 * const bool REGISTERED = khopper::plugin::registerWriter( "mp3", "krp_mp3" );
 * </pre>
 * 其中，"mp3"為程式內使用之識別字，"krp_mp3"為 plugin 所使用的名稱。
 */
// @{
// @}

#include "common/tr1.hpp"

namespace khopper {

	/**
	 * @ingroup Plugins
	 * @brief Contains plugin utility
	 */
	namespace plugin {

		/**
		 * @ingroup Plugins
		 * @brief Common creator factory
		 * @tparam Product The product type of return
		 * @sa ReaderCreator WriterCreator
		 *
		 * You will not use this class directly.
		 */
		template< typename Product >
		class KHOPPER_DLL_EXPORT Creator {
		public:
			/// Default constructor
			Creator() {}
			/// Create the product
			std::tr1::shared_ptr< Product > create() const {
				return this->create_();
			}
		private:
			Creator( const Creator & );
			Creator & operator =( const Creator & );
			virtual std::tr1::shared_ptr< Product > create_() const = 0;
		};

	}

}

#endif
