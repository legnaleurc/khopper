#include "util/os.hpp"

#include <sys/wait.h>
#include <cerrno>
#include <cstring>
#include <climits>
#include <boost/format.hpp>

namespace {

	class Args_{
		public:
			Args_( const std::vector< std::string > & args ) {
				argt_ = new char * [ args.size() + 1 ];
				for( std::size_t i = 0; i < args.size(); ++i ) {
					argt_[i] = new char[ args[i].length() + 1 ];
					strcpy( argt_[i], args[i].c_str() );
				}
				argt_[args.size()] = NULL;
			}

			char * * getArgs() {
				return argt_;
			}

			~Args_() {
				for( int i = 0; argt_[i] != NULL; ++i ) {
					delete [] argt_[i];
				}
				delete [] argt_;
			}
			
		private:
			char * * argt_;
			Args_( const Args_ & );
			Args_ & operator =( const Args_ & );
	};
	
}

namespace Khopper {
	
	int os::getResult( std::pair< std::string, std::string > & msg, const std::vector< std::string > & args ) {
		pid_t pid;
		int ret;
		int out[2];
		int err[2];
		
		// create pipes
		if( pipe( out ) < 0 ) {
			throw Error< OS >( strerror( errno ) );
		}
		if( pipe( err ) < 0 ) {
			throw Error< OS >( strerror( errno ) );
		}
		
		Args_ argt( args );
		
		// fork process
		if( ( pid = fork() ) < 0 ) {	// error
			throw Error< OS >( strerror( errno ) );
		} else if( pid > 0 ) {	// parent process
			int status;
			
			close( out[1] );
			close( err[1] );
			
			waitpid( pid, &status, 0 );
			
			char buffer[PIPE_BUF];
			
			msg.first.clear();
			while( read( out[0], buffer, PIPE_BUF ) > 0 ) {
				msg.first += buffer;
			}
			msg.second.clear();
			while( read( err[0], buffer, PIPE_BUF ) > 0 ) {
				msg.second += buffer;
			}
			
			if( WIFEXITED( status ) ) {
				ret = WEXITSTATUS( status );
			} else {
				throw Error< OS >( "Child process has unknown return status." );
			}
		} else {	// child process
			close( out[0] );
			close( err[0] );
			
			dup2( out[1], 1 );
			dup2( err[1], 2 );
			
			ret = execvp( argt.getArgs()[0], argt.getArgs() );
			
			if( ret < 0 ) {
				throw Error< OS >( strerror( errno ) );
			}
		}
		
		return ret;
	}
	
	void os::exists( const std::string & exe ) {
		if( system( ( boost::format( "[ `which %1%` ]" ) % exe ).str().c_str() ) != 0 ) {
			throw Error< RunTime >( ( boost::format( "`%1%\' not found!" ) % exe ).str() );
		}
	}
	
}
