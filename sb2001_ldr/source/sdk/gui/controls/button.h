#ifndef BUTTON_H
#define BUTTON_H

namespace gui::controls
{
	struct button
	{
	public:
		//void make( const char* label, char* buf, u32 buf_size, ImGuiInputTextFlags flags );
		bool make( const char* label, const ImVec2& size = ImVec2( 0.f, 0.f ) );

	private:
		bool m_is_hovered = 0;
	};
}

#endif // BUTTON_H
