#include <common.h>

#include "core/procedures/initialization.h"

int __stdcall DllMain( void* handle, ul32 call_reason, void* )
{
	switch ( call_reason )
	{
		case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls( ( HMODULE )handle );
			
			n_core::c_initialization::get( ).set_handle( ( u32 )handle );
			std::thread( &n_core::c_initialization::main_thread, &n_core::c_initialization::get( ) )
				.detach( );
			
			break;
		}

		default: break;
	}

	return 1;
}