#include "os.hpp"

#include <sys/wait.h>
#include <cstring>
#include <climits>
#include <boost/format.hpp>

namespace {
	
	void freeArgs( char * * args ) {
		for( int i = 0; args[i] != NULL; ++i ) {
			delete [] args[i];
		}
		delete [] args;
	}
	
}

namespace Khopper {
	
	int os::getResult( std::pair< std::string, std::string > & msg, const std::vector< std::string > & args ) {
		char * * argt = new char*[ args.size() + 1 ];
		
		for( std::size_t i = 0; i < args.size(); ++i ) {
			argt[i] = new char[ args[i].length() + 1 ];
			strcpy( argt[i], args[i].c_str() );
		}
		argt[args.size()] = NULL;
		
		pid_t pid;
		int ret;
		int out[2];
		int err[2];
		
		// create pipes
		if( pipe( out ) < 0 ) {
			freeArgs( argt );
			throw Error< OS >( strerror( errno ) );
		}
		if( pipe( err ) < 0 ) {
			freeArgs( argt );
			throw Error< OS >( strerror( errno ) );
		}
		
		// fork process
		if( ( pid = fork() ) < 0 ) {	// error
			freeArgs( argt );
			throw Error< OS >( strerror( errno ) );
		} else if( pid > 0 ) {	// parent process
			int status;
			
			freeArgs( argt );
			
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
			
			ret = execvp( argt[0], argt );
			freeArgs( argt );
			
			if( ret < 0 ) {
				throw Error< OS >( strerror( errno ) );
			}
		}
		
		return ret;
	}
	
	void os::exists( const std::string & exe ) {
		if( system( ( boost::format( "[ `which %1% ]" ) % exe ).str().c_str() ) != 0 ) {
			throw Error< RunTime >( ( boost::format( "`%1%\' not found!" ) % exe ).str() );
		}
	}
	
}
