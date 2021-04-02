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

		// lets spawn thread that waits for 3s and then calls on_login
		// to simulate login process for now
		std::thread( [ & ]( ) {
			//static u32 wait_time = 0;
			//if ( wait_time == 0 ) wait_time = GetTickCount( ) + 2000;

			//while ( GetTickCount( ) < wait_time )
			//	std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );

			packet::login login_packet;
			memcpy( &login_packet.login_buffer, &login_buffer, sizeof( login_packet.login_buffer ) );
			memcpy( &login_packet.password_buffer, &password_buffer, sizeof( login_packet.password_buffer ) );
			std::vector< u8 > login_as_bytes( ( u8* )&login_packet, ( u8* )&login_packet + sizeof( login_packet ) );

			//auto result_data = n_core::c_session::get( ).sync_send( login_as_bytes );
			auto bytes_recived = n_core::c_session::get( ).sync_send( login_as_bytes );

			auto result_data = packet::login_result( );
			std::memcpy( &result_data, &bytes_recived[ 0 ], sizeof( packet::login_result ) );
			
			//auto result_data = core::c_receiver::convert_bytes< packet::login_result >( bytes_recived );

			if ( result_data.result == 0 )
			{
				gui::c_status_bar::get( ).set( "invalid, try again" );
			}
			else if ( result_data.result == 1 )
			{
				gui::c_status_bar::get( ).set( ( std::string( "welcome, " ) + std::string( login_buffer ) ).c_str( ) );
				on_login( );
			}		
		} ).detach( );
	}

	//if ( m_bdummy.make( "send dummy" ) )
	//{
	//	std::cout << "log: send dummy button clicked" << std::endl;

	//	//dump_packet_mem( ( u32 )&login_buffer, sizeof( login_buffer ) );
	//	//dump_packet_mem( ( u32 )&password_buffer, sizeof( password_buffer ) );

	//	packet::login login_packet;
	//	memcpy( &login_packet.login_buffer, &login_buffer, sizeof( login_packet.login_buffer ) );
	//	memcpy( &login_packet.password_buffer, &password_buffer, sizeof( login_packet.password_buffer ) );
	//	std::vector< u8 > login_as_bytes( ( u8* )&login_packet, ( u8* )&login_packet + sizeof( login_packet ) );

	//	//login_packet.login_buffer = login_buffer;
	//	//std::cout << "log: login_packet.login_buffer: " << login_packet.login_buffer << std::endl;
	//
	//	//std::cout << "log: login_packet.password_buffer: " << login_packet.password_buffer << std::endl;

	//	//dump_packet_mem( ( u32 )&login_packet, sizeof( login_packet ) );


	//	//for ( auto& byte : login_as_bytes )
	//	//	printf( "%#hhx ", byte );
	//	//std::cout << std::endl;

	//	n_core::c_session::get( ).send( login_as_bytes );
	//}

}

bool gui::view::c_login::is_fulfilled( )
{
	return m_is_fulfilled;
}

void gui::view::c_login::on_login( )
{
	m_is_fulfilled = 1;
}
