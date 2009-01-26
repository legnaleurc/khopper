/**
 * @file decoder.hpp
 */
#ifndef KHOPPER_MUXER_DECODER_HPP
#define KHOPPER_MUXER_DECODER_HPP

#include "tr1.hpp"

#include <loki/Factory.h>
#include <loki/Singleton.h>

#include <QWidget>

#include <string>

struct AVFormatContext;
struct AVCodecContext;
struct AVPacket;

namespace Khopper {

	/**
	 * @brief Decoder interface
	 */
	class Decoder {
	public:
		Decoder();

		/**
		 * @brief Pure virtual destructor
		 */
		virtual ~Decoder();

		void open( const std::wstring & filePath );
		bool is_open() const {
			return opening_;
		}
		void close();

		bool seek( double timestamp );

		ByteArray next();
		bool hasNext( double timestamp = 0.0 ) const;

		double getDuration() const;

		/**
		 * @brief Type specified UI
		 * @return A GUI object
		 */
		virtual QWidget * getUI() const;

	private:
		bool opening_;
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

	/**
	 * @namespace Khopper::dec
	 * @brief Namespace of decoder
	 *
	 * Avoid conflict with encoder.
	 */
	
}

#endif
