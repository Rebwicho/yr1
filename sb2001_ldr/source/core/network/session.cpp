#include <common.h>
#include <sdk.h>

#include "session.h"

void n_core::c_session::send( std::vector<u8>& packet )
{
	m_send_queue.push_back( packet );
}
std::vector<u8> n_core::c_session::sync_send( std::vector<u8>& packet )
{
	u8 sync_recv_buffer[ 100020 ]; // optimize xd
	asio::error_code error;

	auto sent_size =  m_socket.write_some( asio::buffer( packet ), error );
	if ( error )
	{
		on_error( error ); return std::vector<u8>( );
	}

	auto recived_size = m_socket.read_some( asio::buffer( sync_recv_buffer, 512 ), error );
	if ( error )
	{
		on_error( error ); return std::vector<u8>( );
	}
	
	return packet::convert::to_bytes( &sync_recv_buffer, recived_size );
}

void n_core::c_session::start( u32 port )
{
	connect_start( port );
}
void n_core::c_session::end( )
{
	m_socket.close( );
	m_io_context.stop( );
}

void n_core::c_session::connect_start( u32 port )
{
	// start connecting to server
	resolve( );
}

void n_core::c_session::on_resolve( tcp::resolver::results_type result )
{
	std::cout << "log: resolved" << std::endl;

	// call connect to connect to server
	connect( result );
}
void n_core::c_session::on_connect( )
{
	std::cout << "log: connected, " <<
		m_socket.remote_endpoint( ).address( ).to_string( ) << ":" <<
		m_socket.remote_endpoint( ).port( ) << " <-remote / local-> " <<
		m_socket.local_endpoint( ).address( ).to_string( ) << ":" <<
		m_socket.local_endpoint( ).port( ) << std::endl;

	std::thread( &n_core::c_session::run_context, this ).detach( );

	m_started = 1;
	
	//std::thread( &n_core::c_session::receiver, this ).detach( );
	//std::thread( &n_core::c_session::sender, this ).detach( );
}
void n_core::c_session::on_send( std::size_t bytes_transferred )
{
	std::cout << "log: we sent " << bytes_transferred << "bytes to server" << std::endl;
}
void n_core::c_session::on_recive( std::size_t bytes_transferred )
{
	std::cout << "log: we recived " << bytes_transferred << "bytes from server" << std::endl;

	// save buffer so we can know what was there
	// because we will start async_recive here again
	std::thread( &n_core::c_session::receiver, this ).detach( );

	// handle packet recognition etc...
}

void n_core::c_session::on_error( const asio::error_code& error )
{
	// todo: 
	// filter error codes and handle as it should be
	// and take action
	
	std::cerr << "error: " << error.message( ) << std::endl;
}

void n_core::c_session::resolve( )
{
	std::cout << "log: trying to resolve" << std::endl;
	
	tcp::resolver resolver( m_io_context );
	asio::error_code error;
	
	auto resolved = resolver.resolve( "localhost", std::to_string( 0xdead ), error );

	if ( error )
	{
		on_error( error ); return;
	}
	
	on_resolve( resolved );
}
void n_core::c_session::connect( tcp::resolver::results_type result )
{
	std::cout << "log: trying to connect to server" << std::endl;

	asio::error_code error;
	asio::connect( m_socket, result, error );
	if ( error )
	{
		on_error( error ); return;
	}
	
	on_connect( );
}
void n_core::c_session::async_send( std::vector<u8> packet )
{
	m_socket.async_send( asio::buffer( packet ),
		[ & ]( const asio::error_code& error, std::size_t bytes_transferred ) {
			if ( error )
			{
				on_error( error ); return;
			}

			on_send( bytes_transferred );
		} );

	std::thread( &n_core::c_session::sender, this ).detach( );
}
void n_core::c_session::async_recive( )
{
	m_socket.async_read_some( asio::buffer( m_recive_buffer ),
		[ & ]( const asio::error_code& error, std::size_t bytes_transferred ) {
			if ( error )
			{
				on_error( error ); return;
			}

			on_recive( bytes_transferred );
		} );
}

void n_core::c_session::sender( )
// this function is called as detached thread do wait for packets to be send
// when there is one it saves that packet and pops from packets to send queue
// then executes async send with that saved packet as data to be send
// on send success we call this func as detached thread again ...
{
	while ( m_send_queue.size( ) == 0 )
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );

	auto packet = m_send_queue.front( );
	m_send_queue.pop_front( );

	async_send( packet );
}
void n_core::c_session::receiver( )
// similar to sender
{
	async_recive( );
}

void n_core::c_session::run_context( )
{
	std::cout << "log: starting context" << std::endl;
	do
	{
		try
		{
			m_io_context.run( );
			break; // <-- context exited normally 
		}
		catch ( std::exception& error )
		{
			std::cerr << "context error: " << error.what( ) << std::endl;
		}
	} while ( 0xffdead );
}

