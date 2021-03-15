#include <common.h>

#include <minhook/MinHook.h>
#pragma comment(lib, "minhook/libMinHook-x86-v141-mdd.lib")

#include "hooking.h"

void n_sdk::c_hooking::register_hook( s_hook hook )
{
	this->m_hook_queue.push_back( hook );
}

u32 n_sdk::c_hooking::hook_all( )
{
	for ( u32 i = 0; i < this->m_hook_queue.size( ); i++ )
		if ( this->m_hook_queue[ i ].execute( ) == 0 ) return i;

	return 0x101;
}

u32 n_sdk::c_hooking::unhook_all( )
{
	return MH_DisableHook( MH_ALL_HOOKS );
}

bool n_sdk::c_hooking::init_mh( )
{
	return MH_Initialize( ) == MH_OK;
}

bool n_sdk::c_hooking::free_mh( )
{
	return MH_Uninitialize( ) == MH_OK;
}

bool n_sdk::c_hooking::s_hook::execute( )
{
	printf( "[ c_memory::hooking ]: hooking %s at %#x ", this->m_hook_name.c_str( ), this->m_func_address );

	auto create_hook = MH_CreateHook( ( void* )this->m_func_address, this->m_func_hook, reinterpret_cast< LPVOID* >( this->m_func_orig ) );
	if ( create_hook != MH_OK )
	{
		printf( "failed to create hook\n" );

		return 0;
	}

	auto enable_hook = MH_EnableHook( ( void* )this->m_func_address );
	if ( enable_hook != MH_OK )
	{
		printf( "failed to enable hook\n" );

		return 0;
	}

	printf( "done -> f_hook: %#x f_orig: %#x\n", this->m_func_hook, ( void* )this->m_func_orig );

	return 1;
}