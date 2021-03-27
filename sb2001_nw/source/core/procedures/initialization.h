#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include <common.h>
#include <sdk.h>

/* initialization procedure
	- this procedure takes care of core initialization
		of this dll:
			- console
			- offsets
			- hooks
			- anti cheat bypasses ( if any )
		    ( not added yet )
			- config		( ? )
			- gui			( ? )
			- networking	( ? )
			- input			( ? )
	- this procedure handles dll attach event
	- this procedure spawns needed threads
	- this procedure starts routines

	contains:
		- main thread
 */

namespace n_core
{
	class c_initialization : public c_singleton< c_initialization >
	{
	public:
		auto set_handle( u32 handle ) -> void;
		auto get_state( ) -> n_sdk::state_t;
	
	private:
		bool start( );
		bool end( );

	public:
		bool main_thread( );

	private:
		n_sdk::state_t m_state = n_sdk::state_t::started;
		u32 m_handle = 0;
	};
}

#endif // INITIALIZATION_H
