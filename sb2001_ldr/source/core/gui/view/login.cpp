#include <common.h>
#include <sdk.h>

#include "login.h"

#include "../../network/session.h"

char login_buffer[ 16 + 1 /* null */ ];
char password_buffer[ 32 + 1 /* null */ ];

void dump_packet_mem( u32 addr, u32 size )
{
	for ( u32 c = addr; c < addr + size; c++ )
	{
		u8 byte = *( u8* )c;

		printf( "%#hhx ", byte );
	}
	std::cout << std::endl;
}

void gui::view::c_login::make( )
{
	const auto max_start = ImGui::GetWindowContentRegionMax( );
	const auto min_start = ImGui::GetWindowContentRegionMin( );

	const auto login_size = ImGui::CalcTextSize( "login:" );
	const auto password_size = ImGui::CalcTextSize( "password:" );

	const auto input_flags = 
		ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_CharsNoBlank;
	
	// CENTER
	//ImGui::SetCursorPos( ImVec2( max_start.x / 2 - ( login_size.x * 2 ), max_start.y / 2 - login_size.y ) );
	//ImGui::Text( "login:" ); ImGui::SameLine( );
	//ImGui::Text( "|login region|" );
	//ImGui::SetCursorPos( ImVec2( max_start.x / 2 - ( login_size.x * 2 ) - ( password_size.x - login_size.x ),
	//	max_start.y / 2 - password_size.y + ( login_size.y ) )  );
	//ImGui::Text( "password:" ); ImGui::SameLine( );
	//ImGui::Text( "|password region|" );
	
	// BOTTOM LEFT
	//ImGui::SetCursorPos( ImVec2( min_start.x + password_size.x - login_size.x, 
	//	max_start.y - login_size.y - password_size.y - 16.f ) );
	//ImGui::Text( "login:" ); ImGui::SameLine( );
	//m_ilogin.make( "##login_region", login_buffer, 16, input_flags);

	//ImGui::SetCursorPos( ImVec2( min_start.x,
	//	max_start.y - password_size.y - 6.f ) );
	//ImGui::Text( "password:" ); ImGui::SameLine( );
	//m_ipassword.make( "##password_region", password_buffer, 32, input_flags | ImGuiInputTextFlags_Password );

	// TOP LEFT - align login
	//ImGui::SetCursorPos( ImVec2( min_start.x + password_size.x - login_size.x,
	//	min_start.y ) );
	//ImGui::Text( "login:" ); ImGui::SameLine( );
	//ImGui::Text( "|login region|" );
	//ImGui::SetCursorPos( ImVec2( min_start.x,
	//	min_start.y + login_size.y + 1.f ) );
	//ImGui::Text( "password:" ); ImGui::SameLine( );
	//ImGui::Text( "|password region|" );

	// TOP LEFT - no align login
	//ImGui::SetCursorPos( ImVec2( min_start.x,
	//	min_start.y ) );
	ImGui::Text( "login:" ); ImGui::SameLine( min_start.x + password_size.x + 8.f );
	m_ilogin.make( "##login_region", login_buffer, 16, input_flags );
	ImGui::SetCursorPos( ImVec2( min_start.x,
		min_start.y + login_size.y + 16.f ) );
	ImGui::Text( "password:" ); ImGui::SameLine( );
	m_ipassword.make( "##password_region", password_buffer, 32, input_flags | ImGuiInputTextFlags_Password );

	ImGui::SameLine( );

	if ( m_bsend.make( "connect" ) )
	{
		std::cout << "log: connect button clicked" << std::endl;
		gui::c_status_bar::get( ).set( "connecting..." );

		std::thread( [ & ]( ) {

			packet::login login_packet;
			memcpy( &login_packet.login_buffer, &login_buffer, sizeof( login_packet.login_buffer ) );
			memcpy( &login_packet.password_buffer, &password_buffer, sizeof( login_packet.password_buffer ) );

			auto login_as_bytes = packet::convert::to_bytes( &login_packet, sizeof( packet::login ) );

			auto bytes_recived = n_core::c_session::get( ).sync_send( login_as_bytes );
			auto login_response = packet::convert::from_bytes< packet::login_response >( bytes_recived );
			
			if ( login_response.result == 0 )
			{
				gui::c_status_bar::get( ).set( "invalid, try again" );
			}
			else if ( login_response.result == 1 )
			{
				gui::c_status_bar::get( ).set( ( std::string( "welcome, " ) + std::string( login_buffer ) ).c_str( ) );
				on_login( );
			}

		} ).detach( );
	}
}

bool gui::view::c_login::is_fulfilled( )
{
	return m_is_fulfilled;
}

void gui::view::c_login::on_login( )
{
	m_is_fulfilled = 1;
}
