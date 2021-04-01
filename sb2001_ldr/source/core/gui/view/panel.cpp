#include <common.h>
#include <sdk.h>

#include "panel.h"

//ImGui::PushFont( n_sdk::c_fonts::get( ).m_input );
//
//auto current = ImGui::GetCursorPos( );
//ImGui::SetCursorPos( ImVec2( current.x, current.y - 1.f ) ); // xd
//
//ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 1.f );
//ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.f );
//
//ImGui::PushStyleColor( ImGuiCol_FrameBg, ImU32( 0xff090c10 ) );
//ImGui::PushStyleColor( ImGuiCol_Border, ImU32( m_is_hovered ? 0xff252526 : 0x5f252526 ) );
//ImGui::PushStyleColor( ImGuiCol_TextSelectedBg, ImU32( 0x5f252526 ) );
//
//ImGui::InputText( label, buf, buf_size, flags );
//
//m_is_hovered = ImGui::IsItemHovered( ImGuiHoveredFlags_RectOnly );
//
//ImGui::PopStyleVar( 2 );
//ImGui::PopStyleColor( 3 );
//
//ImGui::PopFont( );

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

	const char* items[ ] = { "choose game", "Nostale", "NosWings" };
	static int item_current_idx = 0; // Here we store our selection data as an index.
	const char* combo_label = items[ item_current_idx ];  // Label to preview before opening the combo (technically it could be anything)
	
	ImGui::PushItemWidth( max_start.x * 0.30f );
	if ( ImGui::BeginCombo( "##combo_game", combo_label, ImGuiComboFlags_HeightSmall | ImGuiComboFlags_NoArrowButton ) )
	{
		is_hovered = ImGui::IsItemHovered( ImGuiHoveredFlags_RectOnly );

		for ( int n = 0; n < IM_ARRAYSIZE( items ); n++ )
		{
			const bool is_selected = ( item_current_idx == n );
			if ( ImGui::Selectable( items[ n ], is_selected, n == 0 ? ImGuiSelectableFlags_Disabled : 0 ) )
				item_current_idx = n;

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
		std::cout << "log: clicked load button, game: "<< items[ item_current_idx ] << std::endl;
	}
}

bool gui::view::c_panel::is_fulfilled( )
{
	return m_is_fulfilled;
}