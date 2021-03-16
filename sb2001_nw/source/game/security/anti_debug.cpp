#include <common.h>
#include <sdk.h>

#include "anti_debug.h"

void n_game::n_security::c_anti_debug::set_function( u32 location )
{
	this->m_func_location = location;
}

void n_game::n_security::c_anti_debug::restore( )
{
	// todo
}

bool n_game::n_security::c_anti_debug::patch( )
{
	// check if we have function location
	if ( this->m_func_location == 0x0 ) return 0;
	
	// check if we can find patch location
	if ( this->find_patch_location( ) == 0 ) return 0;
	
	// check if we can find sleep call location
	if ( this->find_sleep_call_location( ) == 0 ) return 0;

	// store bytes for later
	// this->store_original_bytes( );

	// unprotect patching location
	DWORD old_protection = 0;
	VirtualProtect( ( u32* )( this->m_patch_location ), 5, PAGE_EXECUTE_READWRITE, &old_protection );

	// patch bytes
	// todo: not hardcoded
	*( u8* )( this->m_patch_location )       = 0x80;
	*( u8* )( this->m_patch_location + 0x1 ) = 0x3e;
	*( u8* )( this->m_patch_location + 0x2 ) = 0x00;
	*( u8* )( this->m_patch_location + 0x3 ) = 0x75;
	*( u8* )( this->m_patch_location + 0x4 ) = this->calculate_jnz( ); // 0xef; // this->calculate_jnz( );
	
	// protect with old protection
	VirtualProtect( ( u32* )( this->m_patch_location ), 5, old_protection, &old_protection );

	// done
	printf( "[ c_anti_debug::patch ]: success\n" );
	
	return 1;
}

void n_game::n_security::c_anti_debug::store_original_bytes( )
{
	// todo
}

bool n_game::n_security::c_anti_debug::find_patch_location( )
{
	// push 0xff [ 0x68, 0xff, 0x00, 0x00, 0x00 ]
	// call [ 0xff ]
	if ( this->m_func_location == 0x0 )
	{
		printf( "[ c_anti_debug::find_patch_location ]: m_func_location is 0x0\n" );
		return 0;
	}
	
	std::deque< u8 > patch_location_bytes = { 0x68, 0xff, 0x00, 0x00, 0x00, 0xff };

	u32 patch_location = this->byte_search( patch_location_bytes );
	if ( patch_location == 0x0 ) return 0;

	this->m_patch_location = patch_location;
	
	printf( "[ c_anti_debug::find_patch_location ]: found: %#x\n", this->m_patch_location );
	
	return 1;
}

bool n_game::n_security::c_anti_debug::find_sleep_call_location( )
{
	// push 0x64 [ 0x6a, 0x64 ]
	// call edi  [ 0xff, 0xd7 ]
	if ( this->m_func_location == 0x0 )
	{
		printf( "[ c_anti_debug::find_sleep_call_location ]: m_func_location is 0x0\n" );
		return 0;
	}
	
	std::deque< u8 > sleep_call_location_bytes = { 0x6a, 0x64, 0xff, 0xd7 };

	u32 sleep_call_location = this->byte_search( sleep_call_location_bytes );
	if ( sleep_call_location == 0x0 ) return 0;

	this->m_sleep_location = sleep_call_location;

	printf( "[ c_anti_debug::find_sleep_call_location ]: found: %#x\n", this->m_sleep_location );

	return 1;
}

u8 n_game::n_security::c_anti_debug::calculate_jnz( )
{
	return ( u8 )( this->m_sleep_location - ( this->m_patch_location + /* shift to location after jnz instruction */ 0x5 ) );
}

u32 n_game::n_security::c_anti_debug::byte_search( std::deque<u8> bytes_to_find )
{
	if ( this->m_func_location == 0x0 )
	{
		printf( "[ c_anti_debug::byte_search ]: m_func_location is 0x0\n" );
		return 0;
	}
	
	u32 first_match = 0x0;
	u32 match_count = 0x0;

	std::deque< u8 > a1 = bytes_to_find;
	
	// function_location + 0x78 = 0xcc byte so we scan i < function_location + 0x78 ...
	for ( u32 current_location = this->m_func_location; current_location < this->m_func_location + 0x78; current_location++ )
	{
		// read byte from current location
		u8 current_byte = *( u8* )current_location;

		// check if current byte matches byte we looking for
		if ( current_byte == a1[ match_count ] )
		{
			// we got a match store first match address if there is none
			if ( first_match == 0x0 ) first_match = current_location;

			// check if all bytes match
			if ( match_count == a1.size( ) - 1 ) break;
			
			// increase match count
			match_count++;
		}
		else // bytes are different - reset
		{
			if ( match_count != 0x0 )
			{
				match_count = 0x0;
				first_match = 0x0;			
			}			
		}	
	}

	// search failed
	if ( first_match == 0x0 )
	{
		printf( "[ c_anti_debug::byte_search ]: failed to find [ " );
		for ( auto byte : a1 ) printf( "%c ", byte ); printf( "]\n" );
	}

	return first_match;
}
