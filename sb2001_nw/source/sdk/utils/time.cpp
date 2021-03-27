#include <common.h>

#include "./time.h"

const std::string n_sdk::c_time::now( )
{
	std::time_t t = std::time( nullptr );
	auto time_now = std::localtime( &t );
	std::stringstream out; out << time_now->tm_hour << ':' << time_now->tm_min << ':' << time_now->tm_sec;
	return out.str( );
}
