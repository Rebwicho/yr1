#include <common.h>
#include <sdk.h>

#include "session.h"

#include "sessions.h"

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

void sdk::c_session::handle_packet( std::vector<u8>& recived_bytes )
{
	std::cout << "reciever: got " << recived_bytes.size( ) << " bytes" << std::endl;

	if ( m_is_logged == 0 && ( enumer::packet_type_t )recived_bytes.front( ) != enumer::packet_type_t::login )
	{
		// client is not logged and tired to do something else than login seems like someone is simulating packets
		// for now just return
		// later on we will want to flag account
		// todo:
		return;
	}
	
	switch ( ( enumer::packet_type_t )recived_bytes.front( ) )
	{
		// on each case we will have something like on_packet_##name( packet );
		// that will handle what operations it needs to do with it

		case sdk::n_enum::e_packet_type::login:
		{
			auto packet = packet::convert::from_bytes< packet::login >( recived_bytes );
			
			packet::login_response login_response;

			std::string login( packet.login_buffer );
			std::string password( packet.password_buffer );

			std::cout << "log: " << m_sid << "> login: " << login << std::endl;
			std::cout << "log: " << m_sid << "> password: " << password << std::endl;
			
			// check if login and password matches that of db one
			if ( login == "rebo" && password == "pass" )
			{
				login_response.result = 1;
				std::cout << "log: " << m_sid << "> login success" << std::endl;
				m_is_logged = 1;
			}
			else
			{
				login_response.result = 0;
				std::cout << "log: " << m_sid << "> login failed" << std::endl;
			}

			memcpy( &m_send_buffer, &login_response, sizeof( login_response ) );
			//auto result_as_bytes = packet::convert::to_bytes( &login_response, sizeof( packet::login_response ) );
			// ^ cant use this because this var gets destructed before async_write writes data :<

			std::cout << "log: sending " << sizeof( login_response ) << "bytes to " << m_sid << std::endl;
			
			m_socket.async_write_some( asio::buffer( m_send_buffer ),
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
		case sdk::n_enum::e_packet_type::game_list:
		{
			auto packet = packet::convert::from_bytes< packet::game_list >( recived_bytes );

			// get list of subscriptions for that client from db
			//std::vector< std::string > avail_client_games = { "Nostale", "NosWings", "test1", "test2" };
			std::vector< u32 > avail_client_games = { 1, 2, 3, 4 };
			// simulating^ remember to change when db is ready todo: <- this
			
			auto game_list_response = packet::game_list_response( );

			for ( int it = 0; it < avail_client_games.size(  ); ++it )
				game_list_response.games_list[ it ] = static_cast< enums::game_type >( avail_client_games[ it ] );
	
			memcpy( &m_send_buffer, &game_list_response, sizeof( game_list_response ) );

			std::cout << "log: sending " << sizeof( game_list_response ) << "bytes to " << m_sid << std::endl;

			m_socket.async_write_some( asio::buffer( m_send_buffer ),
				[ & ]( const asio::error_code& error, std::size_t bytes_transferred ) {
					if ( error )
					{
						on_error( error );
						return;
					}

					//on_read( bytes_transferred );
					std::cout << "log:" << m_sid << "> sent response to game_list request" << std::endl;
				} );
		} break;
		case n_enum::e_packet_type::cheat_load: 
		{
			// todo: check
			// if client requested cheat that he have avail for him
			
			auto packet = packet::convert::from_bytes< packet::cheat_load >( recived_bytes );

			std::vector< char > game_cheat_file = { };
			
			// deduce what cheat client have requested
			// later on we can have a class that takes care of loading bin to byte array and returns that array for us
			// so we can just send it
			switch ( packet.game_cheat )
			{
				case enums::game_type::Nostale:
				{
					// load nostale dll
					// D:\\yr1\\game_cheat\\sb2001_nt_x86_Release.dll
					std::ifstream file_stream( "D:\\yr1\\game_cheat\\sb2001_nt_x86_Release.dll", std::ios::binary );
					game_cheat_file = std::vector< char >(
						std::istreambuf_iterator<char>( file_stream ),
						std::istreambuf_iterator<char>( ) );

					file_stream.close( );
				} break;

				case enums::game_type::Noswings:
				{

				} break;
				
				default: 
				{
					std::cout << "log:" << m_sid << "> recived unknown game type request" << std::endl;
				} break;
			} 

			std::cout << "log:" << m_sid << "> prepared file of size " << game_cheat_file.size( ) << std::endl;

			
			
		} break;
		
		default: 
		{
			std::cout << "log:" << m_sid << "> recived unknown packet type" << std::endl;
		} break;
	}
}