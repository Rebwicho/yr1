#include <common.h>
#include <sdk.h>

#include "login.h"

char login_buffer[ 16 ];
char password_buffer[ 32 ];

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

		// lets spawn thread that waits for 3s and then calls on_login
		// to simulate login process for now
		std::thread( [ & ]( ) {
			static u32 wait_time = 0;
			if ( wait_time == 0 ) wait_time = GetTickCount( ) + 2000;

			while ( GetTickCount( ) < wait_time )
				std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );

			gui::c_status_bar::get( ).set( ( std::string( "welcome, " ) + std::string( login_buffer ) ).c_str( ) );
			
			on_login( );
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
