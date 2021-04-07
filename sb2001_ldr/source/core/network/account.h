#ifndef ACCOUNT_H
#define ACCOUNT_H

namespace core::network
{
	class c_account : public c_singleton< c_account >
	{
	public:
		sdk::account::c_game_list& game_list( );
	
	private:
		sdk::account::c_game_list m_game_list;
		// ...
	};
}

#endif // ACCOUNT_H
