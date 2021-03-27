#include <common.h>
#include <sdk.h>

#include "sessions.h"
#include "server.h"

void core::c_server::start( )
{
	std::cout << "log: server starting" << std::endl;
	
	start_accepting( );
	
	std::thread( &asio::io_context::run, &m_io_context ).detach( );
	//m_io_context.run( );
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
	auto& new_session = core::c_sessions::get( ).add( std::move( peer ) );
	new_session.on_connect( );

	std::cout << "log: accepted new connection, current session count: " << core::c_sessions::get( ).size( ) << std::endl;

	wait_accept( );
}

void core::c_server::on_error( const asio::error_code& error )
{
	std::cerr << "error: " << error.message( ) << std::endl;
	wait_accept( );
}
