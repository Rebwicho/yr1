#ifndef ENUM_FILE_H
#define ENUM_FILE_H

namespace n_sdk::n_enum
{
	enum class e_view_type : u32
	{
		empty, // empty is when we still are not ready to go to login view
		login, // view where we try to login to loader
		panel, // we are after login this is main view where we can do stuff

		/* ideas: */
		settings, // we can have some settings?
		visual_settings, // some modifications to loader look?
		account, // we can have some account infos?
	};
	
	enum class e_packet_type : u8
	{
		login,
		login_response,
		game_list, // client requests game list that he have active subscription for
		game_list_response, // packet that contains list of available games that client have subscription for
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
	typedef n_sdk::n_enum::e_packet_type packet_type_t;
}

namespace enums
{
	typedef n_sdk::n_enum::e_game_type game_type;
}

namespace n_sdk
{
	typedef n_sdk::n_enum::e_view_type view_type_t;
	
}


#endif // ENUM_FILE_H
