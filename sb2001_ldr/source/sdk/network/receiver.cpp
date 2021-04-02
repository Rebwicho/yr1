#include <common.h>
#include <sdk.h>

#include "receiver.h"

#include "../../core/gui/view/login.h"

// to-remove:
//void dump_packet_mem( u32 addr, u32 size )
//{
//	for ( u32 c = addr; c < addr + size; c++ )
//	{
//		u8 byte = *( u8* )c;
//
//		printf( "%#hhx ", byte );
//	}
//	std::cout << std::endl;
//}

void core::c_receiver::handler( std::deque<u8> recived_bytes )
{
	std::cout << "reciever: got " << recived_bytes.size( ) << " bytes> ";

	for ( auto& byte : recived_bytes )
		printf( "%#hhx ", byte );
	std::cout << std::endl;

	// deduce type of packet
	auto packet_type = recived_bytes[ 0 ];

	printf( "reciever: packet type is %#hhx\n", packet_type );

	switch ( ( enumer::packet_type_t )packet_type )
	{
		// on each case we will have something like on_packet_name( packet );
		// that will handle what operations it needs to do with it

		case n_sdk::n_enum::e_packet_type::login: break;
		case n_sdk::n_enum::e_packet_type::login_response:
		{
			//auto packet = convert_bytes< packet::login_result >( recived_bytes );

			//switch ( packet.result )
			//{
			//	case 0:
			//	{
			//		// null
			//		gui::c_status_bar::get( ).set( "null" );
			//	} break;

			//	case 1:
			//	{
			//		// failed
			//		gui::c_status_bar::get( ).set( "invalid, try again" );
			//		
			//	} break;

			//	case 2:
			//	{
			//		// success
			//		//gui::c_status_bar::get( ).set( "invalid, try again" );
			//		//gui::view::c_login::get( ).on_login( );

			//	} break;
			//}
			
		} break;
		/*case sdk::enums::e_packet_type::test_recv1:
		{
			auto packet = convert_bytes< packet::test_recv_t1 >( recived_bytes );

		} break;
		case sdk::enums::e_packet_type::test_recv2:
		{
			auto packet = convert_bytes< packet::test_recv_t2 >( recived_bytes );

		} break;
		case sdk::enums::e_packet_type::test_recv3:
		{
			auto packet = convert_bytes< packet::test_recv_t3 >( recived_bytes );

		} break;
		case sdk::enums::e_packet_type::test_recv4:
		{
			auto packet = convert_bytes< packet::test_recv_t4 >( recived_bytes );

		} break;*/
		default: break;
	}

}

//template<typename packet_type>
//packet_type core::c_receiver::convert_bytes( std::deque<u8>& recived_bytes )
//{
//	auto packet = packet_type( );
//	std::memcpy( &packet, &recived_bytes[ 0 ], sizeof( packet_type ) - 1 );
//
//	return packet;
//}

//template<typename packet_type>
//packet_type core::c_receiver::convert_bytes( std::vector<u8>& recived_bytes )
//{
//	auto packet = packet_type( );
//	std::memcpy( &packet, &recived_bytes[ 0 ], sizeof( packet_type ) - 1 );
//
//	return packet;
//}

template<typename packet_type>
packet_type core::c_receiver::convert_bytes( std::vector<u8>& recived_bytes )
{
	auto packet = packet_type( );
	std::memcpy( &packet, &recived_bytes.data( ), sizeof( packet_type ) );

	return packet;
}