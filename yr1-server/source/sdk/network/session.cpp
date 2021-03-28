#include <common.h>

#include "connection.h"
#include "session.h"


#include "../../core/procedures/receiver.h"
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
	// spawn async_reader
	m_socket.async_read_some( asio::buffer( m_recv_buffer ),
		[ & ]( const asio::error_code& error, std::size_t bytes_transferred ) {
			if ( error )
			{
				on_error( error );
				return;
			}

			on_read( bytes_transferred );
		} );
	
	// check if we have anything to send
	// and spawn async_sender to do it
	// todo: ^ sender

	// other stuff ...
	// like: health_check or status stuff etc 
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
	std::deque< u8 > bytes( m_recv_buffer, m_recv_buffer + size );
	//std::cout << "recv: " << size << " bytes on " << m_sid << "> ";
	////<< bytes. << std::endl;

	//for ( auto& byte : bytes )
	//	printf( "%#hhx ", byte );
	//std::cout << std::endl;

	// handle recv ...
	std::thread( &core::c_receiver::handler, &core::c_receiver::get( ), bytes ).detach( );

	breath( );
}

void sdk::c_session::on_error( const asio::error_code& error )
{
	std::cerr << "error: " << error.message( ) << std::endl;

	disconnect( );
}
