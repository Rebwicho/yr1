#ifndef ENUM_FILE_H
#define ENUM_FILE_H

namespace sdk::enums
{
	enum class e_packet_type : u8
	{
		login,
		login_response,
		cheat_list, // client requests cheat list that is available for him
		cheat_list_response, // packet that contains list of available cheats from client that requested
		
	};
}

namespace enumer
{
	typedef sdk::enums::e_packet_type packet_type_t;
}

#endif // ENUM_FILE_H
