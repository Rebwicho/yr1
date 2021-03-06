#ifndef ENUM_FILE_H
#define ENUM_FILE_H

namespace sdk::n_enum
{
	enum class e_packet_type : u8
	{
		login,
		login_response,
		game_list, // client requests game list that he have active subscription for
		game_list_response, // packet that contains list of available games that client have subscription for
		cheat_load, // client requested cheat with its name 
		cheat_load_response, // server sends back cheat that was requested
	};

	enum class e_game_type : u8
	{
		none,
		Nostale,
		Noswings,
		test1,
		test2,
	};	
}

namespace enumer
{
	typedef sdk::n_enum::e_packet_type packet_type_t;
}

namespace enums
{
	typedef sdk::n_enum::e_game_type game_type;
}

#endif // ENUM_FILE_H
