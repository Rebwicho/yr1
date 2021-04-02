#ifndef ENUM_FILE_H
#define ENUM_FILE_H

namespace sdk::enums
{
	enum class e_packet_type : u8
	{
		login,
		login_response,
		game_list, // client requests game list that he have active subscription for
		game_list_response, // packet that contains list of available games that client have subscription for
		
	};
}

namespace enumer
{
	typedef sdk::enums::e_packet_type packet_type_t;
}

#endif // ENUM_FILE_H
