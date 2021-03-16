#include <common.h>
#include <sdk.h>

#include "../core/memory.h"

bool init_objs( )
{
	// " init objects "
	( void )n_sdk::c_console::get( ); // allocate asap

	auto& memory = n_core::c_memory::get( );
	// move later to more suitable place or organize attach event
	if ( memory.get_offsets( ) == 0 ) return 0;
	if ( memory.hook( ) == 0 ) return 0;

	return 1;
}

bool cleanup( )
{
	auto& memory = n_core::c_memory::get( );
	
	// stop routines in this line
	memory.unhook( );
	memory.hooking.free_mh( );

	//n_sdk::c_console::get( ).free_console( );
	
	( void )getchar( ); ( void )getchar( );
	return 1;
}

void on_attach( void* handle )
{
	// init objs
	if ( init_objs( ) != 1 )
		goto failed;

	// loop
	while ( 0xffdead )
	{
		if ( GetAsyncKeyState( VK_END ) & 1 ) break;
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}

failed:
	cleanup( );

	FreeLibrary( ( HMODULE )handle );
}