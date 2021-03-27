#include <common.h>

#include "./time.h"
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
n_sdk::c_basic_log& n_sdk::c_basic_log::set_directory( const std::string directory )    // note: always relative
																						// ( /a/b... ) 
{ 
	this->m_directory_stack.clear( );	
	if ( directory.empty( ) == 1 || directory == "/" )
	{
		update_directory( );
		return *this;
	}
	
	// split by '/' and add to stack to keep track
	for ( u32 pos = 0;
		( pos = directory.find( '/', pos ) ) != std::string::npos;
		pos++ )
	{

		u32 directory_name_size = directory.find( '/', pos + 1 );
		this->m_directory_stack.push_back(
			directory.substr( pos,
				// find end of that directory name
				directory_name_size != std::string::npos ?
				directory_name_size - 1 : std::string::npos
			)
		);
		
	}

	update_directory( ); return *this;
}

std::string n_sdk::c_basic_log::get_directory( )
{
	return this->m_directory;
}

n_sdk::c_basic_log& n_sdk::c_basic_log::push_directory( const std::string directory )
{
	if ( directory.empty( ) == 1 || directory.starts_with( '/' ) == 1 ) return *this;
	this->m_directory_stack.push_back( directory );
	update_directory( );

	return *this;
}
n_sdk::c_basic_log& n_sdk::c_basic_log::pop_directory( )
{
	// check if we are in owner root
	if ( this->m_directory_stack.empty( ) == 1 ) return *this;

	this->m_directory_stack.pop_back( );
	update_directory( );

	return *this;
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
	std::string status_color = "";
	switch ( *( this->m_state ) )
	{
		case n_sdk::state_t::started: status_color = this->status_started; break;
		case n_sdk::state_t::waiting: status_color = this->status_waiting; break;
		case n_sdk::state_t::working: status_color = this->status_working; break;
		case n_sdk::state_t::failed:  status_color = this->status_failed; break;
		case n_sdk::state_t::success: status_color = this->status_success; break;
		case n_sdk::state_t::idle:    status_color = this->status_idle; break;
		case n_sdk::state_t::warning: status_color = this->status_warning; break;
		case n_sdk::state_t::error:   status_color = this->status_error; break;
		case n_sdk::state_t::exit:    status_color = this->status_exit; break;

		default: status_color = this->status_started; break;
	}
	return "\x1b[38;2;" + status_color + "m" + status_str( ) + "\x1b[0m";
}
std::string n_sdk::c_basic_log::resolve_directory_slashes(  )
{	
	std::string resolved = this->m_directory_stack.empty( ) ? "/.>" : this->m_directory + ">";
	std::string colored = "\x1b[38;2;" + this->directory_slash + "m/\x1b[0m";
	std::string colored_ender = "\x1b[38;2;" + this->directory_ender + "m>\x1b[0m ";

	// u32 directory_end = 0;

	for ( u32 pos = 0;
		( pos = resolved.find( '/', pos ) ) != std::string::npos;
		pos += colored.length( ) )
	{
		// dont go over '>'
		// note: this is preparation for printf-like formatted text coloring
		// because we dont want to print '/' outside of "directory"
		// with same colors or smth
		if ( /* ( u32 ) */ resolved.find( '>' ) < pos )
			break;
		
		resolved.replace( pos, 1, colored );
	}

	resolved.replace( resolved.find( '>' ), 1, colored_ender );

	return resolved;
}

std::string n_sdk::c_basic_log::resolve_colors( const std::string& format )
{
	// for now we dont support base color resolving
	// todo: ^ ( base means printf-like formatted output )
	
	return resolve_cheat_wrappers( ) + this->m_owner + resolve_at_mark( )
		+ resolve_status( ) + resolve_directory_slashes( );
}
std::string n_sdk::c_basic_log::resolve_args( const char* format, va_list args )
{
	va_list args_cpy;
	va_copy( args_cpy, args );
	
	// prepare buffer with size we need
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
		= resolve_colors( format_parsed );
							// ^ not done yet so dont todo: <- this
	return colored_base     // | delete
		+ format_parsed; // <--+ this
}

n_sdk::c_basic_log& n_sdk::c_basic_log::update_directory( )
{
	// keep this->m_directory up-to-date for printing
	this->m_directory = "";
	for ( auto& folder : this->m_directory_stack )
		this->m_directory += '/' + folder;
	return *this;
}

n_sdk::c_basic_log& n_sdk::c_basic_log::store_directory_stack( )
{
	this->m_directory_stack_stored = this->m_directory_stack;
	update_directory( ); return *this;
}
n_sdk::c_basic_log& n_sdk::c_basic_log::restore_directory_stack( )
{
	this->m_directory_stack = this->m_directory_stack_stored;
	this->m_directory_stack_stored.clear( );
	update_directory( );
	return *this;
}

std::string n_sdk::c_basic_log::status_str( )
{
	switch ( *( this->m_state ) )
	{
		case n_sdk::state_t::started: return "started";
		case n_sdk::state_t::waiting: return "waiting";
		case n_sdk::state_t::working: return "working";
		case n_sdk::state_t::failed:  return "failed";
		case n_sdk::state_t::success: return "success";
		case n_sdk::state_t::idle:    return "idle";
		case n_sdk::state_t::warning: return "warning";
		case n_sdk::state_t::error:   return "error";
		case n_sdk::state_t::exit:    return "exit";
		default: return "undeclared";
	}
}

bool n_sdk::c_basic_log::as_true( )
{
	return 1;
}
bool n_sdk::c_basic_log::as_false( )
{
	return 0;
}

n_sdk::c_basic_log& n_sdk::c_basic_log::on_start( )
{
	log( "started - %s", n_sdk::c_time::now( ).c_str( ) ).
		set_state( n_sdk::state_t::waiting );
	return *this;
}

n_sdk::c_basic_log& n_sdk::c_basic_log::on_work( )
{
	//log( "started - %s", n_sdk::c_time::now( ).c_str( ) ).
	set_state( n_sdk::state_t::working );
	return *this;
}

// todo: reference to struct/class that this stores errors warnings... and then push
// error or other options for

// idea#log-procedure: global_log_obj have a mini-services for things like that so its easier to manage
// everything later on with alot of massages etc...

n_sdk::c_basic_log& n_sdk::c_basic_log::on_failed( )
{
	store_directory_stack( );
	
	set_state( n_sdk::state_t::failed ).
	set_directory( "/" );

	// note: later on we should store all warnings, errors ( /warning /error etc... )
	// in core log manager to then dump segregate specific massages like:
	// global_logger.get_errors( c_hooks_obj_here ) - returns vector/list... of strings with hook errors
	// mark with todo:^
	// pin: idea#log-procedure

	return *this;
}

n_sdk::c_basic_log& n_sdk::c_basic_log::on_success( )
{
	set_state( n_sdk::state_t::success ).log( "finished - %s", n_sdk::c_time::now( ).c_str( ) );//.
		set_state( n_sdk::state_t::idle ).log( "peepochill" );
	return *this;
}

n_sdk::c_basic_log& n_sdk::c_basic_log::on_error( )
{
	store_directory_stack( ).
		set_state( n_sdk::state_t::error ).
			push_directory( "error" );
	this->m_error_count++;

	return *this;
}

n_sdk::c_basic_log& n_sdk::c_basic_log::on_exit( )
{
	set_state( n_sdk::state_t::exit ).
		log( "exiting" ); return *this;
}

// todo: something like log stack so we push message and log-procedure
// executes it 1 at a tick so we wont have scuffed logs in console
// pin: idea#log-procedure

n_sdk::c_basic_log& n_sdk::c_basic_log::log( const char* format, ... )
{
	va_list args;
	va_start( args, format );

	std::string resolved = resolver( format, args ) + '\n';
	va_end( args );

	std::cout << resolved << std::flush;

	// check if this is log on_x
	if ( this->m_directory_stack_stored.empty( ) == 0 )
	{
		//&& this->m_directory_stack[ this->m_directory_stack.size( ) - 1 ] == "error" )
		restore_directory_stack( ); // after the error we should also have something like
									// insecure_mode ( ? )
		
	}
	return *this;
}
