#ifndef PATTERN_H
#define PATTERN_H

namespace n_sdk
{
	/* hash pattern scanner */
	class c_pattern_module
	{
	private:
		struct s_pattern
		{
			s_pattern( std::string a0, std::deque< BYTE > a1, u32 a2, bool pure, bool a4 )
			{
				pattern = a1;
				offset = a2;
				is_pure = pure;
				cur_pattern = a1;
				find_one = a4;
				first_match = 0;
				name = a0;
			};

			std::string name = "";
			u32 res = 0;

			std::deque< BYTE > pattern;

			std::deque< BYTE > found_bytes;
			std::deque< BYTE > cur_pattern;

			u32 first_match = 0;
			u32 offset = 0;
			bool is_pure = 0;
			bool find_one = 0;

			std::function< bool( ) > m_f_on_end = std::function< bool( ) >();
		};
		
	public:
		c_pattern_module( u32 module_base )
			: m_module_base( module_base ) { }

		//c_pattern_module( const std::string& module_name )
		//	: m_module_base( handle_name( module_name ) ) { }
		
		c_pattern_module( const std::string& module_name )
			: m_module_name( module_name ) { }
	public:
		s_pattern& pat_call( std::string name, std::deque< BYTE > pattern, u32 offset );
		s_pattern& pat_pure( std::string name, std::deque< BYTE > pattern, u32 offset );

		u32 process_queue( );
		//bool execute( );

		auto get_module_name( ) -> std::string;
		auto get_module_name_pure( ) -> std::string;

		auto get_module_base( ) -> u32;

		auto get_queue( ) -> std::deque< s_pattern >&;

		auto set_module_base( const u32& module_base ) -> void;

		bool on_found( );
	
	private:
		//u32 handle_name( const std::string& module_name )
		//{
		//	this->m_module_name = module_name;
		//	return 0; // ( u32 )GetModuleHandleA( this->m_module_name.c_str( ) )
		//}
	
	private:
		std::deque< s_pattern > m_patterns_queue = { };

		std::string m_module_name = "";
		bool m_found_module = 0;
		
		u32 m_module_base = 0;
	};
}

#endif // PATTERN_H
