#ifndef ENUM_FILE_H
#define ENUM_FILE_H

namespace sdk::enums
{
	enum class e_packet_type : u8
	{
		login,
		login_result
	};
}

namespace enumer
{
	typedef sdk::enums::e_packet_type packet_type_t;
}

#endif // ENUM_FILE_H
