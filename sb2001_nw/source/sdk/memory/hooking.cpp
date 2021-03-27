#include <common.h>

#include <minhook/MinHook.h>
#pragma comment(lib, "minhook/libMinHook-x86-v142.lib")

#include "hooking.h"

bool n_sdk::c_hooking::minhook_start( )
{
	return ( MH_Initialize( ) == MH_OK );
}
bool n_sdk::c_hooking::minhook_end( )
{
	return ( MH_Uninitialize( ) == MH_OK );
}

n_sdk::hook_t n_sdk::c_hooking::make_hook( const std::string& hook_name, u32 location, abyss hook, abyss original )
{	
	return n_sdk::hook_t { hook_name, ( abyss )location, hook, original };
}

bool n_sdk::c_hooking::enable( n_sdk::hook_t& hook )
{
	// check if hook is valid
	if ( n_sdk::c_hooking::is_valid( hook ) == 0 )
		return 0;

	if ( ( MH_CreateHook( hook.m_location, hook.m_hook, 
							reinterpret_cast< abyss* >( hook.m_original ) ) != MH_OK )
		|| ( MH_EnableHook( hook.m_location ) != MH_OK ) )
		return 0;

	hook.m_enabled = 1;
	return 1;
}

bool n_sdk::c_hooking::disable( n_sdk::hook_t& hook )
{
	if ( n_sdk::c_hooking::is_valid( hook ) == 0 )
		return 0;

	if ( MH_DisableHook( hook.m_location ) != MH_OK ) 
		return 0;

	hook.m_enabled = 0;

	return 1;
}

bool n_sdk::c_hooking::is_valid( const n_sdk::hook_t& hook )
{
	// hook is valid when:
	//  - hook_name != f_null
	//	- location != 0
	//	- hook != nullptr
	//	- original != nullptr

	if ( hook.m_hook_name  == "f_null"
		|| hook.m_location == nullptr
		|| hook.m_hook     == nullptr
		|| hook.m_original == nullptr ) return 0;
	
	return 1;
}

bool n_sdk::c_hooking::do_action( n_sdk::hook_t& hook, e_hook_action action )
{
	// we need to check if hook was even enabled to disable it
	return ( bool ) action ? enable( hook ) : disable( hook );
}
