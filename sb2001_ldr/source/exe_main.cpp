#include <common.h>
#include <sdk.h>

#include "core/loader/injectables.h"
#include "core/loader/injection.h"

int main( int, char* )
{
	n_core::c_injectables::get( ).scan( );
	std::wcout << "[ sb2001_ldr ]: select injectable: ";
	u32 selected_injectable;
	std::wcin >> selected_injectable;

	n_core::c_injection::get( ).set(
		n_core::c_injectables::get( ).get_injectable( selected_injectable ) );

	// for now we skip injection mode todo: support it :0
	n_core::c_injection::get( ).set_mode( n_core::injection_mode_t::manualmap );
	n_core::c_injection::get( ).execute( );

	getchar( );
	getchar( );
	
	return 1;
}
