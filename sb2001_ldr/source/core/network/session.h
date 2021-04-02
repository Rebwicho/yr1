#ifndef SESSION_H
#define SESSION_H

using asio::ip::tcp;

namespace n_core
{

	class c_session : public c_singleton< c_session >
	{
	public:
		c_session( )
			: m_socket( m_io_context ) { }
		
	public:
		void send( std::vector< u8 >& packet );
		
		// todo: upgrade it to take lambda as param and spawn thread itself
		std::vector< u8 > sync_send( std::vector< u8 >& packet ); // sync_send sends packet to server
		// and waits for response then returns data from server
		// example usage: on button click create new detached thread with that function
		
		void start( u32 port );
	
	protected:
		void connect_start( u32 port );
	
	private:
		void on_resolve( tcp::resolver::results_type result ); // we call this when resolving success
		//void on_connect( const tcp::endpoint& endpoint ); // we call this when connect success
		void on_connect( ); // we call this when connect success

		void on_send( std::size_t bytes_transferred );
		void on_recive( std::size_t bytes_transferred );
		
		void on_error( const asio::error_code& error ); // handle the errors?

		void resolve( ); // resolve
		void connect( tcp::resolver::results_type result ); // connect to server
		
		void async_send( std::vector< u8 > packet );
		void async_recive( );
		
		void sender( );
		void receiver( );
		
		void run_context( );

	public:
		u32 m_login_result = 0;
	
	private:
		asio::io_context m_io_context;
		tcp::socket m_socket;

		std::array< u8, 512 > m_recive_buffer;
		std::deque< std::vector< u8 > > m_send_queue = { };
	};
}


#endif // SESSION_H
