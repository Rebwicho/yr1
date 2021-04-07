#include <common.h>
#include <sdk.h>

#include "game.h"

std::vector< sdk::network::s_game > sdk::network::game::list = {
	s_game { enums::game_type::none, "none", L"none" }, // id: 0		
	s_game { enums::game_type::Nostale, "Nostale", L"NostaleClientX.exe" }, // id: 1
	s_game { enums::game_type::Noswings, "Noswings", L"NosWings.exe" }, // id: 2
	s_game { enums::game_type::test1, "test1", L"test1" }, // id: 3
	s_game { enums::game_type::test2, "test2", L"test2" }  // id: 4

};