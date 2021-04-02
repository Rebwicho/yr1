#include <common.h>
#include <sdk.h>

#include "packet.h"

std::vector<u8> sdk::network::converter::to_bytes( void* packet, std::size_t size_of_packet )
{
	return std::vector< u8 >( ( u8* )packet, ( u8* )packet + size_of_packet );
}
