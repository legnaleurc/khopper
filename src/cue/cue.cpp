#include "cue.hpp"
#include <libgen.h>
#include <cstring>
#include<iostream>

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
		/* ex: /temp/wer/audio.ape */
		using namespace std;

		FILE *fd;
		FILE *ad;
		
		char temp[BUFSIZE];//catch file line
		
		int disc = 0;//equal to 1 mean in the disc , equal to 0 not in the disc
		int track = 0;//means the track number
		int d_tag = 0;//disc tag
		int t_tag = 0;//track tag
		string str3;//mean \t
		size_t pos;//mean \t number
		size_t pon;//mean \n number
		
		string command = string( "cueprint " ) + filename;
		
		_sheetName_ = std::pair< std::string, std::string >( dirname( strcpy( temp, filename.c_str() ) ), basename( strcpy( temp, filename.c_str() ) ) );
		
		string temp_1;
		if( (ad = fopen(filename.c_str(),"r")) == NULL )
		{		fputs("No file\n",stderr);}
		else
		{
			while(fgets(temp,BUFSIZE,ad)!=NULL)
			{
				if( strstr(temp,"FILE") != NULL )
				{
					temp_1 = temp;
					pos = temp_1.find("\"");
					temp_1 = temp_1.substr(pos+1);
					pon = temp_1.find("\"");
					string no_use = temp_1.substr(pon);
					temp_1.replace(pon,no_use.length(),"");
					break;
				}
			}
		}
		fclose(ad);
		
		
		_audioName_ = std::pair< std::string, std::string >( _sheetName_.first ,temp_1 );
	
		if( (fd = popen(command.c_str(),"r")) == NULL )
			fputs("No command\n", stderr);
		
		
		
		_discInfo_.resize( Disc::SIZE );
// 		_trackInfo_ = vector< FieldType >( 0, FieldType( Disc::SIZE ) );
		
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
					_discInfo_[d_tag] = str3.substr(pos+1);
					d_tag++; 
				}
				else
				{
					//_trackInfo_[track][t_tag] = str3.substr(pos+1);
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
					_discInfo_[d_tag] = str3.substr(pos+1);
					d_tag++; 
				}
				else
				{
					//_trackInfo_[track][t_tag] = str3.substr(pos+1);
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
					_discInfo_[d_tag] = str3.substr(pos+2);
					d_tag++; 
				}
				else
				{
					//_trackInfo_[track][t_tag] = str3.substr(pos+2);
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
					_discInfo_[d_tag] = str3.substr(pos+1);
					d_tag++; 
				}
				else
				{
					//_trackInfo_[track][t_tag] = str3.substr(pos+1);
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
					_discInfo_[d_tag] = str3.substr(pos+1);
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
					_discInfo_[d_tag] = str3.substr(pos+1);
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
				if(disc == 1)
				{
					//_trackInfo_[track][t_tag] = str3.substr(pos+1);
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
					_discInfo_[d_tag] = str3.substr(pos+1);
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
					_discInfo_[d_tag] = str3.substr(pos+2);
					d_tag++; 
				}
				else
				{
					//_trackInfo_[track][t_tag] = str3.substr(pos+2);
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
					_discInfo_[d_tag] = str3.substr(pos+1);
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
					//_trackInfo_[track][t_tag] = str3.substr(pos+1);
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
					//_trackInfo_[track][t_tag] = str3.substr(pos+1);
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
					//_trackInfo_[track][t_tag] = str3.substr(pos+1);
					temp_vector[t_tag] = str3.substr(pos+1);
					_trackInfo_.push_back(temp_vector);
					t_tag++;
				}
			}
		}
		
		pclose(fd);
		
		
	}
	
	std::pair< std::string, std::string > CUESheet::getSheetName() const {
		return _sheetName_;
	}
	
	std::pair< std::string, std::string > CUESheet::getAudioName() const {
		return _audioName_;
	}
	
	CUESheet::FieldType CUESheet::getDiscInfo() const {
		return _discInfo_;
	}
	
	std::vector< CUESheet::FieldType > CUESheet::getTrackInfo() const {
		return _trackInfo_;
	}

}
