#ifndef MEMORY_H
#define MEMORY_H

namespace n_offset
{
	struct s_offsets
	{
		u32 f_anti_debug = 0;
	};
}

typedef n_offset::s_offsets offsets_t;
typedef n_sdk::c_hooking hooking_t;

namespace n_core
{	
	class c_memory : public c_singleton<c_memory>
	{
	public:
		c_memory( )
		{
			printf( "[ c_memory ]: spawned\n" );
		}
		
		~c_memory( )
		{
			printf( "[ c_memory ]: destructing\n" );
		}

	public:
		bool get_offsets( );
		
		bool hook( );
		bool unhook( );

	public:
		offsets_t offset;
		hooking_t hooking;
	
	protected:
		bool write_offset( u32 src, u32& dst, const std::string& offset_name = "null" );
		
		void add_pattern_module( n_sdk::c_pattern_module* pattern_module );
		void process_pattern_modules( );
		void cleanup_pattern_queue( );
	
	private:
		std::deque< n_sdk::c_pattern_module* > m_pattern_module_q = { };
	};
}

#endif // MEMORY_H
