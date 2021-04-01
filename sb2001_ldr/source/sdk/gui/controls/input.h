#ifndef INPUT_H
#define INPUT_H

namespace gui::controls
{
	struct input_str
	{
	public:
		void make( const char* label, char* buf, u32 buf_size, ImGuiInputTextFlags flags );

	private:
		bool m_is_hovered = 0;
	};
}

#endif // INPUT_H
