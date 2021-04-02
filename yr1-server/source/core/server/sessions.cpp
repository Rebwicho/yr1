#include <common.h>
#include <sdk.h>

#include "sessions.h"

//template <class... _Valty>
//decltype( auto ) emplace_back( _Valty&&... _Val )
//{
//	_Orphan_all( );
//	_Emplace_back_internal( _STD forward<_Valty>( _Val )... );

//template < typename... args_t >
//sdk::c_session& core::c_sessions::emplace_back( args_t&&... args )
//{
//	std::unique_lock< std::mutex > lock { m_mutex };
//	
//	return m_collection.emplace_back( std::forward< args_t >( args )... );
//}
//
//template < class ... _Valty >
//sdk::c_session& core::c_sessions::emplace_test( _Valty&&... _Val )
//{
//	return m_collection.emplace_back( std::forward< _Valty >( _Val )... );
//}
//
sdk::c_session& core::c_sessions::add( tcp::socket peer )
{
	std::unique_lock< std::mutex > lock { m_mutex };

	return m_collection.emplace_back( std::move( peer ) );
}

void core::c_sessions::remove( const u32 sid )
{
	std::unique_lock< std::mutex > lock { m_mutex };
	for ( auto it = m_collection.begin( ); it != m_collection.end( ); )
		if ( it->m_sid == sid )
		{
			m_collection.erase( it );
			break;
		}
}

const u32 core::c_sessions::size( )
{
	std::unique_lock< std::mutex > lock { m_mutex };
	return m_collection.size( );
}
