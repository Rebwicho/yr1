#include "hooks.h"

#include "offsets.h"

static abyss test_original = nullptr;

void f_test_target( )
{
	int x = 5;
	x = 10;
	int y = x;
	y++;
	printf( "original called\n" );
	return;
}

void f_test_hook( )
{
	( ( void ( * )( void ) ) test_original )( );
	( ( void( * )( ) ) test_original ) ( );
	return;
}

auto n_core::c_hooks::get_state( ) -> n_sdk::state_t
{
	return this->m_state;
}

void n_core::c_hooks::waiter( )
{
	//set_directory( "/" ).
	//log( "waiting for bones" ).
	log( "waiting for offsets" );
	//while ( ( n_core::c_offsets::get( ).get_state( ) == n_sdk::state_t::started )
	//	|| ( n_core::c_offsets::get( ).get_state( ) == n_sdk::state_t::waiting ) )
	//	std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );

	while ( 0xffdead )
	{
		if ( n_core::c_offsets::get( ).get_state( ) == n_sdk::state_t::exit
			|| n_core::c_offsets::get( ).get_state( ) == n_sdk::state_t::success 
			|| n_core::c_offsets::get( ).get_state( ) == n_sdk::state_t::idle ) break;
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}
}

bool n_core::c_hooks::sanity_check( )
{
	return ( n_core::c_offsets::get( ).get_state( ) == n_sdk::state_t::exit ) ? 0 : 1;
}

bool n_core::c_hooks::minhook_start( )
{
	return ( n_sdk::c_hooking::minhook_start( ) == 0 ? 0 : 1 );
}

bool n_core::c_hooks::worker( const n_sdk::hook_action_t action )
{
	// pin: idea#log-extended/on_event
	// note: not sure tho if should pin this we can just do simple log here
	// logs like down below should be called with only log( something )
	// also states should never be set manually or at least reduced to minimum 
	// note-to-self: get better at directory log management?
	//set_state( n_sdk::state_t::waiting ).
	//set_directory( "/" ).
	on_work( ).log( action == n_sdk::c_hooking::e_hook_action::disable ? "disabling" : "enabling" );

	action == n_sdk::c_hooking::e_hook_action::disable ?
		walk_hooks( n_sdk::hook_action_t::disable ) == 0 ? // we try to disable funcs here
		on_error( ).log( "failed to disable some hooks" ) : // <- something failed
		on_success( ) :

		walk_hooks( n_sdk::hook_action_t::enable ) == 0 ? // we try to hook funcs here
		on_error( ).log( "enabling failed" ) : // <- something failed
		on_success( );
	
	return this->m_state == n_sdk::state_t::error ? 0 : 1;
}

bool n_core::c_hooks::on_start( )
{
	create_hooks( );
	return bind_state( &this->m_state ).set_owner( "hooks" ).on_start( ).as_true( );
}

void n_core::c_hooks::start( )
{
	on_start( );
	waiter( );
	
	if ( minhook_start( ) == 0 ) 
	{
		on_failed( ).log( "minhook_start failed" ).on_exit( );
		return;
	}
	if ( sanity_check( ) == 0 ) 
	{
		on_failed( ).log( "sanity_check failed" ).on_exit( );
		return;
	}
	if ( worker( n_sdk::hook_action_t::enable ) == 0 ) return;
}

void n_core::c_hooks::end( )
{
	worker( n_sdk::hook_action_t::disable );

	n_sdk::c_hooking::minhook_end( );
}

void n_core::c_hooks::create_hooks( )
{
	make_hook( "f_test_target", ( u32 )f_test_target, &f_test_hook, &test_original );
}

bool n_core::c_hooks::walk_hooks( const n_sdk::hook_action_t action )
{
	push_directory( "function" );
	for ( auto& hook : this->m_hooks )
	{
		if ( action == n_sdk::hook_action_t::disable )
			if ( hook.m_enabled == 0 ) continue;
		
		push_directory( hook.m_hook_name );

		n_sdk::c_hooking::do_action( hook, action ) == 0 ? on_error( ).
			log( "%#x -> %#x { %#x } &failed",
				hook.m_hook_name.c_str( ), ( u32 )hook.m_location,
				( u32 )hook.m_hook, ( u32 )hook.m_original ) :

			log( action == n_sdk::hook_action_t::enable ?
				"%#x -> %#x { %#x } &enabled" : "%#x -> %#x { %#x } &disabled",
				hook.m_hook_name.c_str( ), ( u32 )hook.m_location,
				( u32 )hook.m_hook, ( u32 )hook.m_original );

		pop_directory( );
	}

	pop_directory( );

	return this->m_state == n_sdk::state_t::error ? 0 : 1;
}

void n_core::c_hooks::make_hook( const std::string& hook_name, u32 location, abyss hook, abyss original )
{
	this->m_hooks.push_back( n_sdk::c_hooking::make_hook( hook_name, location, hook, original ) );
}
