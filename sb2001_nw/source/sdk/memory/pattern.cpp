#include <common.h>

#include "pattern.h"

u32 rip_convert_call( u32 start, u32 offset )
{
	BYTE bResult[ 0x8 ] = { 0x0 };
	memcpy( bResult, ( PVOID )( start + offset ), 0x4 );

	if ( bResult[ 3 ] == 0xFF )
	{
		bResult[ 4 ] = 0xFF;
		bResult[ 5 ] = 0xFF;
		bResult[ 6 ] = 0xFF;
		bResult[ 7 ] = 0xFF;
	}

	return ( start + offset + *( ( PDWORD32 )bResult ) + 0x4 );
}

n_sdk::c_pattern_module::s_pattern& n_sdk::c_pattern_module::pat_call( std::string name, std::deque<BYTE> pattern, u32 offset )
{
	//auto obj = new s_pattern( name, pattern, offset, 0, 1 );
	return this->m_patterns_queue.emplace_back( name, pattern, offset, 0, 1 );
	//return obj;
}
n_sdk::c_pattern_module::s_pattern& n_sdk::c_pattern_module::pat_pure( std::string name, std::deque<BYTE> pattern, u32 offset )
{
	//auto obj = new s_pattern( name, pattern, offset, 1, 1 );
	return this->m_patterns_queue.emplace_back( name, pattern, offset, 1, 1 );
	//return obj;
}

u32 n_sdk::c_pattern_module::process_queue( )
{
	if ( this->m_module_base == 0 ) return 0;
	
	u32 f = 0;
	auto start_address = this->m_module_base;

	PIMAGE_DOS_HEADER dos_header = { };
	PIMAGE_NT_HEADERS nt_header = { };

	dos_header = ( PIMAGE_DOS_HEADER )start_address;
	nt_header = ( PIMAGE_NT_HEADERS )( ( PBYTE )( dos_header ) + dos_header->e_lfanew );

	PIMAGE_SECTION_HEADER section_header = IMAGE_FIRST_SECTION( nt_header );
	
	auto end_address = start_address + section_header->Misc.VirtualSize; // 0x7FFFFFFF

	// printf( "[ pattern_queue ]: start_address: %#x end_address: %#x\n", start_address, end_address );

	for ( auto c = start_address; c < end_address; c += 1 ) // xd
	{
		if ( f == this->m_patterns_queue.size( ) ) break;
		auto curbyte = *( BYTE* )( c );
		for ( auto& obj : this->m_patterns_queue )
		{
			if ( obj.cur_pattern.empty( ) || obj.res ) continue;
			/*auto skip already found patterns*/
			if ( obj.cur_pattern.front( ) != 0xFF && curbyte != obj.cur_pattern.front( ) )
				/*seems we dont have a wildcard neither a match need to reset!*/
			{
				obj.cur_pattern = obj.pattern;
				/*restart the search as we have failed lol*/
				obj.found_bytes.clear( ); /*clear found bytes*/
				obj.first_match = 0; /*clear address that was first matched*/
				continue;
			}
			if ( curbyte == obj.cur_pattern.front( ) )
				/*we have found a byte matching the next byte we look for*/
			{
				if ( !obj.first_match ) obj.first_match = c;
				/*add the address as first matched so it can be added as start of the pattern to the results*/
				obj.cur_pattern.pop_front( ); /*remove this byte from the searching queue and add to the found list*/
				obj.found_bytes.push_back( curbyte );
				/*add the current byte to the list of found bytes*/
				if ( obj.cur_pattern.empty( ) ) /*check if we have completed the scanning*/
				{
					if ( !obj.is_pure ) /*adjust the result based on our selection when registering the object*/
					{
						if ( obj.offset ) obj.first_match += obj.offset;
						auto b = rip_convert_call( obj.first_match, 0x1 );
						obj.first_match = b;
					}
					else if ( obj.offset ) obj.first_match += obj.offset;

					obj.res = obj.first_match;

					/*add the result to our list*/
					obj.first_match = 0; /*reset address that was first matched*/
					obj.found_bytes.clear( );
					f++;
					continue;
				}
				continue;
			}
			if ( obj.cur_pattern.front( ) == 0xFF )
				/*check for wildcard match*/
			{
				obj.cur_pattern.pop_front( );
				/*delete the wildcard from search*/
				obj.found_bytes.push_back( curbyte );
				/*add the current byte to the list of found bytes*/
				if ( obj.cur_pattern.empty( ) )
					/*check if we have completed the scanning*/
				{
					if ( !obj.is_pure ) /*adjust the result based on our selection when registering the object*/
					{
						if ( obj.offset ) obj.first_match += obj.offset;
						auto b = rip_convert_call( obj.first_match, 0x1 );
						obj.first_match = b;
					}
					else if ( obj.offset ) obj.first_match += obj.offset;

					obj.res = obj.first_match;

					/*add the result to our list*/
					obj.first_match = 0; /*reset address that was first matched*/
					obj.found_bytes.clear( );
					f++;
					continue;
				}
			}
		}
	}
	
	for ( auto& obj : this->m_patterns_queue )
		obj.m_f_on_end( );

	return f;
}

auto n_sdk::c_pattern_module::get_module_name( ) -> std::string
{
	std::string formatted = this->m_module_name;
	for ( u32 pos = 0;
		( pos = formatted.find( '.', pos ) ) != std::string::npos;
		pos++ )
		formatted.replace( pos, 1, "_" );

	return formatted;
}

auto n_sdk::c_pattern_module::get_module_name_pure( ) -> std::string
{
	return this->m_module_name;
}

auto n_sdk::c_pattern_module::get_module_base( ) -> u32
{
	return this->m_module_base;
}

auto n_sdk::c_pattern_module::get_queue( ) -> std::deque< s_pattern >&
{
	return this->m_patterns_queue;
}

auto n_sdk::c_pattern_module::set_module_base( const u32& module_base ) -> void
{
	this->m_module_base = module_base;
}

bool n_sdk::c_pattern_module::on_found( )
{
	return this->m_found_module == 0 ? ( this->m_found_module = 1 ) : 0;
}
