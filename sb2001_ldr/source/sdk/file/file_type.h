#ifndef FILE_TYPE_H
#define FILE_TYPE_H

namespace n_sdk
{
	class c_file_type
	{
	public:
		static bool is_dll( std::wstring file_path );
		static bool is_dll( std::filesystem::path file_path );

		static bool is_exe( std::wstring file_path );
		static bool is_exe( std::filesystem::path file_path );
	};
}

#endif // FILE_TYPE_H
