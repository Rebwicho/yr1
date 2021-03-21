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
	return n_sdk::hook_t { hook_name,  ( abyss )location, hook, original };
}

bool n_sdk::c_hooking::enable( const s_hook& hook )
{
	// check if hook is valid
	if ( n_sdk::c_hooking::is_valid( hook ) == 0 )
		return 0;

	if ( ( MH_CreateHook( hook.m_location, hook.m_hook, 
							reinterpret_cast< abyss* >( hook.m_original ) ) != MH_OK )
		|| ( MH_EnableHook( hook.m_location ) != MH_OK ) )
	{
		//printf( "[ sb2001_nw ]: hooks@failed/function/%s> %#x -> %#x { %#x } &failed\n", 
		//	hook.m_hook_name.c_str( ), ( u32 )hook.m_location,
		//	( u32 )hook.m_hook, ( u32 )hook.m_original );
		return 0;
	}

	//printf( "[ sb2001_nw ]: hooks@waiting/function/%s> %#x -> %#x { %#x } &enabled\n",
	//	hook.m_hook_name.c_str( ), ( u32 )hook.m_location, 
	//	( u32 )hook.m_hook, ( u32 )hook.m_original );
	return 1;
}

bool n_sdk::c_hooking::disable( const s_hook& hook )
{
	if ( n_sdk::c_hooking::is_valid( hook ) == 0 )
		return 0;

	if ( MH_DisableHook( hook.m_location ) != MH_OK ) 
	{
		//	printf( "[ sb2001_nw ]: hooks@waiting - hooking...\n" );
		//printf( "[ sb2001_nw ]: hooks@failed/function/%s> %#x -> %#x { %#x } &failed\n",
		//	hook.m_hook_name.c_str( ), ( u32 )hook.m_location,
		//	( u32 )hook.m_hook, ( u32 )hook.m_original );
		return 0;
	}

	//printf( "[ sb2001_nw ]: hooks@waiting/function/%s> %#x -> %#x { %#x } &disabled\n",
	//	hook.m_hook_name.c_str( ), ( u32 )hook.m_location,
	//	( u32 )hook.m_hook, ( u32 )hook.m_original );
	return 1;
}

bool n_sdk::c_hooking::is_valid( const s_hook& hook )
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

bool n_sdk::c_hooking::do_action( const s_hook& hook, const e_hook_action action )
{
	return ( bool ) action ? enable( hook ) : disable( hook );
}
