#include <common.h>
#include <sdk.h>

#include "input.h"

void gui::controls::input_str::make( const char* label, char* buf, u32 buf_size, ImGuiInputTextFlags flags )
{
	ImGui::PushFont( n_sdk::c_fonts::get( ).m_input );

	auto current = ImGui::GetCursorPos( );
	ImGui::SetCursorPos( ImVec2( current.x, current.y - 1.f ) ); // xd

	ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 1.f );
	ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.f );

	ImGui::PushStyleColor( ImGuiCol_FrameBg, ImU32( 0xff090c10 ) );
	ImGui::PushStyleColor( ImGuiCol_Border, ImU32( m_is_hovered ? 0xff252526 : 0x5f252526 ) );
	ImGui::PushStyleColor( ImGuiCol_TextSelectedBg, ImU32( 0x5f252526 ) );

	ImGui::InputText( label, buf, buf_size, flags );

	m_is_hovered = ImGui::IsItemHovered( ImGuiHoveredFlags_RectOnly );

	ImGui::PopStyleVar( 2 );
	ImGui::PopStyleColor( 3 );

	ImGui::PopFont( );
}
