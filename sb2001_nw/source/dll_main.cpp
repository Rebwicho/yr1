#include "_include/common.h"

void on_attach( void* handle );

int __stdcall DllMain( void* handle, ul32 call_reason, void* )
{
	switch ( call_reason )
	{
		case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls( ( HMODULE )handle ); // disable thread attach / detach shit

			// spawn thread and detach from new thread
			std::thread on_attach_thread( on_attach, handle ); on_attach_thread.detach( );
			
			break;
		}

		default: break;
	}

	return 1; // yeet 1 we only care about dll_process_attach
}