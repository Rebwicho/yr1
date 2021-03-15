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
		};
		
	public:
		c_pattern_module( u32 start_address )
			: m_start_addres( start_address )
		{
			printf( "[ c_pattern_module ]: created, start_address: %#x\n", this->m_start_addres );
		}

		~c_pattern_module( )
		{
			for ( u32 i = 0; i < this->patterns_queue.size(  ); i++ )
			{
				printf( "[ c_pattern_module!%#x ]: deleting %s\n", this->m_start_addres, this->patterns_queue[ i ]->name.c_str( ) );
				delete this->patterns_queue[ i ]; // clear pattern queue
			}
			
			printf( "[ c_pattern_module ]: deleted\n" );
		}

	public:
		s_pattern* pat_call( std::string name, std::deque< BYTE > pattern, u32 offset );
		s_pattern* pat_pure( std::string name, std::deque< BYTE > pattern, u32 offset );

		bool process_queue( );
		//bool execute( );

	private:
		std::deque< s_pattern* > patterns_queue;

		u32 m_start_addres = 0;
	};
}

#endif // PATTERN_H
