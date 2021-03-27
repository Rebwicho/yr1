#include "offsets.h"

auto n_core::c_offsets::get_state( ) -> n_sdk::state_t
{
	return this->m_state;
}

bool n_core::c_offsets::waiter( )
{
	//log( "waiting for bones" ).
	log( "waiting for modules" ).
		push_directory( "modules" );
	
	u32 loaded_modules_count = 0,
		max_modules_count = this->m_pattern_modules.size( );
	
	while ( on_waiter(  ) == 1 )
	{
		// wait for all modules we need
		for ( auto& scanner_module : this->m_pattern_modules )
		{
			// if we already have that module 
			if ( scanner_module.get_module_base( ) != 0 )
				continue; // already found, skip

			// we dont have that module yet, check
			auto module_base = ( u32 )GetModuleHandleA( scanner_module.get_module_name_pure( ).c_str( ) );
			if ( module_base == 0 ) continue;

			scanner_module.set_module_base( module_base );
			
			push_directory( scanner_module.get_module_name( ) ).log( "found@%#x", module_base )
				.pop_directory( );
		
			loaded_modules_count++;
		}

		// if we have all modules stop waiter we good
		if ( loaded_modules_count == max_modules_count )
			return log( "found %i/%i", loaded_modules_count, max_modules_count )
				.pop_directory( )
					.as_true( );

		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}

	// seems like we dont have all modules we need after waiting for 5s
	// fail, something is wrong
	return on_error( ).log( "found %i/%i", loaded_modules_count, max_modules_count )
		.pop_directory( )
			.as_false( );
}

bool n_core::c_offsets::worker( )
{
	on_work( ).log( "scanning modules" );
	return walk_modules( ) == 0 ? 0 : 1;	
}

bool n_core::c_offsets::on_waiter( )
{
	static u32 start_point = GetTickCount( );

	// if we are waiting for modules more than 5s something is wrong
	return start_point + 5000 <= GetTickCount( ) ? 0 : 1;
}
bool n_core::c_offsets::on_start( )
{
	register_patterns( );
	return bind_state( &this->m_state ).set_owner( "offsets" ).on_start( ).as_true( );
}
bool n_core::c_offsets::on_end( )
{
	return 1;
}

void n_core::c_offsets::start( )
{
	on_start( );

	if ( waiter( ) == 0 ) 
	{
		on_failed( ).log( "waiter failed" ).set_state( n_sdk::state_t::exit );
		return;
	}
	if ( worker( ) == 0 ) 
	{
		on_failed( ).log( "worker failed" ).set_state( n_sdk::state_t::exit );
		return;
	}

	on_success( );
}

void n_core::c_offsets::end( )
{
	
}

bool n_core::c_offsets::on_pattern_end( const u32 value, u32& container, const std::string& offset_name )
{
	push_directory( offset_name );

	if ( value != 0 ) 
		container = value;
	
	return value == 0 ? on_error( ).log( "%#x -> invalid", value ).pop_directory(  ).as_false(  ) : log( "%#x -> valid", value ).pop_directory( ).as_true( );
}

n_sdk::c_pattern_module& n_core::c_offsets::register_module( const std::string& module_name )
{
	return this->m_pattern_modules.emplace_back( module_name );
}

void n_core::c_offsets::register_patterns( )
{
	// for the time being we test stuff on official
	// when noswings server open swap names and other stuff in project
	// todo: ^ !!!
	auto& client_exe = register_module( "NostaleClientX.exe" ); 
	//auto& ewdl_ews = register_module( "ewdl.ews" ); 
	//auto& d3d9_dll = register_module( "d3d9.dll" ); 
	//auto& d3d11_dll = register_module( "d3d11.dll" ); 
	//auto invalid = register_module( "d3d11.dll" );

	// network_ptr = A1 ? ? ? ? 8B ? E8 ? ? ? ? C3 ? ? ? 8b ca
	//					^ - starts here
	
	auto& f_send = client_exe.pat_pure( "f_send", { 0x53, 0x56, 0x8b, 0xf2, 0x8b, 0xd8, 0xeb, 0x04 }, 0x0 );
	auto& f_recv = client_exe.pat_pure( "f_recv", { 0x55, 0x8B, 0xEC, 0x83, 0xff, 0xff, 0x53, 0x56, 0x57, 0x33, 0xC9, 0x89, 0x4D, 0xF4, 0x89, 0x55, 0xFC, 0x8B, 0xD8, 0x8B, 0x45, 0xFC, 0xE8, 0xff, 0xff, 0xff, 0xFF, 0x33, 0xC0, 0x55, 0x68, 0xff, 0xff, 0xff, 0xff, 0x64, 0xFF, 0x30 }, 0x0 );
	f_send.m_f_on_end = [ & ]( ) -> bool {
		return on_pattern_end( f_send.res, offset::get( ).f_send, "f_send" );
	};
	f_recv.m_f_on_end = [ & ]( ) -> bool {
		return on_pattern_end( f_recv.res, offset::get( ).f_recv, "f_recv" );
	};
	
	//auto& noswings_dll = register_module( "NosWings.dll" );
	//auto& anti_debug_pat = noswings_dll.pat_pure( "f_anti_debug",
	//	{ 0x56, 0x8B, 0xF1, 0x80, 0x3E, 0xff, 0x74, 0xff, 0x57, 0x8B, 0x3D, 0xff, 0xff, 0xff, 0xff, 0x90, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0xE8, 0xff, 0xff, 0xff, 0xff, 0x84, 0xC0, 0x75, 0xff, 0x6a, 0x64, 0xFF, 0xff, 0x80, 0x3E, 0xff, 0x75, 0xff, 0x5F },
	//	0x0 );
	//anti_debug_pat.m_f_on_end = [ & ]( ) -> bool {
	//	return n_core::c_offsets::on_pattern_end( anti_debug_pat.res, offset::get( ).f_anti_debug, "f_anti_debug" );
	//};

}

bool n_core::c_offsets::walk_modules( )
{
	push_directory( "modules" );
	
	u32 found_offsets_count = 0,
		max_offsets_count = 0;
	
	for ( auto& pattern_module : this->m_pattern_modules )
	{
		push_directory( pattern_module.get_module_name( ) );

		max_offsets_count = pattern_module.get_queue( ).size( );
		found_offsets_count = pattern_module.process_queue( );
		( found_offsets_count == max_offsets_count ?
			log( "found %i/%i", found_offsets_count, max_offsets_count ) :
			on_error( ).log( "found %i/%i", found_offsets_count, max_offsets_count ) );

		pop_directory( );
	}

	pop_directory( );
	
	return this->m_state == n_sdk::state_t::error ? 0 : 1;
}
