#include <common.h>

#include "connection.h"
#include "session.h"

#include "../../core/server/sessions.h"

// std::deque< sdk::c_session > sessions = { };

void sdk::c_session::disconnect( )
{
	// find 

	
	std::cout << "log: " << m_sid << "> disconnecting" << std::endl;
	m_socket.close( );
	core::c_sessions::get( ).remove( m_sid );

}

void sdk::c_session::breath( )
{
	m_socket.async_read_some( asio::buffer( m_recv_buffer ),
		[ & ]( const asio::error_code& error, std::size_t bytes_transferred ) {
			if ( error )
			{
				on_error( error );
				return;
			}

			on_read( bytes_transferred );
		} );
}

void sdk::c_session::on_connect( )
{
	std::cout << "log: new session " <<
		m_socket.remote_endpoint( ).address( ).to_string( ) << ":" <<
		m_socket.remote_endpoint( ).port( ) << " <-remote / local-> " <<
		m_socket.local_endpoint( ).address( ).to_string( ) << ":" <<
		m_socket.local_endpoint( ).port( ) << std::endl;
	
	breath( );
}

void sdk::c_session::on_read( u32 size )
{
	std::cout << "recv: on " << m_sid << "> " << std::string( m_recv_buffer, size ) << std::endl;

	// handle recv ...
	
	breath( );
}

void sdk::c_session::on_error( const asio::error_code& error )
{
	std::cerr << "error: " << error.message( ) << std::endl;

	disconnect( );
}
