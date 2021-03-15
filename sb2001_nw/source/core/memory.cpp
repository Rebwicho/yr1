#include <common.h>
#include <sdk.h>

#include "memory.h"
#include "hooks/hooks.h"

typedef n_sdk::c_hooking::s_hook hook_t;

bool n_core::c_memory::get_offsets( )
{
	auto noswings_dll_queue = new n_sdk::c_pattern_module( ( u32 )GetModuleHandleA( "NosWings.dll" ) );

	// 56 8B F1 80 3E ff 74 ff 57 8B 3D ff ff ff ff 90 E8 ff ff ff ff 84 C0 75 ff E8 ff ff ff ff 84 C0 75 ff E8 ff ff ff ff 84 C0 75 ff E8 ff ff ff ff 84 C0 75 ff E8 ff ff ff ff 84 C0 75 ff E8 ff ff ff ff 84 C0 75 ff E8 ff ff ff ff 84 C0 75 ff E8 ff ff ff ff 84 C0 75 ff E8 ff ff ff ff 84 C0 75 ff 68 E8 03 00 00 FF ff 80 3E ff 75 ff 5F
	auto f_anti_debug_obj = noswings_dll_queue->pat_pure( "f_anti_debug",
		{ 0x56, 0x8B, 0xF1, 0x80, 0x3E, 0xff, 0x74, 0xff, 0x57, 0x8B, 0x3D, 0xff, 0xff, 0xff, 0xff, 0x90, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0x68, 0xE8, 0x03, 0x00, 0x00, 0xFF, 0xff, 0x80, 0x3E, 0xff, 0x75, 0xff, 0x5F },
		0x0 );

	this->add_pattern_module( noswings_dll_queue );

	this->process_pattern_modules( );

	bool result = 0;
	
	// save found 
	result = this->write_offset( f_anti_debug_obj->res, this->offset.f_anti_debug, "f_anti_debug" );
	if ( result == 0 ) return 0;

	// clean up
	this->cleanup_pattern_queue( );
	
	return 1;
}

bool n_core::c_memory::hook( )
{
	if ( this->hooking.init_mh( ) != 0 ) return 0;

	//this->hooking.register_hook( hook_t( "f_anti_debug", this->offset.f_anti_debug, )  )

	auto hooking_res = this->hooking.hook_all( );
	if ( hooking_res != 0x101 ) // if not 0x101 failed
	{
		// print where failed
		printf( "[ c_memory::hooking ]: failed to hook %i\n", hooking_res );
		return 0;
	}

	return 1;
}

bool n_core::c_memory::unhook( )
{
	if ( this->hooking.unhook_all( ) != 0 ) // failed
	{
		printf( "[ c_memory::hooking ]: failed to unhook\n" );
		return 0;
	}
	return 1;
}

bool n_core::c_memory::write_offset( u32 src, u32& dst, const std::string& offset_name )
{
	if ( src == 0 )
	{
		printf( "[ c_memory::write_offset ]: couldn't find %s, update pattern!\n", offset_name.c_str( ) );

		// getchar( );
		// getchar( );

		return 0;
	}

	dst = src; // write
	
	printf( "[ c_memory::write_offset ]: %s: %#x / dbg org: %#x\n", offset_name.c_str( ), dst, src );

	return 1;
}

void n_core::c_memory::add_pattern_module( n_sdk::c_pattern_module* pattern_module )
{
	this->m_pattern_module_q.push_back( pattern_module );
}

void n_core::c_memory::process_pattern_modules( )
{
	for ( u32 i = 0; i < this->m_pattern_module_q.size( ); i++ )
		this->m_pattern_module_q[ i ]->process_queue( );
}

void n_core::c_memory::cleanup_pattern_queue( )
{
	for ( u32 i = 0; i < this->m_pattern_module_q.size( ); i++ )
		delete this->m_pattern_module_q[ i ];
}
