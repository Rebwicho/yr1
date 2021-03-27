#ifndef PROCESS_H
#define PROCESS_H

#include <BlackBone/BlackBone/Process/Process.h>

#ifdef _DEBUG
#pragma comment(lib, "BlackBone/BlackBone-DBG.lib")
#else
#pragma comment(lib, "BlackBone/BlackBone.lib")
#endif

namespace n_sdk
{
	class c_process
	{
	public:
		// c_process( );
		// ~c_process( );

	public:
		bool prepare( const std::wstring process_name ); // set process by name

		/* injection section */
		bool manual_map( const std::wstring file_path ); // manual map injection

		u32 get_module( std::wstring module_name );
	
	public: 
		static u32 get_pid( std::wstring process_name );

	private:
		blackbone::Process m_process_obj;
		
		std::wstring m_target_process_name = L"";
		u32 m_target_pid = 0;
	};
	
}

#endif // PROCESS_H
