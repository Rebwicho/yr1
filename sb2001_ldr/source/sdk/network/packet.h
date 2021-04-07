#ifndef PACKET_H
#define PACKET_H

namespace sdk::network
{
	class converter
	{
	public:
		// convert vector of bytes to desired packet
		template < typename desired_packet_type >
		static desired_packet_type from_bytes( std::vector< u8 >& bytes );

		// conver packet to vector of bytes
		static std::vector< u8 > to_bytes( void* packet, std::size_t size_of_packet );
	};

	template <typename desired_packet_type>
	desired_packet_type converter::from_bytes( std::vector<u8>& bytes )
	{
		auto desired_packet = desired_packet_type( );

		std::memcpy( &desired_packet, bytes.data( ), sizeof( desired_packet ) );

		return desired_packet;
	}
}

namespace packet
{
	typedef sdk::network::converter convert;
}

#endif // PACKET_H
