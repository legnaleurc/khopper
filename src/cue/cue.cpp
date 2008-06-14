#include "cue.hpp"
#include<libgen.h> 
#include <string>  

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
		
		string d_name;
		string b_name;

		FILE *fd;
		FILE *ad;
		
		char temp[200];//catch file line
		
		int disc = 0;//equal to 1 mean in the disc , equal to 0 not in the disc
		int track = 0;//means the track number
		int d_tag = 0;//disc tag
		int t_tag = 0;//track tag
		string str3;//mean \t
		size_t pos;//mean \t number
		size_t pon;//mean \n number
		
		string cue("cueprint ");
		string filepath_name(filename);
		string command = cue+filepath_name;
		
		
		/*d_name( dirname( filename.c_str() ) );
		b_name( basename( filename.c_str() ) );*/
		
		_sheetName_ = std::pair< std::string, std::string >(d_name( dirname( filename.c_str() ) ),b_name( basename( filename.c_str() ) ));
		
		string temp_1;
		if( (ad = fopen(filename,"r")) == NULL )
		{		fputs("No file\n",stderr);}
		while(fgets(temp,200,ad)!=NULL)
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
		fclose(ad);
		
		
		_audioName_ = std::pair< std::string, std::string >(d_name( dirname( filename.c_str() ) ),b_name( basename( temp_1.c_str() ) ));
	
		if( (fd = popen(command.c_str(),"r")) == NULL )
			fputs("No command\n",20,stderr);
		
		
		
		_discInfo_ = FieldType( Disc::Field::SIZE );
		_TrackInfo_ = FieldType( Disc::Field::SIZE);
		
		while(fgets(temp,200,fd)!=NULL)
		{
			if( strstr(temp,"Disc Information") != NULL )
				disc = 1;
			if( strstr(temp,"Track") != NULL )
			{
				disc = 0;
				sscanf(temp,"Track %d Information",&track);
				track = track-1;
			}
			if( strstr(temp,"arranger:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pos);
				str3.replace(pon,no_use.length(),"");
				
				if(disc == 1)
				{
					_discInfo_[d_tag] = str3.substr(pos+1);
					d_tag++; 
				}
				else
				{
					_TrackInfo_[track][t_tag] = str3.substr(pos+1);
					t_tag++;
				}
			}
			if( strstr(temp,"composer:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pos);
				str3.replace(pon,no_use.length(),"");
				if(disc == 1)
				{
					_discInfo_[d_tag] = str3.substr(pos+1);
					d_tag++; 
				}
				else
				{
					_TrackInfo_[track][t_tag] = str3.substr(pos+1);
					t_tag++;
				}
			}
			if( strstr(temp,"genre:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pos);
				str3.replace(pon,no_use.length(),"");
				if(disc == 1)
				{
					_discInfo_[d_tag] = str3.substr(pos+2);
					d_tag++; 
				}
				else
				{
					_TrackInfo_[track][t_tag] = str3.substr(pos+2);
					t_tag++;
				}
			}
			if( strstr(temp,"message:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pos);
				str3.replace(pon,no_use.length(),"");
				if(disc == 1)
				{
					_discInfo_[d_tag] = str3.substr(pos+1);
					d_tag++; 
				}
				else
				{
					_TrackInfo_[track][t_tag] = str3.substr(pos+1);
					t_tag++;
				}
			}
			if( strstr(temp,"no. of tracks:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pos);
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
				string no_use = str3.substr(pos);
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
				string no_use = str3.substr(pos);
				str3.replace(pon,no_use.length(),"");
				if(disc == 1)
				{
					_TrackInfo_[track][t_tag] = str3.substr(pos+1);
					t_tag++; 
				}
			}
			if( strstr(temp,"songwriter:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pos);
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
				string no_use = str3.substr(pos);
				str3.replace(pon,no_use.length(),"");
				if(disc == 1)
				{
					_discInfo_[d_tag] = str3.substr(pos+2);
					d_tag++; 
				}
				else
				{
					_TrackInfo_[track][t_tag] = str3.substr(pos+2);
					t_tag++;
				}
			}
			if( strstr(temp,"UPC/EAN:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pos);
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
				string no_use = str3.substr(pos);
				str3.replace(pon,no_use.length(),"");
				if(disc != 1)
				{
					_TrackInfo_[track][t_tag] = str3.substr(pos+1);
					t_tag++;
				}
			}
			if( strstr(temp,"track number:") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pos);
				str3.replace(pon,no_use.length(),"");
				if(disc != 1)
				{
					_TrackInfo_[track][t_tag] = str3.substr(pos+1);
					t_tag++;
				}
			}
			if( strstr(temp,"ISRC (CD-TEXT):") != NULL )
			{
				str3 = temp;
				pos = str3.find("\t");
				pon = str3.find("\n");
				string no_use = str3.substr(pos);
				str3.replace(pon,no_use.length(),"");
				if(disc != 1)
				{
					_TrackInfo_[track][t_tag] = str3.substr(pos+1);
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
		return _TrackInfo_;
	}

}
