#include "initialization.h"

#include "../memory/offsets.h"
#include "../memory/hooks.h"

// $(MSBuildProjectName)_$(PlatformTarget)_$(Configuration) <- target dll name

auto n_core::c_initialization::set_handle( u32 handle ) -> void
{
	this->m_handle = handle;
}
auto n_core::c_initialization::get_state( ) -> n_sdk::state_t
{
	return this->m_state;
}

bool n_core::c_initialization::start( )
{
	n_sdk::c_console::get( ).allocate_console( );
	
	std::thread offset_start( &n_core::c_offsets::start, &n_core::c_offsets::get( ) ); offset_start.detach( );
	std::thread hooks_start( &n_core::c_hooks::start, &n_core::c_hooks::get( )  ); hooks_start.detach( );

	// wait for all
	while ( 0xfdead )
	{
		if ( ( n_core::c_offsets::get( ).get_state( ) == n_sdk::state_t::exit
			|| n_core::c_offsets::get( ).get_state( ) == n_sdk::state_t::success
			|| n_core::c_offsets::get( ).get_state( ) == n_sdk::state_t::idle )
			&& ( n_core::c_hooks::get( ).get_state( ) == n_sdk::state_t::exit
				|| n_core::c_hooks::get( ).get_state( ) == n_sdk::state_t::success
				|| n_core::c_hooks::get( ).get_state( ) == n_sdk::state_t::idle ) ) break;
		
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}

	if ( n_core::c_offsets::get( ).get_state( ) == n_sdk::state_t::exit
		|| n_core::c_hooks::get( ).get_state( ) == n_sdk::state_t::exit ) return 0;
	
	return 1;
}

bool n_core::c_initialization::end( )
{
	n_core::c_hooks::get( ).end( );
	
	( void )getchar( ); n_sdk::c_console::get( ).free_console( );
	return FreeLibrary( ( HMODULE )this->m_handle );
}

bool n_core::c_initialization::main_thread( )
{
	if ( start( ) == 0 )
		return end( );

	while ( 0xffdead )
	{
		if ( GetAsyncKeyState( VK_END ) & 1 ) break;
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}

	return end( );
}
