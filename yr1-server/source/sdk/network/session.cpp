#include <common.h>
#include <sdk.h>

#include "session.h"

#include "../../core/server/sessions.h"

void sdk::c_session::start( )
{
	std::cout << "log: new session " <<
		m_socket.remote_endpoint( ).address( ).to_string( ) << ":" <<
		m_socket.remote_endpoint( ).port( ) << " <-remote / local-> " <<
		m_socket.local_endpoint( ).address( ).to_string( ) << ":" <<
		m_socket.local_endpoint( ).port( ) << std::endl;

	breath( );
}

void sdk::c_session::stop( )
{
	std::cout << "log: " << m_sid << "> disconnecting" << std::endl;
	
	m_socket.close( );
	core::c_sessions::get( ).remove( m_sid );
}

void sdk::c_session::on_read( u32 size )
{
	std::vector< u8 > bytes( m_recv_buffer, m_recv_buffer + size );
	//std::cout << "recv: " << size << " bytes on " << m_sid << "> ";
	////<< bytes. << std::endl;

	//for ( auto& byte : bytes )
	//	printf( "%#hhx ", byte );
	//std::cout << std::endl;

	// handle recv ...
	//std::thread( &core::c_receiver::handler, &core::c_receiver::get( ), bytes ).detach( );
	handle_packet( bytes );

	breath( );
}

void sdk::c_session::on_error( const asio::error_code& error )
{
	std::cerr << "error: " << error.message( ) << std::endl;

	stop( );
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

void asdasdsa( u32 addr, u32 size )
{
	for ( u32 c = addr; c < addr + size; c++ )
	{
		u8 byte = *( u8* )c;

		printf( "%#hhx ", byte );
	}
	std::cout << std::endl;
}

void sdk::c_session::handle_packet( std::vector<u8>& recived_bytes )
{
	std::cout << "reciever: got " << recived_bytes.size( ) << " bytes" << std::endl;

	//for ( auto& byte : recived_bytes )
	//	printf( "%#hhx ", byte );
	//std::cout << std::endl;

	// deduce type of packet
	auto packet_type = recived_bytes.front(  );

	//printf( "reciever: packet type is %#hhx\n", packet_type );

	switch ( ( enumer::packet_type_t )packet_type )
	{
		// on each case we will have something like on_packet_name( packet );
		// that will handle what operations it needs to do with it

		case sdk::enums::e_packet_type::login:
		{
			auto packet = sdk::network::c_packet::convert_bytes< packet::login >( recived_bytes );
			
			packet::login_response login_result;

			std::string login( packet.login_buffer );
			std::string password( packet.password_buffer );

			std::cout << "log: " << m_sid << "> login: " << login << std::endl;
			std::cout << "log: " << m_sid << "> password: " << password << std::endl;
			
			// check if login and password matches that of db one
			if ( login == "rebo" && password == "pass" )
			{
				login_result.result = 1;
				std::cout << "log: " << m_sid << "> login success" << std::endl;

			}
			else
			{
				login_result.result = 0;
				std::cout << "log: " << m_sid << "> login failed" << std::endl;
			}

			memcpy( &m_send_buffer, &login_result, sizeof( login_result ) );

			std::cout << "log: sending " << sizeof( login_result ) << "bytes to " << m_sid << std::endl;
			
			m_socket.async_write_some( asio::buffer( m_send_buffer, sizeof( login_result ) ),
				[ & ]( const asio::error_code& error, std::size_t bytes_transferred ) {
					if ( error )
					{
						on_error( error );
						return;
					}

					//on_read( bytes_transferred );
					std::cout << "log:" << m_sid << "> sent response to login request" << std::endl;
				} );

		} break;
		case sdk::enums::e_packet_type::login_response:
		{

		} break;

		default: break;
	}
}