#ifndef HOOKS_H
#define HOOKS_H

#include <common.h>
#include <sdk.h>

namespace n_core
{
	class c_hooks : public n_sdk::c_basic_log, public c_singleton< c_hooks >
	{
	public:
		auto get_state( )->n_sdk::state_t;

	public:
		void waiter( );

		bool sanity_check( );
		bool minhook_start( );
		bool worker( const n_sdk::hook_action_t action );
		
		bool on_start( );
		bool on_end( );

		void start( );
		void end( );

	private:
		void create_hooks( );
		void make_hook( const std::string& hook_name, u32 location, 
			abyss hook, abyss original );
		
		bool walk_hooks( const n_sdk::hook_action_t action );

	private:
		n_sdk::state_t m_state = n_sdk::state_t::started;
		std::deque< n_sdk::hook_t > m_hooks = { };
	};
}

#endif // HOOKS_H
