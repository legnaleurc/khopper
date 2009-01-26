/**
 * @file encoder.hpp
 */
#ifndef KHOPPER_MUXER_ENCODER_HPP
#define KHOPPER_MUXER_ENCODER_HPP

#include "tr1.hpp"

#include <loki/Factory.h>
#include <loki/Singleton.h>
#include <loki/AssocVector.h>

#include <QWidget>

#include <string>

struct AVFormatContext;
struct AVOutputFormat;
struct AVStream;

namespace Khopper {

	/**
	 * @brief Encoder interface
	 */
	class Encoder {
	public:
		Encoder();
		/**
		 * @brief Pure virtual destructor
		 */
		virtual ~Encoder();

		void open( const std::wstring & filePath );
		bool is_open() const;
		void close();

		void setBitRate( int bit_rate );
		void setSampleRate( int sample_rate );
		void setChannels( int channels );

		void write( const ByteArray & data );
		void flush();

		/**
		 * @brief Get Type GUI
		 * @return GUI object
		 */
		virtual QWidget * getUI() const;

	protected:
		virtual AVOutputFormat * guessFormat() const;

	private:
		bool openAudio_();
		void writeFrame_( short * );

		bool opening_;
		std::string filePath_;
		ByteArray buffer_;
		ByteArray samples_;
		std::tr1::shared_ptr< AVFormatContext > pFormatContext_;
		AVStream * pStream_;
		int bitRate_;
		int sampleRate_;
		int channels_;
	};

	/**
	 * @brief Encoder smart pointer
	 *
	 * Use TR1 shared pointer.
	 */
	typedef std::tr1::shared_ptr< Encoder > EncoderSP;

	/**
	 * @brief The encoder Object Factory for Converter module
	 */
	typedef Loki::SingletonHolder< Loki::Factory< Encoder, std::string >, Loki::CreateUsingNew, Loki::LongevityLifetime::DieAsSmallObjectChild > EncoderFactory;

	/**
	 * @brief Encoder type list
	 */
	typedef Loki::SingletonHolder< Loki::AssocVector< std::string, std::string > > EncoderList;
	
	/**
	 * @namespace Khopper::enc
	 * @brief Namespace of encoder
	 *
	 * Avoid conflict with decoder.
	 */
	
}

#endif
