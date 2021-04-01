#include <common.h>
#include <sdk.h>

#include "session.h"

void n_core::c_session::start( u32 port )
{
	connect_start( port );
}

void n_core::c_session::connect_start( u32 port )
{
	tcp::resolver resolver( m_io_context );

	auto endpoint =
		*resolver.resolve( "localhost", std::to_string( port ) ).begin( );

	// waiting for connection to server
	m_socket.async_connect( endpoint, [ & ]( const asio::error_code& error ) {
		if ( error )
		{
			std::cerr << "error: cant connect to endpoint" << std::endl;
			return;
		}

		std::cout << "log: connected to endpoint" << std::endl;
		
	} );

	
}
