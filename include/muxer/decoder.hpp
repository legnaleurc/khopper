/**
 * @file decoder.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KHOPPER_MUXER_DECODER_HPP
#define KHOPPER_MUXER_DECODER_HPP

#include "tr1.hpp"

#include <loki/Factory.h>
#include <loki/Singleton.h>

#include <string>

struct AVFormatContext;
struct AVCodecContext;
struct AVPacket;

namespace Khopper {

	/**
	 * @brief Decoder interface
	 *
	 * This module uses std::wstring to store unicode string.\n
	 * UTF-16 on Windows(R), UCS4 on POSIX.
	 */
	class Decoder {
	public:
		/**
		 * @brief Default constructor
		 */
		Decoder();

		/**
		 * @brief Pure virtual destructor
		 */
		virtual ~Decoder();

		/**
		 * @brief Open file
		 * @param filePath File path
		 */
		void open( const std::wstring & filePath );
		/**
		 * @brief Check if file is opening
		 * @retval true opening
		 * @retval false closed
		 */
		bool is_open() const {
			return opening_;
		}
		/**
		 * @brief Close decoder
		 */
		void close();

		/**
		 * @brief Seek time
		 * @param timestamp Seeks time stamp, in seconds
		 * @retval ture Succeed
		 * @retval false Failed
		 */
		bool seek( double timestamp );

		/**
		 * @brief Set decode range
		 * @param begin Begin time stamp, in seconds
		 * @param end End time stamp, in seconds
		 *
		 * If @p begin is -1, means begin of file, \n
		 * if @p end is -1, means end of file.
		 */
		void setRange( double begin, double end );
		/**
		 * @brief Read one frame
		 * @return Frame raw binary data
		 */
		ByteArray read();
		/**
		 * @brief Check if there is any readable data
		 * @retval true Has data to read
		 * @retval false No data
		 */
		bool hasNext() const;

		/**
		 * @brief Get sample duration
		 * @return Duration
		 */
		double getDuration() const;

	private:
		bool afterBegin_() const;
		bool afterEnd_() const;
		bool opening_;
		int64_t begin_;
		int64_t end_;
		int64_t nextPts_;
		bool hasNext_;
		double duration_;
		std::tr1::shared_ptr< AVFormatContext > pFormatContext_;
		std::tr1::shared_ptr< AVCodecContext > pCodecContext_;
		std::tr1::shared_ptr< AVPacket > pPacket_;
		double timeBase_;
	};

	/**
	 * @brief Decoder smart pointer
	 *
	 * Use TR1 shared pointer.
	 */
	typedef std::tr1::shared_ptr< Decoder > DecoderSP;

	/**
	 * @brief The decoder Object Factory for Converter module
	 */
	typedef Loki::SingletonHolder< Loki::Factory< Decoder, std::string >, Loki::CreateUsingNew, Loki::LongevityLifetime::DieAsSmallObjectChild > DecoderFactory;

}

#endif
