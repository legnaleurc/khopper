/**
 * @file cuesheet.hpp
 */
#ifndef CUESHEET_HPP
#define CUESHEET_HPP

#include "track.hpp"

#include <string>
#include <vector>
#include <map>

namespace Khopper {

	class AudioFile;
	
	/**
	 * @brief The CUE parser
	 */
	class CUESheet {
		public:
			/**
			 * @brief Headers of fields
			 */
			static const std::vector< std::string > headers;

			/**
			 * @brief Default constructor
			 */
			CUESheet();
			
			/**
			 * @brief Give a CUE Sheet path name and create this object
			 * @param [in] fileName path of CUE sheet
			 */
			CUESheet( const std::vector< std::string > & content );

			/**
			 * @brief Set CUE sheet by given string vector
			 * @param [in] content contents of CUE sheet
			 */
			void open( const std::vector< std::string > & content );

			/**
			 * @brief Get catalog infomation
			 * @return catalog
			 */
			const std::string & getCatalog() const;
			
			/**
			 * @brief Get CD text infomation
			 * @return CD text
			 */
			const std::string & getCDTextFile() const;
			
			/**
			 * @brief Get comments
			 * @return comments
			 */
			const std::map< std::string, std::string > & getComments() const;
			
			/**
			 * @brief Get garbage infomation
			 * @return garbage
			 */
			const std::vector< std::string > & getGarbage() const;
			
			/**
			 * @brief Get performer infomation
			 * @return performer
			 */
			const std::string & getPerformer() const;

			/**
			 * @brief Get song writer infomation
			 * @return song writer
			 */
			const std::string & getSongWriter() const;
			
			/**
			 * @brief Get disc title infomation
			 * @return disc title
			 */
			const std::string & getTitle() const;

			/**
			 * @brief Get track infomation
			 * @param [in] index track index
			 * @return track information
			 */
			const Track & operator []( std::size_t index ) const;
			
			/**
			 * @brief Get track amount of disc
			 * @return track amount
			 */
			std::size_t size() const;
		private:
			void parseCUE_( const std::vector< std::string > & );
			void parseSingle_( const std::string &, const std::string &, int );
			AudioFile parseFile_( const std::string &, const std::string & );
			void parseFlags_( const std::string &, int );
			void parseIndex_( const std::string &, const std::string &, const std::string &, const std::string &, const std::string &, int );
			void parseComment_( const std::string &, const std::string &, int );
			void parseTrack_( const std::string &, const std::string & );
			void parseGarbage_( const std::string &, int );
			
			std::string catalog_;
			std::string cdTextFile_;
			std::map< std::string, std::string > comments_;
			std::string dirName_;
			std::vector< std::string > garbage_;
			std::string performer_;
			std::string songWriter_;
			std::string title_;
			std::vector< Track > tracks_;
	};

}

#endif
