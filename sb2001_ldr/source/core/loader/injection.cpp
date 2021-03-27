#include <common.h>
#include <sdk.h>

#include "injectables.h"

#include "injection.h"

void n_core::c_injection::set( const n_core::injectable_t injectable )
{
	this->m_injectable = n_core::injectable_t( );
	
	if ( injectable.m_injectable_type == injectable_type_t::invalid )
	{
		std::wcout << "[ sb2001_ldr ]: specified injectable is invalid" << std::endl;
		return;
	}

	this->m_injectable = injectable;
}

void n_core::c_injection::set_mode( e_injection_mode injection_mode )
{
	// check if we have valid injectable
	if ( this->is_valid_injectable( ) == 0 )
	{
		std::wcout << "[ sb2001_ldr ]: cant set mode when injectable is invalid" << std::endl;
		return;
	}
	
	// check if chosen inject mode is supported yet
	if ( injection_mode >= e_injection_mode::invalid || injection_mode < 0 )
	{
		std::wcout << "[ sb2001_ldr ]: specified injection mode is invalid" << std::endl;
		return;
	}

	this->m_injection_mode = injection_mode;
}

bool n_core::c_injection::execute( )
{
	// check if we have injectable
	if ( this->is_valid_injectable( ) == 0 )
	{
		std::wcout << "[ sb2001_ldr ]: cant inject when injectable is invalid" << std::endl;
		return 0;
	}
	
	// check if injection mode is valid
	if ( this->is_valid_mode( ) == 0 )
	{
		std::wcout << "[ sb2001_ldr ]: cant inject when injection mode is invalid" << std::endl;
		return 0;
	}

	// execute injection
	n_sdk::c_process target;

	std::wcout << "[ sb2001_ldr ]: waiting for - " << this->m_injectable.m_process_name << "..." << std::endl;
	
	//target.prepare( this->m_injectable.m_process_name );
	while ( target.prepare( this->m_injectable.m_process_name ) == 0 )
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
		
	// there we will check if certain modules are loaded
	// todo: not hardcoded
	// note: check what modules must exist in a process to successfully load dll to target
	std::wcout << "[ sb2001_ldr ]: found process -> checking modules..." << std::endl;
	while ( 0xffdead )
		if ( target.get_module( L"ntdll.dll" )
			&& target.get_module( L"kernel32.dll" )
			&& target.get_module( L"combase.dll" )
			&& target.get_module( L"user32.dll" ) 
			//&& target.get_module( L"ewdl.ews" )
			)
		{
			std::wcout << "[ sb2001_ldr ]: found modules -> injecting..." << std::endl;
			break;
		}

	if ( target.manual_map( this->m_injectable.m_injectable_file_path.wstring( ) ) == 0 )
	{
		std::wcout << "[ sb2001_ldr ]: injection failed" << std::endl;
		return 0;
	}

	std::wcout << "[ sb2001_ldr ]: injection succeed" << std::endl;
	
	return 1;
}

bool n_core::c_injection::is_valid_injectable( )
{
	return ( this->m_injectable.m_injectable_type != injectable_type_t::invalid );
}

bool n_core::c_injection::is_valid_mode( )
{
	return ( this->m_injection_mode < e_injection_mode::invalid && this->m_injection_mode >= 0 );
}
