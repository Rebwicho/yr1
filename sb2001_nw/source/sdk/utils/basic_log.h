#ifndef BASIC_LOG_H
#define BASIC_LOG_H

#include "enum_file.h"

namespace n_sdk
{
	class c_basic_log
	{
	private:
		/* colors */
		std::string cheat_wrappers = "46;46;54";
		std::string at_mark = "30;53;130";
		std::string status_started = "89;89;89";
		std::string status_waiting = "160;166;51";
		std::string status_failed = "140;10;10";
		std::string status_success = "3;163;0";
		std::string directory_slash = "201;148;85";
		
	public:
		c_basic_log& bind_state( n_sdk::state_t* state );
		c_basic_log& set_state( n_sdk::state_t state );
		c_basic_log& set_cheat( const std::string cheat );
		c_basic_log& set_owner( const std::string owner );
		c_basic_log& set_directory( const std::string directory );

	private:
		std::string resolve_cheat_wrappers( );
		std::string resolve_at_mark( );
		std::string resolve_status( );
		std::string resolve_directory_slashes( const std::string& );

	public:
		std::string resolve_colors( const std::string& base );
		std::string resolve_args( const char* format, va_list args );
		std::string resolver( const char* format, va_list args );

		std::string status_str( );
		
		// todo: args ...
		c_basic_log& log( const char* format, ... );

		bool return_true( );
		bool return_false( );

	private:
		std::string m_cheat = "sb2001_nw"; // todo: make preprocessor.h file and #define CHEAT_NAME "sb2001_nw"
		std::string m_owner = "null";
		std::string m_directory = "/.";
		//std::
	
	private:
		n_sdk::state_t* m_state = nullptr;
	};
}

#endif // BASIC_LOG_H
