#include <common.h>

#include "./time.h"

const std::string n_sdk::c_time::now( )
{
	std::time_t t = std::time( nullptr );
	auto time_now = std::localtime( &t );

	std::string formated_time;
	std::sprintf( formated_time.data( ), "%.2d:%.2d:%.2d", time_now->tm_hour, time_now->tm_min, time_now->tm_sec );
	return formated_time;
}
