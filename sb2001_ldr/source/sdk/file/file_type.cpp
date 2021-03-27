#include <common.h>

#include "file_type.h"

bool n_sdk::c_file_type::is_dll( std::wstring file_path )
{
	return file_path.ends_with( L".dll" );
}
bool n_sdk::c_file_type::is_dll( std::filesystem::path file_path )
{
	return file_path.wstring( ).ends_with( L".dll" );
}

bool n_sdk::c_file_type::is_exe( std::wstring file_path )
{
	return file_path.ends_with( L".exe" );
}
bool n_sdk::c_file_type::is_exe( std::filesystem::path file_path )
{
	return file_path.wstring( ).ends_with( L".exe" );
}
