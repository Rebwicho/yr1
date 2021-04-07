#ifndef SERVER_H
#define SERVER_H

using asio::ip::tcp;

namespace core
{
	class c_server : public c_singleton< core::c_server >
	{
	public:
		c_server( )
			: m_acceptor( m_io_context,
				tcp::endpoint( tcp::v4( ), 0xdead ) ) { }
	public:
		void start( );

	protected:
		void wait_accept( );

	private:
		void start_accepting( );

		void on_accept( tcp::socket peer );
		void on_error( const asio::error_code& error );

		void run_context( );

	private:
		asio::io_context m_io_context;
		tcp::acceptor m_acceptor;
	};
}

#endif // SERVER_H
