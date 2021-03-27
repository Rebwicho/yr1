#ifndef OFFSETS_H
#define OFFSETS_H

#include <common.h>
#include <sdk.h>

// todo: think of a system for this or move somewhere else
class offset : public c_singleton< offset >
{ public:
	u32 f_anti_debug = 0;
	u32 f_recv = 0;
	u32 f_send = 0;	
};

namespace n_core
{
	class c_offsets : public n_sdk::c_basic_log, public c_singleton< c_offsets >
	{
	public:
		auto get_state( )->n_sdk::state_t;

	public:
		bool waiter( );

		bool sanity_check( );
		bool worker( );
		
		bool on_waiter( );
		bool on_start( );
		bool on_end( );

		void start( );
		void end( );

	private:
		n_sdk::c_pattern_module& register_module( const std::string& module_name );
		
		void register_patterns( );
		bool walk_modules( );

		bool on_pattern_end( const u32 value, u32& container, const std::string& offset_name = "null" );

	private:
		n_sdk::state_t m_state = n_sdk::state_t::started;
		
		std::deque< n_sdk::c_pattern_module > m_pattern_modules = { };
	};
}

#endif // OFFSETS_H
