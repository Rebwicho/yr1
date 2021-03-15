#include "../_include/common.h"

void on_attach( void* handle )
{


	FreeLibrary( ( HMODULE )handle );
}