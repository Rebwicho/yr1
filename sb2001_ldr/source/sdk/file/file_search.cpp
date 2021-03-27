#include <common.h>
#include <sdk.h>

#include "file_search.h"

std::deque< std::filesystem::path > n_sdk::c_file_search::get_files( std::filesystem::path directory )
{
	auto files = std::deque< std::filesystem::path >( );

	for ( auto& file : std::filesystem::directory_iterator( directory ) )
		files.push_back( file.path( ) );
	
	return files;
}

std::deque< std::filesystem::path > n_sdk::c_file_search::get_dll_files( std::filesystem::path directory )
{
	auto files = std::deque< std::filesystem::path >( );

	for ( auto& file : std::filesystem::directory_iterator( directory ) )
		if ( n_sdk::c_file_type::is_dll( file ) == 1 )
			files.push_back( file.path( ) );

	return files;
}

std::deque< std::filesystem::path > n_sdk::c_file_search::get_exe_files( std::filesystem::path directory )
{
	auto files = std::deque< std::filesystem::path >( );

	for ( auto& file : std::filesystem::directory_iterator( directory ) )
		if ( n_sdk::c_file_type::is_exe( file ) == 1 )
			files.push_back( file.path( ) );
	
	return files;
}
