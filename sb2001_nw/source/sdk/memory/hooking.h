#ifndef HOOKING_H
#define HOOKING_H

namespace n_sdk
{
	class c_hooking
	{
	public:
		struct s_hook
		{
			std::string m_hook_name = "f_null";
			
			abyss m_location = nullptr;			
			abyss m_hook = nullptr;
			abyss m_original = nullptr;
		};

		enum class e_hook_action
		{
			disable,
			enable
		};
	
	public:
		static s_hook make_hook( const std::string& hook_name, u32 location, abyss hook, abyss original );
		
		static bool enable( const s_hook& hook );
		static bool disable( const s_hook& hook );
		static bool is_valid( const s_hook& hook );

		static bool do_action( const s_hook& hook, const e_hook_action action );
		
		static bool minhook_start( );
		static bool minhook_end( );
	};

	typedef n_sdk::c_hooking::s_hook hook_t;
	typedef n_sdk::c_hooking::e_hook_action hook_action_t;
}

#endif // HOOKING_H
