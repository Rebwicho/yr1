#include <common.h>
#include <sdk.h>

#include "button.h"

bool gui::controls::button::make( const char* label, const ImVec2& size )
{
	ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 2.f );
	ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 0.f );

	ImGui::PushStyleColor( ImGuiCol_Button, ImU32( 0xff238636 ) );
	ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImU32( 0xff2ea043 ) );
	ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImU32( 0xff0f5323 ) );
	ImGui::PushStyleColor( ImGuiCol_Border, ImU32( m_is_hovered ? 0xff252526 : 0x1f252526 ) );

	const auto is_clicked = ImGui::Button( label, size );

	m_is_hovered = ImGui::IsItemHovered( ImGuiHoveredFlags_RectOnly );

	ImGui::PopStyleVar( 2 );
	ImGui::PopStyleColor( 4 );
	
	return is_clicked ? 1 : 0;
}
