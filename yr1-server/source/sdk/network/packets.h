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
	packet_start( s_login, enumer::packet_type_t::login )
		char login_buffer[ 17 ] = { };
	char password_buffer[ 33 ] = { };
	packet_end

		packet_start( s_login_response, enumer::packet_type_t::login_response )
		u32 result; /* u32 because we can have multiple diffrent results like banned suspended success failed ... */
	packet_end

		packet_start( s_game_list, enumer::packet_type_t::game_list )

		packet_end

		packet_start( s_game_list_response, enumer::packet_type_t::game_list_response )
		enums::game_type games_list[ 4 /* size of how much we have cheats created Nostale, NosWings, ... */ ] = { };
	packet_end

		packet_start( s_cheat_load, enumer::packet_type_t::cheat_load )
		enums::game_type game_cheat;
	packet_end

		packet_start( s_cheat_load_response, enumer::packet_type_t::cheat_load_response )
		std::byte bin[ 1024 ];
	packet_end

		// ...

}
#pragma pack(pop)

namespace packet
{
	typedef sdk::packets::s_login login;
	typedef sdk::packets::s_login_response login_response;
	typedef sdk::packets::s_game_list game_list;
	typedef sdk::packets::s_game_list_response game_list_response;
	typedef sdk::packets::s_cheat_load cheat_load;
	typedef sdk::packets::s_cheat_load_response cheat_load_response;

}

#endif // PACKETS_H