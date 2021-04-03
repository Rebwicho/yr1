#ifndef GAME_LIST_H
#define GAME_LIST_H

namespace sdk::account
{
	class c_game_list
	{
	public:
		std::vector< sdk::network::s_game >& get( );
		void add( enums::game_type game_type );
	
	private:
		std::vector< sdk::network::s_game > m_client_list = {
			// always none first
			sdk::network::game::list[ 0 ]
		};
	};
}

#endif // GAME_LIST_H
