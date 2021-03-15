#ifndef CONSOLE_H
#define CONSOLE_H

namespace n_sdk
{
	class c_console : public c_singleton<c_console>
	{
	public:
		c_console( );
		//c_console( std::string console_title );

		~c_console( );

	public:
		void allocate_console( );
		void free_console( );

	private:
		void init_streams( );
		void free_streams( );

	private:
		std::string m_console_title = "console";
	};

	inline c_console::c_console( )
	{
		this->allocate_console( );
		printf( "[ c_console ]: spawn\n" );
	}

	//inline c_console::c_console( std::string console_title )
	//	: m_console_title( console_title )
	//{
	//	this->allocate_console( );
	//	printf( "[ c_console ]: spawn\n" );
	//}

	inline c_console::~c_console( )
	{
		//printf( "[ c_console ]: destructing\n" );
		this->free_console( );
	}
}

#endif // CONSOLE_H
