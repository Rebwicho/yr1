#include <common.h>
#include <sdk.h>

#include "account.h"

sdk::account::c_game_list& core::network::c_account::game_list( )
{
	return m_game_list;
}
