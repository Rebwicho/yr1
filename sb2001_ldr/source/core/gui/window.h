#ifndef WINDOW_H
#define WINDOW_H

// todo: decision
// i can make this file be "sdk friendly"
// so it will be compatible with any other
// application that wants to create windows window

// prepare window
// create window
// render window / we are rendering current window_view
// close window

// create
// run

// window bg color: 0d1117

namespace n_core
{
	class c_window : public c_singleton< c_window >
	{
	public:
		bool create( );
		bool destroy( );
		
		bool run( );

	protected:
		bool window_register( WNDCLASS& wnd_class );
		bool window_create( WNDCLASS& wnd_class );
		bool window_destroy( );
		
		bool device_create( );
		bool device_destroy( );

		bool imgui_create_theme( );
		bool imgui_create( );
		bool imgui_destroy( );

	private:
		void on_region_update( );
		void on_position_update( LPARAM lparam );

		void view_switch( n_sdk::view_type_t next_view_type );
		void view_make( );
		
		LRESULT on_nchittest( LPARAM lparam );
		LRESULT on_nccalcsize( WPARAM wparam, LPARAM lparam );

		LRESULT on_paint( );

		LRESULT on_wnd_proc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
		
		static LRESULT __stdcall wnd_proc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
	public:
	
	protected:
		n_sdk::view_type_t m_view_type = n_sdk::view_type_t::empty;
		//u32* m_view = nullptr;

		//std::string m_bottom_message = "starting...";
		
		u8 m_destroy = 0;
	
	private:
		LPDIRECT3D9 m_d3d = nullptr;
		LPDIRECT3DDEVICE9 m_device = nullptr;
		
		D3DPRESENT_PARAMETERS m_d3d_params = { };
		
		HWND m_hwnd = nullptr;

		unsigned m_width = 0;
		unsigned m_height = 0;

		RECT m_region = { };
	};
}

#endif // WINDOW_H
