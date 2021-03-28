#ifndef PACKETS_H
#define PACKETS_H

#pragma pack(push)
#pragma pack(1)

#define packet_start( packet_name, packet_type ) \
struct packet_name \
{ \
	private: enumer::packet_type_t m_type = packet_type; \
public: // ...

#define packet_end };

namespace sdk::packets
{
	packet_start( s_test_recv1, enumer::packet_type_t::test_recv1 )
		u32 test_value;
	packet_end

	packet_start( s_test_recv2, enumer::packet_type_t::test_recv2 )
		u32 test_value;
		u32 test_value2;
	packet_end
	
	packet_start( s_test_recv3, enumer::packet_type_t::test_recv3 )
		u32 test_value;
		u32 test_value2;
		u32 test_value3;
	packet_end

	packet_start( s_test_recv4, enumer::packet_type_t::test_recv4 )
		u32 test_value;
		u32 test_value2;
		u32 test_value3;
		u32 test_value4;
	packet_end
	
	// ...

}
#pragma pack(pop)

namespace packet
{
	typedef sdk::packets::s_test_recv1 test_recv_t1;
	typedef sdk::packets::s_test_recv2 test_recv_t2;
	typedef sdk::packets::s_test_recv3 test_recv_t3;
	typedef sdk::packets::s_test_recv4 test_recv_t4;
}

#endif // PACKETS_H
