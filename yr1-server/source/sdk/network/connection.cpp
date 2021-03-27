#include <common.h>

#include "connection.h"

void sdk::c_connection::start_socket( tcp::socket socket )
{
	m_socket = tcp::socket( std::move( socket ) );
}
