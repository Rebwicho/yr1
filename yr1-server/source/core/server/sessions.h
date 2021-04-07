#ifndef SESSIONS_H
#define SESSIONS_H

#include "session.h"

namespace core
{
	// template< typename type = sdk::c_session >
	class c_sessions : public c_singleton< core::c_sessions >
	{
	public:
		// needs more stuff xd
		//template< typename... args_t >
		//sdk::c_session& emplace_back( args_t&&... args );

		sdk::c_session& add( tcp::socket peer );

		void remove( const u32 sid );

		const u32 size( );

		std::deque< sdk::c_session > m_collection = { };
	private:
		std::mutex m_mutex;
	};
}

#endif // SESSIONS_H
