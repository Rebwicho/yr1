#ifndef FONTS_H
#define FONTS_H

namespace n_sdk
{
	// this is only temporary
	// todo: ^
	class c_fonts : public c_singleton< c_fonts >
	{
	public:
		ImFont* m_default = nullptr;
		ImFont* m_input = nullptr;
	};
}

#endif // FONTS_H
