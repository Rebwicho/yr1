#ifndef INJECTABLES_H
#define INJECTABLES_H

namespace n_core
{
	class c_injectables : public c_singleton< c_injectables >
	{
		/* later on this class can do a lot more checks if needed
		 * like if dll i valid pe extract some info for printing
		 * and even download files from server
		 *
		 */
	
	private:
		std::array< std::wstring, 2 > m_process_names = {
			L"NostaleClientX.exe",
			L"NosWings.exe"
		};

		std::array< std::wstring, 2 > m_supported_injectables = {
			L"nt",
			L"nw"
		};
		// todo: ^connect those two strings into struct/class/tuple...

	public:
		enum e_injectable_type
		{
						/*	publisher/owner		game		process name  */
			nostale_gf, //	- gameforge			nostale		process: NostaleClientX.exe
			nostale_nw, //	- noswings-project	nostale		process: NosWings.exe


			invalid
		};

		struct s_injectable
		{
			std::wstring m_process_name;
			std::wstring m_injectable_name;
			std::filesystem::path m_injectable_file_path;
			e_injectable_type m_injectable_type = invalid;
			// ...
		};

	public:
		// scanning for dll's
		// add dll to injectables list if dll is an injectable :0)
		// we choose what injectable we want to use
		// scan for proper process that chosen injectable needs
		// inject if found

		void scan( std::filesystem::path file_path = std::filesystem::current_path( ) );
		
		s_injectable get_injectable( u32 injectable_id );
	
	private:
		std::wstring get_injectable_process( e_injectable_type injectable_type ); /*	returns proper process name for
																						our injectable					 */
		e_injectable_type get_injectable_type( std::filesystem::path file_path ); // returns injectable type

	private:
		std::deque< s_injectable > m_injectables = { };
		
	};

	typedef n_core::c_injectables::e_injectable_type injectable_type_t;
	typedef n_core::c_injectables::s_injectable injectable_t;
	
}

#endif // INJECTABLES_H
