#ifndef RECEIVER_H
#define RECEIVER_H

namespace core
{
	class c_receiver : public c_singleton< c_receiver >
	{
	public:
		// handler gets recived bytes from client as a param
		// first byte is type of packet
		static void handler( std::deque< u8 > recived_bytes );

		//template < typename packet_type >
		//static packet_type convert_bytes( std::deque< u8 >& recived_bytes );

		//template < typename packet_type >
		//static packet_type convert_bytes( std::vector< u8 >& recived_bytes );

		template < typename packet_type >
		static packet_type convert_bytes( std::vector< u8 >& recived_bytes );
	};

}

#endif // RECEIVER_H
