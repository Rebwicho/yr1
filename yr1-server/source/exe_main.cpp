#include <common.h>

using asio::ip::tcp;

// asio::io_context io_context;

const int max_length = 512;
const unsigned __int16 main_port = 0xdead;
static __int32 m_static_id = 0;

class c_connection;
class c_connection
{
private: tcp::socket m_socket;
public:
    c_connection( tcp::socket socket )
        : m_socket( std::move( socket ) ), m_connection_id( m_static_id++ )
    {

    }

public:
    tcp::socket& socket( )
    {
        return m_socket;
    }
    __int32& connection_id( )
    {
        return m_connection_id;
    }

public:
    void on_connect( )
    {
        std::cout << "log: new connection with id: " << m_connection_id << std::endl;

        std::string on_connect_message = "welcome";

        a_write( on_connect_message );

        a_read( );
    }

    void a_write( const std::string message )
    {
        m_write_buffer = message;

        std::cout << "log: sending " << m_write_buffer.size( ) << " bytes to " << m_connection_id << std::endl;
        asio::async_write( m_socket, asio::buffer( m_write_buffer.data( ), m_write_buffer.size( ) ),
            [ & ]( const asio::error_code& error_lamb, std::size_t bytes_transferred_lamb ) {
                if ( error_lamb )
                {
                    on_error( error_lamb );
                    return;
                }

                std::cout << "log: sent " << bytes_transferred_lamb << " bytes to " << m_connection_id << std::endl;
            } );
    }
    void a_read( )
    {
        m_socket.async_read_some( asio::buffer( m_recv_buffer ),
            [ & ]( const asio::error_code& error_lamb, std::size_t bytes_transferred_lamb ) {
                if ( error_lamb )
                {
                    on_error( error_lamb );
                    return;
                }

                std::string client_data( m_recv_buffer, bytes_transferred_lamb );
                std::cout << "log: received " << bytes_transferred_lamb << " bytes from " << m_connection_id << std::endl;
                std::cout << "log: data: " << client_data << " from " << m_connection_id << std::endl;

                // handle packet/data here
                if ( client_data == "exit" )
                {
                    disconnect( );
                    return;
                }

                a_read( );
            } );
    }

    void disconnect( )
    {
        a_write( "disconnect" );

        std::cout << "log: disconnecting " << m_connection_id << std::endl;
        m_socket.close( );
        m_ferror_callback( m_connection_id );
    }

    void set_error_callback( std::function< void( __int32 ) > ferror_callback )
    {
        m_ferror_callback = ferror_callback;
    }

private:
    void on_write( const asio::error_code& error, std::size_t bytes_transferred )
    {
        if ( error )
        {
            std::cerr << "error: " << error.message( ) << std::endl;
            return;
        }

        std::cout << "log: sent " << bytes_transferred << " bytes to " << m_connection_id << std::endl;
    }
    void on_read( const asio::error_code& error, std::size_t bytes_transferred )
    {
        if ( error )
        {
            std::cerr << "error: " << error.message( ) << std::endl;
            return;
        }

        std::cout << "log: received " << bytes_transferred << " bytes from " << m_connection_id << std::endl;
        std::cout << "log: recv_buffer: " << std::string( m_recv_buffer, bytes_transferred ) << " from " << m_connection_id << std::endl;
    }

    void on_error( const asio::error_code& error )
    {
        std::cerr << "error[ " << m_connection_id << " ]: " << error.message( ) << std::endl;
        m_socket.close( );
        m_ferror_callback( m_connection_id );
    }

private:

    __int32 m_connection_id = -1;

    char m_recv_buffer[ 512 ];
    char m_send_buffer[ 512 ];
    std::string m_write_buffer;

    std::function< void( __int32 ) > m_ferror_callback;
};

class c_tcp_server : public c_singleton<c_tcp_server>
{
public:
    c_tcp_server( )
        : m_acceptor( m_io_context,
            tcp::endpoint( tcp::v4( ), main_port ) )
    {
        wait_accept( );
    }

    void run( )
    {
        m_io_context.run( );
    }

    void disconnect( __int32 connection_id )
    {
        for ( auto it = m_connections.begin( ); it != m_connections.end( ); )
            if ( it->connection_id( ) == connection_id )
            {
                m_connections.erase( it );
                break;
            }

        std::cout << "log: connection count is " << m_connections.size( ) << std::endl;
    }

private:
    void wait_accept( )
    {
        m_acceptor.async_accept(
            [ & ]( const asio::error_code& error_lamb, asio::ip::tcp::socket peer ) {
                if ( error_lamb )
                {
                    std::cerr << "accept error: " << error_lamb.message( ) << "\n";
                    wait_accept( );
                    return;
                }

                auto& new_connection = m_connections.emplace_back( std::move( peer ) );
                new_connection.set_error_callback( [ this ]( auto&& a1 ) {
                    disconnect( std::forward< decltype( a1 ) >( a1 ) );
                    } ); new_connection.on_connect( );

                    std::cout << "log: new connection, connected " << m_connections.size( ) << std::endl;
                    wait_accept( );
            } );
    }

    void on_accept( const asio::error_code& error )
    {
        if ( error )
            std::cerr << "accept error: " << error.message( ) << "\n";

        wait_accept( );
    }

private:
    asio::io_context m_io_context;
    tcp::acceptor m_acceptor;

    std::deque< c_connection > m_connections = { };

};

int main( int argc, char* argv[ ] )
{
    try
    {
        c_tcp_server::get( ).run( );
    }
    catch ( std::exception& e )
    {
        std::cerr << "Exception: " << e.what( ) << "\n";
    }

    return 0;
}