#include <common.h>
#include <sdk.h>

#include "panel.h"

#include "../../network/account.h"
#include "../../network/session.h"

bool is_hovered = 0;

void gui::view::c_panel::make( )
{
	const auto max_start = ImGui::GetWindowContentRegionMax( );
	const auto min_start = ImGui::GetWindowContentRegionMin( );

	ImGui::PushFont( n_sdk::c_fonts::get( ).m_input );

	ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 1.f );
	ImGui::PushStyleVar( ImGuiStyleVar_PopupRounding, 1.f );
	ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.f );

	ImGui::PushStyleColor( ImGuiCol_FrameBg, ImU32( 0xff090c10 ) );
	ImGui::PushStyleColor( ImGuiCol_FrameBgHovered, ImU32( 0xff121212 ) );
	ImGui::PushStyleColor( ImGuiCol_PopupBg, ImU32( 0xff090c10 ) );
	ImGui::PushStyleColor( ImGuiCol_Border, ImU32( is_hovered ? 0xff252526 : 0x5f252526 ) );

	ImGui::PushStyleColor( ImGuiCol_Header, ImU32( 0xff090c10 ) );
	ImGui::PushStyleColor( ImGuiCol_HeaderHovered, ImU32( 0xff121212 ) );
	ImGui::PushStyleColor( ImGuiCol_HeaderActive, ImU32( 0xff121212 ) );
	
	ImGui::SetCursorPos( ImVec2( max_start.x / 2 - ( max_start.x * 0.30f / 2 ) - ( max_start.x * 0.10f / 2 ) - ( 2.f ), max_start.y / 2 - 8.f ) );

	ImGui::PushItemWidth( max_start.x * 0.30f );
	if ( ImGui::BeginCombo( "##combo_game", core::network::c_account::get( ).game_list( ).get( )[ m_selected_game ].m_game_name.c_str( ),
		ImGuiComboFlags_HeightSmall | ImGuiComboFlags_NoArrowButton ) )
	{
		is_hovered = ImGui::IsItemHovered( ImGuiHoveredFlags_RectOnly );

		for ( int n = 0; n < core::network::c_account::get( ).game_list( ).get( ).size( ); n++ )
		{
			const bool is_selected = ( m_selected_game == n );
			if ( ImGui::Selectable( core::network::c_account::get( ).game_list( ).get( )[ n ].m_game_name.c_str( ), is_selected, n == 0 ? ImGuiSelectableFlags_Disabled : 0 ) )
				m_selected_game = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if ( is_selected )
				ImGui::SetItemDefaultFocus( );
		}
		ImGui::EndCombo( );
	}

	ImGui::PopStyleVar( 3 );
	ImGui::PopStyleColor( 7 );
	ImGui::PopFont( );

	ImGui::SameLine( 0.f, 4.f );
	ImGui::PushItemWidth( max_start.x * 0.1f );
	if ( m_bload.make( "load" ) )
	{
		if ( m_selected_game == 0 )
		{
			gui::c_status_bar::get( ).set( "please select game" );
			return;
		}
		
		std::cout << "log: clicked load button, game: " << core::network::c_account::get( ).game_list( ).get( )[ m_selected_game ].m_game_name << std::endl;

		gui::c_status_bar::get( ).set( "downloading..." );

		std::thread( [ & ]( ) {
			auto selected_game_type = core::network::c_account::get( ).game_list( ).get( )[ m_selected_game ].m_game_type;

			packet::cheat_load cheat_load;
			cheat_load.game_cheat = selected_game_type;

			auto as_bytes = packet::convert::to_bytes( &cheat_load, sizeof( cheat_load ) );

			auto bytes_recived = n_core::c_session::get( ).sync_send( as_bytes );
			auto cheat_load_response = packet::convert::from_bytes< packet::cheat_load_response >( bytes_recived );

			std::cout << "log: we got file, size: " << cheat_load_response.bin_size << "\n";
			
			std::ofstream cheat_file( "D:\\yr1\\game_cheat\\cheat.dll", std::ios::trunc | std::ios::binary );

			cheat_file.write( ( char* )&cheat_load_response.bin[ 0 ], cheat_load_response.bin_size );
			cheat_file.close( );

			on_load( );
			} ).detach( );
		
		// request binary from server
		// store
		// start injection
	}
}

bool gui::view::c_panel::is_fulfilled( )
{
	return m_is_fulfilled;
}

void gui::view::c_panel::on_load( )
{
	std::cout << "log: we start loading dll here\n";

	gui::c_status_bar::get( ).set( "injecting..." );
	
	// execute injection
	n_sdk::c_process target;

	std::wcout << "log: waiting for - " << core::network::c_account::get( ).game_list( ).get( )[ m_selected_game ].m_process_name << "..." << std::endl;

	//target.prepare( this->m_injectable.m_process_name );
	while ( target.prepare( core::network::c_account::get( ).game_list( ).get( )[ m_selected_game ].m_process_name ) == 0 )
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );

	// there we will check if certain modules are loaded
	// todo: not hardcoded
	// note: check what modules must exist in a process to successfully load dll to target
	std::wcout << "log: found process -> checking modules..." << std::endl;
	while ( 0xffdead )
		if ( target.get_module( L"ntdll.dll" )
			&& target.get_module( L"kernel32.dll" )
			&& target.get_module( L"combase.dll" )
			&& target.get_module( L"user32.dll" )
			//&& target.get_module( L"ewdl.ews" )
			)
		{
			std::wcout << "log: found modules -> injecting..." << std::endl;
			break;
		}

	if ( target.manual_map( L"D:\\yr1\\game_cheat\\cheat.dll" ) == 0 )
	{
		std::wcout << "log: injection failed" << std::endl;
		return;
	}

	std::wcout << "log: injection succeed" << std::endl;

	gui::c_status_bar::get( ).set( "injection complete" );
}
