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
		void on_load( );
	
	private: /* define controls here */
		controls::button m_bload;
	
	private:
		bool m_is_prepared = 0; // do we have client game_list 
		bool m_is_fulfilled = 0;

		u32 m_selected_game = 0;
	};
}

#endif // PANEL_H
