#include <common.h>

#include "basic_log.h"

// todo: move some of those to .h as inline

n_sdk::c_basic_log& n_sdk::c_basic_log::bind_state( n_sdk::state_t* state )
{
	this->m_state = state; return *this;
}
n_sdk::c_basic_log& n_sdk::c_basic_log::set_state( n_sdk::state_t state )
{
	*this->m_state = state; return *this;
}
n_sdk::c_basic_log& n_sdk::c_basic_log::set_cheat( const std::string cheat )
{
	this->m_cheat = cheat; return *this;
}
n_sdk::c_basic_log& n_sdk::c_basic_log::set_owner( const std::string owner )
{
	this->m_owner = owner; return *this;
}
n_sdk::c_basic_log& n_sdk::c_basic_log::set_directory( const std::string directory )
{
	this->m_directory = directory; return *this;
}

std::string n_sdk::c_basic_log::resolve_cheat_wrappers( )
{
	return "\x1b[38;2;" + this->cheat_wrappers + "m[\x1b[0m " + this->m_cheat + " \x1b[38;2;" + this->cheat_wrappers + "m]\x1b[0m: ";
}

std::string n_sdk::c_basic_log::resolve_at_mark( )
{
	return "\x1b[38;2;" + this->at_mark + "m@\x1b[0m";
}

std::string n_sdk::c_basic_log::resolve_status( )
{
	// "\x1b[38;2;" + this->at_mark + "m@\x1b[0m";
	std::string status_color = "";
	switch ( *( this->m_state ) )
	{
		case n_enum::started: status_color = this->status_started; break;
		case n_enum::waiting: status_color = this->status_waiting; break;
		case n_enum::failed:  status_color = this->status_failed; break;
		case n_enum::success: status_color = this->status_success; break;

		default: status_color = this->status_started; break;
	}
	return "\x1b[38;2;" + status_color + "m" + status_str( ) + "\x1b[0m";
}

std::string n_sdk::c_basic_log::resolve_directory_slashes( const std::string& base )
{
	std::string resolved = base; // 201;148;85
	std::string colored = "\x1b[38;2;" + this->directory_slash + "m/\x1b[0m";

	for ( int pos = 0;
		( pos = resolved.find( '/', pos ) ) != std::string::npos;
		pos += colored.length( ) )
	{
		// dont go over '>'
		if ( resolved.find( '>' ) < pos ) break;
		
		resolved.replace( pos, 1, colored );
	}

	return resolved;
}

std::string n_sdk::c_basic_log::resolve_colors( const std::string& base )
{
	// for now we dont support base color resolving
	// todo: ^
	
	return resolve_cheat_wrappers( ) + this->m_owner + resolve_at_mark( )
		+ resolve_status( ) + resolve_directory_slashes( this->m_directory );
}

std::string n_sdk::c_basic_log::resolve_args( const char* format, va_list args )
{
	va_list args_cpy;
	va_copy( args_cpy, args );
	
	// prepare buffer with exact size
	std::vector<char> formatted( std::vsnprintf( nullptr, 0, 
		format, args ) + 1 /* \0 */ );

	// put formatted text into buffer
	std::vsnprintf( formatted.data( ), formatted.size( ), format, args_cpy );
	va_end( args_cpy );
	
	return std::string( formatted.data( ) );
}

std::string n_sdk::c_basic_log::resolver( const char* format, va_list args )
{
	std::string format_parsed
		= resolve_args( format, args );
	std::string colored_base
		= resolve_colors( "" );

	return colored_base + "> " + format_parsed;
}

std::string n_sdk::c_basic_log::status_str( )
{
	switch ( *( this->m_state ) )
	{
		case n_enum::started: return "started";
		case n_enum::waiting: return "waiting";
		case n_enum::failed:  return "failed";
		case n_enum::success: return "success";

		default: return "undeclared";
	}
}

n_sdk::c_basic_log& n_sdk::c_basic_log::log( const char* format, ... )
{
	va_list args;
	va_start( args, format );

	std::string resolved = resolver( format, args );
	va_end( args );
	
	std::cout << resolved << std::endl; 
	
	return *this;
}

bool n_sdk::c_basic_log::return_true( )
{
	return 1;
}
bool n_sdk::c_basic_log::return_false( )
{
	return 0;
}
