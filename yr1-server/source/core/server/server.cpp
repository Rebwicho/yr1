#include <common.h>
#include <sdk.h>

#include "sessions.h"
#include "server.h"

void core::c_server::start( )
{
	std::cout << "log: server starting" << std::endl;
	
	std::thread( &core::c_server::run_context, this ).detach( );
	
	start_accepting( );	
}

void core::c_server::wait_accept( )
{
	m_acceptor.async_accept(
		[ & ]( const asio::error_code& error, asio::ip::tcp::socket peer )
		{
			if ( error )
			{
				on_error( error );
				return;
			}

			on_accept( std::move( peer ) );
		} );
}

void core::c_server::start_accepting( )
{
	std::cout << "log: waiting for connections" << std::endl;
	
	wait_accept( );
}

void core::c_server::on_accept( tcp::socket peer )
{
	//auto& new_session = 
	// new_session.on_connect( );
	core::c_sessions::get( ).add( std::move( peer ) ).on_connect( );

	std::cout << "log: accepted new connection, current session count: " << core::c_sessions::get( ).size( ) << std::endl;

	wait_accept( );
}

void core::c_server::on_error( const asio::error_code& error )
{
	std::cerr << "error: " << error.message( ) << std::endl;
	
	wait_accept( );
}

void core::c_server::run_context( )
{
	std::cout << "log: starting context" << std::endl;
	do {
		try {
			m_io_context.run( ); /* | */ break; // <-- context exited normally 
		} catch ( std::exception& error ) {
			std::cerr << "context error: " << error.what( ) << std::endl;
		}
	} while ( 0xffdead );
}
