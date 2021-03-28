#ifndef ENUM_FILE_H
#define ENUM_FILE_H

namespace sdk::enums
{
	enum class e_packet_type : u8
	{
		test_recv1 = 1,
		test_recv2 = 2,
		test_recv3 = 3,
		test_recv4 = 4,
	};
}

namespace enumer
{
	typedef sdk::enums::e_packet_type packet_type_t;
}

#endif // ENUM_FILE_H
