/**
 * @file decoder.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KHOPPER_DECODER_HPP
#define KHOPPER_DECODER_HPP

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

		int getDecodeTimes();

		/**
		 * @brief Get sample duration
		 * @return Duration
		 */
		double getDuration() const {
			return this->duration_;
		}
		const std::wstring & getTitle() const {
			return this->title_;
		}
		const std::wstring & getAuthor() const {
			return this->author_;
		}
		const std::wstring & getCopyright() const {
			return this->copyright_;
		}
		const std::wstring & getComment() const {
			return this->comment_;
		}
		const std::wstring & getAlbum() const {
			return this->album_;
		}
		int getYear() const {
			return this->year_;
		}
		int getIndex() const {
			return this->index_;
		}
		const std::wstring & getGenre() const {
			return this->genre_;
		}
		int getBitRate() const {
			return this->bitRate_;
		}
		int getSampleRate() const {
			return this->sampleRate_;
		}
		int getChannels() const {
			return this->channels_;
		}

	private:
		bool afterBegin_() const;
		bool afterEnd_() const;

		bool opening_;
		int64_t begin_;
		int64_t end_;
		int64_t nextPts_;
		bool hasNext_;
		std::tr1::shared_ptr< AVFormatContext > pFormatContext_;
		std::tr1::shared_ptr< AVCodecContext > pCodecContext_;
		std::tr1::shared_ptr< AVPacket > pPacket_;
		double timeBase_;

		double duration_;
		std::wstring title_;
		std::wstring author_;
		std::wstring copyright_;
		std::wstring comment_;
		std::wstring album_;
		int year_;
		int index_;
		std::wstring genre_;
		int bitRate_;
		int sampleRate_;
		int channels_;
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
