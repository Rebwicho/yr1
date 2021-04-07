#ifndef GAME_H
#define GAME_H

namespace sdk::network
{
	struct s_game
	{
		enums::game_type m_game_type;
		std::string m_game_name;
		std::wstring m_process_name;
	};

	namespace game
	{ // those are predefined game structs so we can identify by just id we get from server
		
		extern std::vector< s_game > list; // = {
		//	s_game { enums::game_type::none, "none", L"none" }, // id: 0
		//	s_game { enums::game_type::Nostale, "Nostale", L"NostaleClientX.exe" }, // id: 1
		//	s_game { enums::game_type::Noswings, "Noswings", L"NosWings.exe" }, // id: 2
		//	s_game { enums::game_type::test1, "test1", L"test1" }, // id: 3
		//	s_game { enums::game_type::test2, "test2", L"test2" }  // id: 4

		//};

	}
}

#endif // GAME_H
