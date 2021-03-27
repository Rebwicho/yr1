#include <common.h>

#include "console.h"

void n_sdk::c_console::allocate_console( )
{
	AllocConsole( );

	SetConsoleTitleA( this->m_console_title.c_str( ) );

	this->init_streams( );
}

void n_sdk::c_console::free_console( )
{
	this->free_streams( );
	FreeConsole( );
}

void n_sdk::c_console::init_streams( )
{
	//freopen_s( ( FILE** )stderr, "conout$", "w", stderr );
	//freopen_s( ( FILE** )stdout, "conout$", "w", stdout );
	//freopen_s( ( FILE** )stdin, "conin$", "r", stdin );

	freopen( "conout$", "w", stderr );
	freopen( "conout$", "w", stdout );
	freopen( "conin$", "r", stdin );
	
	HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );
	DWORD dwMode = 0; GetConsoleMode( hOut, &dwMode );
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode( hOut, dwMode );
}

void n_sdk::c_console::free_streams( )
{
	fclose( stderr );
	fclose( stdout );
	fclose( stdin );
}