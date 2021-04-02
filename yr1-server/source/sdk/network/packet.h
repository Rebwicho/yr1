#ifndef PACKET_H
#define PACKET_H

namespace sdk::network
{
	class c_packet
	{
	public:
		template < typename packet_type >
		static packet_type convert_bytes( std::vector< u8 >& recived_bytes );
	
	};

	//template <typename packet_type>
	//packet_type c_packet::convert_bytes( std::vector<u8>& recived_bytes )
	//{
	//	auto packet = packet_type( );
	//	std::memcpy( &packet, recived_bytes.data( ), sizeof( packet_type ) );

	//	return packet;
	//}

	template <typename packet_type>
	packet_type c_packet::convert_bytes( std::vector<u8>& recived_bytes )
	{
		auto packet = packet_type( );
		std::memcpy( &packet, recived_bytes.data( ), sizeof( packet_type ) );

		return packet;
	}
}

#endif // PACKET_H
