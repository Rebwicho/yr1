#include <common.h>
#include <sdk.h>

#include "window.h"

// add views here xd
#include "view/login.h"
#include "view/panel.h"

bool n_core::c_window::create( )
{
	WNDCLASS wnd_class = { };
	std::memset( &wnd_class, 0, sizeof( wnd_class ) );

	if ( window_register( wnd_class ) == 0 ) return 0;
	if ( window_create( wnd_class ) == 0 ) return 0;
	
	if ( device_create( ) == 0 ) return 0;

	if ( imgui_create( ) == 0 ) return 0;
	
	std::cout << "log: created window" << std::endl;

	ShowWindow( m_hwnd, SW_SHOW );
	UpdateWindow( m_hwnd );
	
	return 1;
}

bool n_core::c_window::destroy( )
{
	imgui_destroy( );
	device_destroy( );
	window_destroy( );

	std::cout << "log: destroyed window" << std::endl;
	
	return 1;
}

bool n_core::c_window::run( )
{
	MSG msg; std::memset( &msg, 0, sizeof( msg ) );

	while ( 0xffdead )
	{
		while ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );

			if ( m_destroy == 1 ) break;
			
		}
		
		if ( m_destroy == 1 ) break;
	}

	destroy( );

	return 1;
}

bool n_core::c_window::window_register( WNDCLASS& wnd_class )
{
	wnd_class.style = CS_HREDRAW | CS_VREDRAW;
	wnd_class.hbrBackground = ( HBRUSH )COLOR_WINDOW;
	wnd_class.lpfnWndProc = n_core::c_window::wnd_proc;
	wnd_class.hInstance = GetModuleHandleA( 0 );
	wnd_class.lpszClassName = L"class_name";
	wnd_class.hCursor = ::LoadCursorW( nullptr, IDC_ARROW );

	auto register_result = RegisterClass( &wnd_class );

	if ( register_result == 0 )
	{
		std::cerr << "error: registering window failed " << GetLastError( ) << std::endl;
		return 0;
	}

	return 1;
}

bool n_core::c_window::window_create( WNDCLASS& wnd_class )
{
	auto window_style = WS_POPUP; // | WS_SYSMENU; //  | WS_CAPTION

	m_hwnd = CreateWindowEx(
		0,              // Optional window styles.
		L"class_name",  // Window class
		L"window_name", // Window text
		window_style,   // Window style

		// Size and position
		700, 500, 450, 200,
		0,       // Parent window    
		0,       // Menu
		wnd_class.hInstance,  // Instance handle      
		nullptr           // Additional application data
	);
	
	if ( m_hwnd == nullptr )
	{
		std::cerr << "error: creating window failed " << GetLastError( ) << std::endl;
		return 0;
	}
	
	return 1;
}

bool n_core::c_window::window_destroy( )
{
	::DestroyWindow( m_hwnd );
	::UnregisterClass( L"class_name", GetModuleHandleA( 0 ) );

	return 1;
}

bool n_core::c_window::device_create( )
{
	m_d3d = Direct3DCreate9( D3D_SDK_VERSION );

	memset( &m_d3d_params, 0, sizeof( m_d3d_params ) );

	m_d3d_params.Windowed = TRUE;
	m_d3d_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3d_params.BackBufferFormat = D3DFMT_UNKNOWN;
	m_d3d_params.EnableAutoDepthStencil = TRUE;
	m_d3d_params.AutoDepthStencilFormat = D3DFMT_D16;
	m_d3d_params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	m_d3d_params.hDeviceWindow = m_hwnd;

	m_d3d->CreateDevice( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&m_d3d_params,
		&m_device );

	if ( m_d3d < 0 )
	{
		std::cerr << "error: creating device failed " << GetLastError( ) << std::endl;
		return 0;
	}

	return 1;
}

bool n_core::c_window::device_destroy( )
{
	if ( m_device ) { m_device->Release( ); m_device = nullptr; }
	if ( m_d3d ) { m_d3d->Release( ); m_d3d = nullptr; }
	return 1;
}

bool n_core::c_window::imgui_create_theme( )
{
	ImGui::StyleColorsDark( );

	ImGuiStyle& style = ImGui::GetStyle( );
	style.WindowRounding = 1.f;
	style.FrameRounding = 0.f;
	style.ScrollbarRounding = 0.f;

	// 0xff0d1117
	//style.Colors[ ImGuiCol_WindowBg ] = ImVec4( 0x0d / 255, 0x11 / 255, 0x17 / 255, 1.f );
	//style.Colors[ ImGuiCol_ChildBg ] = ImVec4( 0x0d / 255, 0x11 / 255, 0x17 / 255, 1.f );
	
	return 1;
}

bool n_core::c_window::imgui_create( )
{
	ImGui::CreateContext( );
	ImGuiIO& io = ImGui::GetIO( );
	
	n_sdk::c_fonts::get( ).m_default = io.Fonts->AddFontDefault( );
	n_sdk::c_fonts::get( ).m_input = io.Fonts->AddFontInput( );
	
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.IniFilename = nullptr;

	imgui_create_theme( );
	
	ImGui_ImplWin32_Init( this->m_hwnd );
	ImGui_ImplDX9_Init( m_device );

	return 1;
}

bool n_core::c_window::imgui_destroy( )
{
	ImGui_ImplDX9_Shutdown( );
	ImGui_ImplWin32_Shutdown( );
	ImGui::DestroyContext( );

	return 1;
}

void n_core::c_window::on_region_update( )
{
	RECT old_region = m_region;
	RECT zero_rect = { 0 };

	//else if ( !data->composition_enabled )
	//{
	//	/* For ordinary themed windows when composition is disabled, a region
	//	   is needed to remove the rounded top corners. Make it as large as
	//	   possible to avoid having to change it when the window is resized. */
	//	orgn.left = 0;
	//	data->rgn.top = 0;
	//	data->rgn.right = 32767;
	//	data->rgn.bottom = 32767;
	//}
	//else
	//{
	//	/* Don't mess with the region when composition is enabled and the
	//	   window is not maximized, otherwise it will lose its shadow */
	//	//m_region = zero_rect;
	//	m_region.left = 0;
	//	m_region.top = 0;
	//	m_region.right = 32767;
	//	m_region.bottom = 32767;
	//}

	//if ( EqualRect( &m_region, &old_region ) )
	//	return; // skip equal

	///* Treat empty regions as NULL regions */
	//SetWindowRgn( m_hwnd, EqualRect( &m_region, &zero_rect ) ? nullptr : CreateRectRgnIndirect( &m_region ), 1 );

	/*
	 	if ( EqualRect( &m_region, &r ) )
			SetWindowRgn( m_hwnd, nullptr, 1 );
		else
			SetWindowRgn( m_hwnd, CreateRectRgnIndirect( &m_region ), 1 );	 
	 */
}
void n_core::c_window::on_position_update( LPARAM lparam )
{
	RECT client_rect; GetClientRect( m_hwnd, &client_rect );
	
	auto old_width = m_width;
	auto old_height = m_height;
	
	m_width = client_rect.right;
	m_height = client_rect.bottom;

	const bool is_changed = ( m_width != old_width || m_height != old_height );

	if ( is_changed == 1
		|| ( ( WINDOWPOS* )lparam )->flags & SWP_FRAMECHANGED 
		)
		on_region_update( ); // update region

	if ( is_changed == 0 ) return;
	
	// invalidate rects ...
	//InvalidateRect( m_hwnd, &client_rect, 1 );
	//return;
	if ( m_width > old_width )
	{
		client_rect = { ( l32 )( old_width - 1 ), 0,
			( l32 )old_width, ( l32 )old_height };
		InvalidateRect( m_hwnd, &client_rect, 0 );
	}
	else
	{
		client_rect = { ( l32 )( m_width - 1 ), 0,
			( l32 )m_width, ( l32 )m_height };
		InvalidateRect( m_hwnd, &client_rect, 0 );
	}

	if ( m_height > old_height )
	{
		client_rect = { 0, ( l32 )( old_height - 1 ),
			( l32 )old_width, ( l32 )old_height };
		InvalidateRect( m_hwnd, &client_rect, 0 );
	}
	else
	{
		client_rect = { 0, ( l32 )( m_height - 1 ),
			( l32 )m_width, ( l32 )m_height };
		InvalidateRect( m_hwnd, &client_rect, 0 );
	}

}

void n_core::c_window::view_switch( n_sdk::view_type_t next_view_type )
{
	std::cout << "log: view_switch called with " << ( u32 )next_view_type << std::endl;
	m_view_type = next_view_type;
}

void n_core::c_window::view_make( )
{
	//gui::view::c_login::get( ).make( );
	//auto test = gui::view::c_login::get( ).is_fulfilled( );
	static u32 wait_var = 0; // <-- TODO: remove this line and corresponding
	
	switch ( m_view_type )
	{
		// we can wait for some stuff here if needed but idk
		case n_sdk::n_enum::e_view_type::empty:
		{
			// lets wait some s before we switch to login view
			if ( wait_var == 0 ) wait_var = GetTickCount( ) + 1000;
			if ( GetTickCount( ) < wait_var ) break;

			view_switch( n_sdk::view_type_t::login );
			gui::c_status_bar::get( ).set( "please enter your credentials" );
			
		} break;
		case n_sdk::n_enum::e_view_type::login:
		{
			gui::view::c_login::get( ).make( );

			if ( gui::view::c_login::get( ).is_fulfilled( ) == 1 )
				view_switch( n_sdk::view_type_t::panel );
		} break;
		case n_sdk::n_enum::e_view_type::panel: 
		{		
			gui::view::c_panel::get( ).make( );

			// so now we should have some thing like
			// desired/next view do switch to
			// and parent view so we can get back to it?
		} break;
		case n_sdk::n_enum::e_view_type::settings: break;
		case n_sdk::n_enum::e_view_type::visual_settings: break;
		case n_sdk::n_enum::e_view_type::account: break;
		default: break;
	}
}

LRESULT n_core::c_window::on_nchittest( LPARAM lparam )
{
	POINT mouse = { GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) };
	ScreenToClient( m_hwnd, &mouse );

	u32 frame_size = GetSystemMetrics( SM_CXFRAME ); // + GetSystemMetrics(SM_CXPADDEDBORDER);
	u32 diagonal_width = frame_size * 2 + GetSystemMetrics( SM_CXBORDER );

	if ( mouse.y < frame_size )
	{
		if ( mouse.x < diagonal_width )
			return HTTOPLEFT;
		if ( mouse.x >= ( int )m_width - diagonal_width )
			return HTTOPRIGHT;
		return HTTOP;
	}

	if ( mouse.y >= ( int )m_height - frame_size )
	{
		if ( mouse.x < diagonal_width )
			return HTBOTTOMLEFT;
		if ( mouse.x >= ( int )m_width - ( int )diagonal_width )
			return HTBOTTOMRIGHT;
		return HTBOTTOM;
	}

	if ( mouse.x < frame_size )
		return HTLEFT;
	
	if ( mouse.x >= ( int )m_width - frame_size )
		return HTRIGHT;
	
	return 1;
}
LRESULT n_core::c_window::on_nccalcsize( WPARAM wparam, LPARAM lparam )
{
	//if ( wparam == 0 && lparam == 0 ) return 0;
	
	union
	{
		LPARAM lparam;
		RECT* rect;
	} params = { lparam };

	/* DefWindowProc must be called in both the maximized and non-maximized
	   cases, otherwise tile/cascade windows won't work */
	RECT nonclient = *params.rect;
	DefWindowProcW( m_hwnd, WM_NCCALCSIZE, wparam, params.lparam );
	//RECT client = *params.rect;

	/* For the non-maximized case, set the output RECT to what it was
		before WM_NCCALCSIZE modified it. This will make the client size the
		same as the non-client size. */
	*params.rect = nonclient;

	return 0;
}

LRESULT n_core::c_window::on_paint( )
{
	if ( m_device == nullptr ) return 0;

	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;
	//| ImGuiWindowFlags_NoResize
	
	ImGui_ImplDX9_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	{
		ImGui::PushStyleColor( ImGuiCol_WindowBg, ImU32( 0x0d1117 ) );
		ImGui::PushStyleColor( ImGuiCol_Border, ImU32( 0xff000000 ) );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 4.f );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 6.f, 4.f ) );
	}

	ImGuiViewport* viewport = ImGui::GetMainViewport( );

	ImGui::SetNextWindowPos( viewport->Pos );
	ImGui::SetNextWindowSize( viewport->Size );

	ImGui::Begin( "main_window", ( bool* )0, flags );
	{
		// this is the top bar region
		{
			// this is the top bar region
			ImGui::PushStyleColor( ImGuiCol_Border, ImU32( 0xff000000 ) );
			ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 2.f );
			ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 2.f, 1.f ) );

			ImGui::BeginChild( "##top" );
			{
				ImGui::Text( "sb2001_ldr/gui" );

				ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 2.f, 1.f ) );
				ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( -1.f, 0.f ) );
				ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 212 / 255.f, 61 / 255.f,  61 / 255.f, 0.95f ) );
				ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.f, 0.f, 0.f, 0.f ) );
				ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
				ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.1f, 0.1f, 0.1f, 0.5f ) );

				auto minimalize_size = ImGui::CalcTextSize( "[-]" );
				auto exit_size = ImGui::CalcTextSize( "[+]" );
			
				ImGui::SameLine( ImGui::GetWindowContentRegionMax( ).x - minimalize_size.x - exit_size.x );
				if ( ImGui::Button( "[-]" ) )
				{
					std::cout << "log: minimalize clicked" << std::endl;		
				}
				ImGui::SameLine( );

				if ( ImGui::Button( "[x]" ) )
				{
					std::cout << "log: exit clicked" << std::endl;
					m_destroy = 1;
				}

				// pop stuff from top bar buttons
				ImGui::PopStyleColor( 4 );
				ImGui::PopStyleVar( 2 );

				ImGui::EndChild( ); // END OF ##TOP 
				
				// pop stuff from "##top"
				ImGui::PopStyleColor( );
				ImGui::PopStyleVar( 2 );
			}

			// this is the top bar separator region
			ImGui::PushStyleColor( ImGuiCol_ChildBg, ImU32( 0xff000000 ) );
			ImGui::SetNextWindowPos( ImVec2( viewport->Pos.x + 1.f, ImGui::GetWindowContentRegionMin( ).y + 16.f ) );
			ImGui::BeginChild( "##top_separator", ImVec2( ImGui::GetWindowContentRegionMax( ).x + 1.f, 2.f ) );
			{
				ImGui::EndChild( ); // END OF ##top_separator
				
				// pop stuff from "##top_separator"
				ImGui::PopStyleColor( );
			}
			
		}
		
		// we calculate time_size here because we need to reserve for
		// for bottom region note: should just make it static it wont change anyway
		//const auto time_size = ImGui::CalcTextSize( "[ 00:00:00 ]" );
		
		// here is region where we "make" view
		{
			ImGui::PushStyleColor( ImGuiCol_Border, ImU32( 0xff000000 ) );
			ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 2.f );
			//ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 6.f, 16.f ) );
			ImGui::SetNextWindowPos( ImVec2( ImGui::GetWindowContentRegionMin( ).x + 2.f,
				ImGui::GetWindowContentRegionMin( ).y + 21.f ) );
			ImGui::BeginChild( "##content_view",
				ImVec2( viewport->WorkSize.x + 2.f,
					ImGui::GetWindowContentRegionMax( ).y
					- ImGui::GetWindowContentRegionMin( ).y - 36.f - 13.f /* size of font here */ ) );
			{
				// add shift from start its fucky
				ImGui::SetCursorPos( ImVec2( ImGui::GetWindowContentRegionMin( ).x,
					ImGui::GetWindowContentRegionMin( ).y + 6.f ) );
				
				// we "make" view here
				// code goes brrr...
				view_make( );

				ImGui::EndChild( ); // END OF ##content_view region

				// pop stuff from "##content_view"
				ImGui::PopStyleColor( );
				ImGui::PopStyleVar( );
			}
			// "make" view region ends
		}

		// this is bottom bar region
		//{
		//	// this is bottom bar separator region
		//	ImGui::PushStyleColor( ImGuiCol_ChildBg, ImU32( 0xff000000 ) );
		//	ImGui::SetNextWindowPos( ImVec2( ImGui::GetCursorPos( ).x, ImGui::GetCursorPos( ).y ) );
		//	ImGui::BeginChild( "##bottom_separator", ImVec2( ImGui::GetContentRegionAvail( ).x, 2.f ) );
		//	{
		//		ImGui::EndChild( ); // END OF ##bottom_separator
		//		
		//		// pop stuff from "##bottom_separator" 
		//		ImGui::PopStyleColor( );
		//	}
		//	
		//	// this is bottom bar text region
		//	ImGui::BeginChild( "##bottom_text", ImVec2( 0, 0 ) );
		//	{
		//		ImGui::Text( "loading..." );

		// make bottom status region
		gui::c_status_bar::get( ).make( );

		//		//auto time_size = ImGui::CalcTextSize( "[ 00:00:00 ]" );

		//		ImGui::SameLine( ImGui::GetWindowContentRegionMax( ).x - time_size.x );
		//		ImGui::Text( "[ %s ]", n_sdk::c_time::now( ).c_str( ) );

		//		ImGui::EndChild( ); // END OF ##bottom_text
		//	}
		//}

		ImGui::End( ); // END OF main_window

		// pop stuff from "main_window"
		ImGui::PopStyleColor( 2 );
		ImGui::PopStyleVar( 2 );
	}
	
	ImGui::EndFrame( ); // END OF IMGUI FRAME

	m_device->SetRenderState( D3DRS_ZENABLE, FALSE );
	m_device->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	m_device->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
	m_device->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff0d1117, 1.0f, 0 );
	m_device->BeginScene( );

	// we call render here
	ImGui::Render( );
	ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
	
	m_device->EndScene( );

	auto present_res = m_device->Present( nullptr, nullptr, nullptr, nullptr );
	if ( present_res == D3DERR_DEVICELOST && m_device->TestCooperativeLevel( ) == D3DERR_DEVICENOTRESET )
		std::cout << "log: lost device" << std::endl;

	return 0;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT n_core::c_window::on_wnd_proc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{	
	if ( HIWORD( lparam ) >= 0 && HIWORD( lparam ) <= 24 && LOWORD( lparam ) <= get( ).m_width - 49 && LOWORD( lparam ) >= 0 )
	{
		if ( msg == WM_LBUTTONDOWN )
		{
			SendMessageW( hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0 );
			return 1;
		}

	}

	if ( msg == WM_PAINT )
	{
		get( ).on_paint( );
		return 2;
	}

	return ImGui_ImplWin32_WndProcHandler( hwnd, msg, wparam, lparam );
}

LRESULT __stdcall n_core::c_window::wnd_proc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	
	switch ( msg )
	{
		case WM_CLOSE:
			DestroyWindow( hwnd );
			return 0;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			return 0;
		
		case WM_DWMCOMPOSITIONCHANGED:
		{
			std::cout << "log: composition changed" << std::endl;
			return 0;
		}

		case WM_NCACTIVATE:
			return DefWindowProcW( hwnd, msg, wparam, -1 );
		
		case WM_NCCALCSIZE:
		{
			//return
				//get( ).on_nccalcsize( wparam, lparam );
				//return DefWindowProcW( hwnd, msg, wparam, lparam );
			break;
		}
		case WM_NCHITTEST:
		{
			return
				get( ).on_nchittest( lparam );
			//break;
		}

		case WM_WINDOWPOSCHANGED:
		{
			get( ).on_position_update( lparam );
			return DefWindowProcW( hwnd, msg, wparam, lparam ); // so WM_SIZE and WM_MOVE dont get fucked :)
			//return 0;
			//break;
		}

		case WM_SIZE:
		{
			if ( get( ).m_device != nullptr )
			{	
				get( ).m_d3d_params.BackBufferWidth = LOWORD( lparam );
				get( ).m_d3d_params.BackBufferHeight = HIWORD( lparam );

				ImGui_ImplDX9_InvalidateDeviceObjects( );
				HRESULT hr = get( ).m_device->Reset( &get( ).m_d3d_params );
				if ( hr == D3DERR_INVALIDCALL )
					IM_ASSERT( 0 );
				ImGui_ImplDX9_CreateDeviceObjects( );
				return 0;
			}
		};
		
	}

	if ( msg == WM_ERASEBKGND ) return 0; //????????????????????

	if ( get( ).on_wnd_proc( hwnd, msg, wparam, lparam ) ) return 0;

	return DefWindowProcA( hwnd, msg, wparam, lparam );
}
