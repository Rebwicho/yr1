#ifndef CONNECTION_H
#define CONNECTION_H

using asio::ip::tcp;

namespace sdk
{
	class c_connection
	{
	public:
		//c_connection( ) = default;
		c_connection( tcp::socket socket )
			: m_socket( std::move( socket ) ) { }

	public:
		void start_socket( tcp::socket socket );

	public:
		tcp::socket m_socket;
	};
}

#endif // CONNECTION_H
