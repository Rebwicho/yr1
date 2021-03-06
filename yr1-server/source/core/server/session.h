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
		void start( );
		void stop( );

	protected: /* self */
		void on_read( u32 size );
		void on_error( const asio::error_code& error );

		void breath( );

	protected: /* auth */
		bool m_is_logged = 0;
	
	protected: /* receiver */		
		void handle_packet( std::vector<u8>& recived_bytes );
	
	protected: /* sender */

	
	public:
		u32 m_sid = 0;

	private:
		u8 m_recv_buffer[ 512 ];
		u8 m_send_buffer[ 100020 ];

	private:
		tcp::socket m_socket;
	};
}

#endif // SESSION_H
