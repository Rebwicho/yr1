#ifndef SESSION_H
#define SESSION_H

using asio::ip::tcp;

namespace sdk
{
	class c_session // : public sdk::c_connection
	{
		inline static u32 sessions_id = 0;
	public:
		//c_session( ) = delete;
		c_session( tcp::socket socket )
			: m_socket( std::move( socket ) ), m_sid( sessions_id ++ )
		{
			//start_socket( std::move( socket ) );
		}
	
	public:
		//void start_connection( asio::ip::tcp )
		//void connect( );
		void disconnect( );
		
		void breath( );

		void on_connect( );
	protected:
		void on_read( u32 size );
		void on_error( const asio::error_code& error );

	public:
		u32 m_sid = 0;

	private:
		//sdk::c_connection m_connection;
		tcp::socket m_socket;
		char m_recv_buffer[ 512 ];
	};
}

#endif // SESSION_H
