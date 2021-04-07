#include <common.h>
#include <sdk.h>

#include "game_list.h"

std::vector<sdk::network::s_game>& sdk::account::c_game_list::get( )
{
	return m_client_list;
}

void sdk::account::c_game_list::add( enums::game_type game_type )
{
	for ( auto& game : sdk::network::game::list )
	{
		if ( game.m_game_type == game_type )
		{
			m_client_list.push_back( game ); break;
		}
	}
}
