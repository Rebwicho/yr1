#ifndef FILE_SEARCH_H
#define FILE_SEARCH_H

namespace n_sdk
{
	class c_file_search
	{
	public:
		// note-to-all: add param to select if we want full file path or just file name
		
		// returns file names of specified path default: current_path
		static std::deque< std::filesystem::path > get_files( std::filesystem::path directory = std::filesystem::current_path( ) );

		// returns all dll files from path we specify default: current_path
		static std::deque< std::filesystem::path > get_dll_files( std::filesystem::path directory = std::filesystem::current_path( ) );

		// returns all exe files from path we specify default: current_path
		static std::deque< std::filesystem::path > get_exe_files( std::filesystem::path directory = std::filesystem::current_path( ) );

	};
}

#endif // FILE_SEARCH_H
