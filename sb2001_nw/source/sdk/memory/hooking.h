#ifndef HOOKING_H
#define HOOKING_H

namespace n_sdk
{
	class c_hooking
	{
	public:
		struct s_hook
		{
		public:
			s_hook( ) = delete;
			s_hook( std::string hook_name, __int32 func_address, void* func_hook, void* original = nullptr )
				: m_hook_name( hook_name ), m_func_address( func_address ), m_func_hook( func_hook ), m_func_orig( original )
			{
				printf( "[ c_memory::hooking ]: registered hook [ %s, %#x ]\n", this->m_hook_name.c_str( ), this->m_func_address );
			}

		public:
			bool execute( );
			/*{
				printf( "[ c_memory::hooking ]: hooking %s at %#x ", this->m_hook_name.c_str( ), this->m_func_address );

				auto create_hook = MH_CreateHook( ( void* )this->m_func_address, this->m_func_hook, reinterpret_cast< LPVOID* >( this->m_func_orig ) );
				if ( create_hook != MH_OK )
				{
					printf( "failed to create hook\n" );

					return 0;
				}

				auto enable_hook = MH_EnableHook( ( void* )this->m_func_address );
				if ( enable_hook != MH_OK )
				{
					printf( "failed to enable hook\n" );

					return 0;
				}

				printf( "done -> f_hook: %#x f_orig: %#x\n", this->m_func_hook, ( void* )this->m_func_orig );

				return 1;
			}*/

		private:
			std::string m_hook_name = "f_null";

			__int32 m_func_address = 0;

			void* m_func_hook = nullptr;
			void* m_func_orig = nullptr;
		};
		
	public:
		// c_hooking( );
		// ~c_hooking( );
	
	public:
		void register_hook( s_hook hook );

		u32 hook_all( );
		u32 unhook_all( );
		
		bool init_mh( );
		bool free_mh( );

	private:
		std::deque< s_hook > m_hook_queue = { };
	};
}

#endif // HOOKING_H
