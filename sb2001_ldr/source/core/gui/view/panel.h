#ifndef PANEL_H
#define PANEL_H

namespace gui::view
{
	class c_panel final : public n_sdk::c_view, public c_singleton< c_panel >
	{
	public:
		void make( ) override;
		bool is_fulfilled( ) override;

	public:


	private: /* define controls here */
		controls::button m_bload;
	
	private:
		bool m_is_fulfilled = 0;
	};
}

#endif // PANEL_H
