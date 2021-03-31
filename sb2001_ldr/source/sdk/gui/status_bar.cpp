#include <common.h>
#include <sdk.h>

#include "status_bar.h"

void gui::c_status_bar::set( const char* message )
{
	m_message = message;
}

void gui::c_status_bar::make( )
{
	const auto time_size = ImGui::CalcTextSize( "[ 00:00:00 ]" ); // for shift

	// this is bottom bar separator region
	ImGui::PushStyleColor( ImGuiCol_ChildBg, ImU32( 0xff000000 ) );
	ImGui::SetNextWindowPos( ImVec2( ImGui::GetCursorPos( ).x, ImGui::GetCursorPos( ).y ) );
	ImGui::BeginChild( "##bottom_separator", ImVec2( ImGui::GetContentRegionAvail( ).x, 2.f ) );
	{
		ImGui::EndChild( ); // END OF ##bottom_separator

		// pop stuff from "##bottom_separator" 
		ImGui::PopStyleColor( );
	}

	// this is bottom bar text region
	ImGui::BeginChild( "##bottom_text", ImVec2( 0, 0 ) );
	{
		ImGui::Text( m_message.c_str( ) );

		//auto time_size = ImGui::CalcTextSize( "[ 00:00:00 ]" );

		ImGui::SameLine( ImGui::GetWindowContentRegionMax( ).x - time_size.x );
		ImGui::Text( "[ %s ]", n_sdk::c_time::now( ).c_str( ) );

		ImGui::EndChild( ); // END OF ##bottom_text
	}
}
