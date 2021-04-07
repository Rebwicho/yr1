#include <common.h>

#include "process_impl.h"

bool n_sdk::c_process::prepare( const std::wstring process_name )
{
	this->m_target_process_name = process_name;
	// std::wcout << "[ c_process::prepare ]: preparing " << process_name << "..." << std::endl;

	auto pid = this->get_pid( process_name );
	if ( pid == 0 )
	{
		// std::wcout << "[ c_process::prepare ]: failed, cant find process named" << process_name <<  std::endl;
		return 0;
	}
	
	// store
	//this->m_target_pid = pid;
	this->m_process.Attach( pid );

	// those work like "initializers" for certain blackbone classes etc
	// or gather needed info for further actions
	auto& core = this->m_process.core( );
	auto barrier = this->m_process.barrier( );
	this->m_target_pid = core.pid( );
	auto handle = core.handle( );

	blackbone::PEB_T peb = { };
	auto peb_ptr = core.peb( &peb );
	
	return 1;
}

bool n_sdk::c_process::manual_map( const std::wstring file_path )
{
	//std::wcout << "[ c_process::manual_map ]: executing manual map injection into " << this->m_target_process_name << "..." << std::endl;
	
	auto mapped_image = this->m_process.mmap( ).MapImage( file_path,
		blackbone::NoThreads | blackbone::ManualImports | blackbone::WipeHeader | blackbone::RebaseProcess );
	if ( !mapped_image )
	{
		std::wcout << "[ c_process::manual_map ]: failed to inject with error 0x" << std::hex << mapped_image.status
			<< " " << blackbone::Utils::GetErrorDescription( mapped_image.status ) << std::endl;
		return 0;
	}

	//std::wcout << "[ c_process::manual_map ]: injected" << std::endl;
	
	return 1;
}

u32 n_sdk::c_process::get_module( std::wstring module_name )
{
	auto& modules = this->m_process.modules( );
	auto module_result = modules.GetModule( module_name );
	if ( module_result )
		return module_result->baseAddress;

	return 0;
}

u32 n_sdk::c_process::get_pid( std::wstring process_name )
{
	auto processes_found = blackbone::Process::EnumByName( process_name );
	if ( processes_found.size( ) == 0 ) return 0;

	// todo: return first process that dont have our injectable loaded
	return ( u32 )( processes_found.front( ) /* only return pid of process that was first */ );
}