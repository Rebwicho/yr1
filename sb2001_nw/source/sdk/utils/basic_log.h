#ifndef BASIC_LOG_H
#define BASIC_LOG_H

#include "enum_file.h"

namespace n_sdk
{
	// todo: move to sdk/log
	class c_basic_log
	{
	private:
		/* colors */
		std::string cheat_wrappers = "46;46;54";
		std::string at_mark = "30;53;130";
		std::string status_started = "89;89;89";
		std::string status_waiting = "160;166;51";
		std::string status_working = "163;79;0";
		std::string status_failed = "140;10;10";
		std::string status_success = "5;165;0";
		std::string status_idle = "30;102;189";
		std::string status_warning = "30;102;189";
		std::string status_error = "140;10;10";
		std::string status_exit = "74;7;64";
		std::string directory_slash = "201;148;85";
		std::string directory_ender = "155;25;25";

		/* log utils *WIP* */
		u32 m_error_count = 0;
		u32 m_success_count = 0;
		u32 m_warning_count = 0;
		
	public:
		c_basic_log& bind_state( n_sdk::state_t* state );
		
		c_basic_log& set_state( n_sdk::state_t state );
		c_basic_log& set_cheat( const std::string cheat );
		c_basic_log& set_owner( const std::string owner );
		c_basic_log& set_directory( const std::string directory );

		std::string get_directory( );

		u32 get_error_count( );
		u32 get_success_count( );
		u32 get_warning_count( );

		c_basic_log& push_directory( const std::string directory );
		c_basic_log& pop_directory( );
	
	private:
		std::string resolve_cheat_wrappers( );
		std::string resolve_at_mark( );
		std::string resolve_status( );
		std::string resolve_directory_slashes( );

		std::string resolve_colors( const std::string& base );
		std::string resolve_args( const char* format, va_list args );
		std::string resolver( const char* format, va_list args );

		c_basic_log& update_directory( );
		
		c_basic_log& store_directory_stack( );
		c_basic_log& restore_directory_stack( );

		//c_basic_log& add_error
	
	public:
		std::string status_str( );
		
		bool as_true( );
		bool as_false( );
	
		c_basic_log& on_start( );
		c_basic_log& on_work( );
		c_basic_log& on_failed( );
		c_basic_log& on_success( );
		c_basic_log& on_error( );
		
		c_basic_log& on_exit( );
	
		// note: adding on_success may be bad idea
		// it should be better as on_event:success
		// need to rethink that
		
		c_basic_log& log( const char* format, ... );
	
	private:
		std::string m_cheat = "sb2001_nw"; // todo: make preprocessor.h file and #define CHEAT_NAME "sb2001_nw"
		std::string m_owner = "null";
		std::string m_directory = "";
		std::string m_directory_stored = "";
		std::string m_last_directory = "";

		std::deque< std::string > m_directory_stack = { }; // todo: make separate class with : public std::deque< std::string >
		std::deque< std::string > m_directory_stack_stored = { };
	
	private:
		n_sdk::state_t* m_state = nullptr;		
	};
}

#endif // BASIC_LOG_H
