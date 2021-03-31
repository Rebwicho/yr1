#include <common.h>

#include "core/server/server.h"

//using asio::ip::tcp;

int main( int argc, char* argv[ ] )
{
    core::c_server::get( ).start( );

    while ( 0xffdead )
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    return 0;
}

/*
    server@localhost:0xffdead
     +--- i want to have alive connections in some queue/list
     |     +--- so later i can have live monitor like [ connection_ip@status: dropping in 13.37s ]
     |                                                                 +
     |                                                                 +- where connection is currently at ex: sb2001_nw:connected
     +--- accept incoming connections -> add to connections queue/list
     +--- check deadline -> listen to connections
     |     |                 +--- handle connection message and send result data to connection -> ...
     |     +-----------------------+
     |                             +--- remove dropped connections from list
     |                   
     +--- custom interactions with connection ( we push to connection action stack and then executor does rest? )
     |    like: disconnect, send custom message, ...
     |
     |
     +...

     connection
        - connection( socket ) : m_socket( socket )
        - close( ) - closes connection

     session : public connection, public : executor
        //- session( socket ) : m_socket( socket )
        - status ( started, connected ) 
        - create_connection( socket )
        //- set_deadline( deadline_length )
        - executor( )
            - if deadline -> disconnect ( remove self from queue )
            - custom_stack -> execute ( calls executor( ) at the end of its work )
            - handle requests ( calls executor( ) at the end of its work )
            - any error -> disconnect ( remove self from queue )

 */