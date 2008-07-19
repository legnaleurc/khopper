#include "cue.hpp"
#include <libgen.h>
#include <cstring>

namespace {
	const size_t BUFSIZE = 4096;
}

namespace Khopper {

	const char * const CUESheet::Track::Header[] = {
		"Arranger",
		"Composer",
		"Genre",
		"ISRC",
		"Message",
		"Number",
		"Perfomer",
		"Title",
		"CD_Text",
		NULL
	};

	CUESheet::CUESheet( const std::string & filename ) {
		using std::string;
		
		FILE *fd;
		
		char temp[BUFSIZE];//catch file line
		
		int disc = 0;//equal to 1 mean in the disc , equal to 0 not in the disc
		int track = 0;//means the track number
		int d_tag = 0;//disc tag
		int t_tag = 0;//track tag
		string str3;//mean \t
		size_t pos;//mean \t number
		size_t pon;//mean \n number
		
		std::string command = ( boost::format( "cueprint \"%1%\"" ) % filename ).str();
		
		boost::regex pattern( "(.*)/(.*)" );
		boost::smatch result;
		
		if( !boost::regex_match( filename, result, pattern ) ) {
			throw Error< Parsing >( ( boost::format( "`%1%\' is not a valid CUE Sheet path." ) % filename ).str() );
		}
		
		sheetName_ = std::pair< std::string, std::string >( result[1], result[2] );
		
		string temp_1;
		std::ifstream fin( filename.c_str() );
		if( !fin.is_open() ) {
			throw Error< System >( ( boost::format( "Can not open %1% !" ) % filename ).str() );
		}
		pattern = ".*FILE(\\s+)\"(.+)\".*";
		while( getline( fin, temp_1 ) ) {
			if( boost::regex_match( temp_1, result, pattern ) ) {
				audioName_ = std::pair< std::string, std::string >( sheetName_.first ,result[2] );
				break;
			}
		}
		fin.close();
		if( audioName_.second.length() == 0 ) {
			throw Error< System >( "Can not found media path." );
		}
		
		if( (fd = popen(command.c_str(),"r")) == NULL )
			fputs("No command\n", stderr);
		
		discInfo_.resize( Disc::SIZE );
		
		FieldType temp_vector( Disc::SIZE );
		while(fgets(temp,BUFSIZE,fd)!=NULL)
		{
			
			if( strstr(temp,"Disc Information") != NULL )
				disc = 1;
			if( strstr(temp,"Track") != NULL )
			{
				disc = 0;
				sscanf(temp,"Track %d Information",&track);
				track = track-1;
				t_tag = 0;
			}
			if( strstr(temp,"arranger:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pon);
				str3.replace(pon,no_use.length(),"");
				
				if(disc == 1)
				{
					discInfo_[d_tag] = str3.substr(pos+1);
					d_tag++; 
				}
				else
				{
					temp_vector[t_tag] = str3.substr(pos+1);
					t_tag++;
				}
			}
			if( strstr(temp,"composer:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pon);
				str3.replace(pon,no_use.length(),"");
				if(disc == 1)
				{
					discInfo_[d_tag] = str3.substr(pos+1);
					d_tag++; 
				}
				else
				{
					temp_vector[t_tag] = str3.substr(pos+1);
					t_tag++;
				}
			}
			if( strstr(temp,"genre:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pon);
				str3.replace(pon,no_use.length(),"");
				if(disc == 1)
				{
					discInfo_[d_tag] = str3.substr(pos+2);
					d_tag++; 
				}
				else
				{
					temp_vector[t_tag] = str3.substr(pos+2);
					t_tag++;
				}
			}
			if( strstr(temp,"message:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pon);
				str3.replace(pon,no_use.length(),"");
				if(disc == 1)
				{
					discInfo_[d_tag] = str3.substr(pos+1);
					d_tag++; 
				}
				else
				{
					temp_vector[t_tag] = str3.substr(pos+1);
					t_tag++;
				}
			}
			if( strstr(temp,"no. of tracks:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pon);
				str3.replace(pon,no_use.length(),"");
				if(disc == 1)
				{
					discInfo_[d_tag] = str3.substr(pos+1);
					d_tag++; 
				}
			}
			if( strstr(temp,"performer:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pon);
				str3.replace(pon,no_use.length(),"");
				if(disc == 1)
				{
					discInfo_[d_tag] = str3.substr(pos+1);
					d_tag++; 
				}
			}
			if( strstr(temp,"perfomer:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pon);
				str3.replace(pon,no_use.length(),"");
				if(disc != 1)
				{
					temp_vector[t_tag] = str3.substr(pos+1);
					t_tag++; 
				}
			}
			if( strstr(temp,"songwriter:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pon);
				str3.replace(pon,no_use.length(),"");
				if(disc == 1)
				{
					discInfo_[d_tag] = str3.substr(pos+1);
					d_tag++; 
				}
			}
			if( strstr(temp,"title:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pon);
				str3.replace(pon,no_use.length(),"");
				if(disc == 1)
				{
					discInfo_[d_tag] = str3.substr(pos+2);
					d_tag++; 
				}
				else
				{	
					//cout<<str3.substr(pos+2)<<endl;
					temp_vector[t_tag] = str3.substr(pos+2);
					t_tag++;
				}
			}
			if( strstr(temp,"UPC/EAN:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pon);
				str3.replace(pon,no_use.length(),"");
				if(disc == 1)
				{
					discInfo_[d_tag] = str3.substr(pos+1);
					d_tag++; 
				}
			}
			if( strstr(temp,"ISRC:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pon);
				str3.replace(pon,no_use.length(),"");
				if(disc != 1)
				{
					temp_vector[t_tag] = str3.substr(pos+1);
					t_tag++;
				}
			}
			if( strstr(temp,"track number:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pon);
				str3.replace(pon,no_use.length(),"");
				if(disc != 1)
				{
					temp_vector[t_tag] = str3.substr(pos+1);
					t_tag++;
				}
			}
			if( strstr(temp,"ISRC (CD-TEXT):") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pon);
				str3.replace(pon,no_use.length(),"");
				if(disc != 1)
				{
					temp_vector[t_tag] = str3.substr(pos+1);
					trackInfo_.push_back(temp_vector);
					t_tag++;
				}
			}
		}
		
		pclose(fd);
		
	}
	
	const std::pair< std::string, std::string > & CUESheet::getSheetName() const {
		return sheetName_;
	}
	
	const std::pair< std::string, std::string > & CUESheet::getAudioName() const {
		return audioName_;
	}
	
	const CUESheet::FieldType & CUESheet::getDiscInfo() const {
		return discInfo_;
	}
	
	const std::vector< CUESheet::FieldType > & CUESheet::getTrackInfo() const {
		return trackInfo_;
	}

}
