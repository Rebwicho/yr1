#ifndef SESSION_H
#define SESSION_H

using asio::ip::tcp;

namespace n_core
{
    //asio::io_context io_context;

    //tcp::socket s( io_context );
    //tcp::resolver resolver( io_context );
    //asio::connect( s, resolver.resolve( "localhost", std::to_string( main_port ) ) );

	class c_session : public c_singleton< c_session >
	{
	public:
		c_session( )
			: m_socket( m_io_context ) { }
		
	public:
		void start( u32 port );

	protected:
		void connect_start( u32 port );

	private:
		asio::io_context m_io_context;
		tcp::socket m_socket;

	};
}

#endif // SESSION_H
