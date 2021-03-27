#include <common.h>
#include <sdk.h>

#include "injectables.h"

void n_core::c_injectables::scan( std::filesystem::path file_path )
{
	// get dlls from path
	auto dll_list = n_sdk::c_file_search::get_dll_files( file_path );

	// check if dll is an injectable
	for ( auto dll : dll_list )
	{
		auto type = this->get_injectable_type( dll );
		if ( type >= e_injectable_type::invalid || type < e_injectable_type( 0 ) ) // invalid type
			continue;

		// check if we support process for that type
		auto injectable_process_name = this->get_injectable_process( type );
		if ( injectable_process_name == L"not supported" ) // not supported
			continue;

		// seems like we support that dll

		this->m_injectables.emplace_back( injectable_process_name, dll.filename( ).wstring( ),
			dll, type );
	}

	// print injectables
	// todo: move to proper place

	std::wcout << "[ sb2001_ldr ]: list of found injectables:" << std::endl;

	for ( u32 it = 0; it < this->m_injectables.size( ); it++ )
	{
		std::wcout << "[ sb2001_ldr ]: " << this->m_injectables[ it ].m_injectable_name << " - [ " << it << " ];"
			<< std::endl;
	}
}

n_core::c_injectables::s_injectable n_core::c_injectables::get_injectable( u32 injectable_id )
{
	if ( injectable_id >= this->m_injectables.size(  ) || this->m_injectables.size( ) < 0 )
	{
		auto invalid_injectable = n_core::c_injectables::s_injectable( );
		return n_core::c_injectables::s_injectable( );
	}

	std::wcout << "[ sb2001_ldr ]: selected -> " << this->m_injectables[ injectable_id ].m_injectable_name << std::endl;

	return this->m_injectables[ injectable_id ];
	
	//for ( auto injectable : this->m_injectables )
	//	if ( injectable.m_injectable_type == injectable_type ) return injectable;
}

std::wstring n_core::c_injectables::get_injectable_process( e_injectable_type injectable_type )
{
	if ( injectable_type >= this->m_process_names.size( ) ) return L"not supported";

	return this->m_process_names[ injectable_type ];
}

n_core::c_injectables::e_injectable_type n_core::c_injectables::get_injectable_type( std::filesystem::path file_path )
{
	// conditions:
	//	- dll name begins with sb2001_
	//	- after 1st '_' char dll contains some shortened cheat name check if
	//		we support that name

	auto file_name = file_path.filename( ).wstring( );
	if ( file_name.starts_with( L"sb2001_" ) == 0 ) return e_injectable_type::invalid;

	for ( u32 it = 0; it < this->m_supported_injectables.size( ); it++ )
	{
		std::wstringstream type; type << L"_" << this->m_supported_injectables[ it ] << L"_";

		if ( file_name.find( type.str( ) ) != std::wstring::npos )
			return ( e_injectable_type )it;
	}
		
	return e_injectable_type::invalid;
}
