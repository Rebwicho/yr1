#ifndef CONSOLE_H
#define CONSOLE_H

namespace n_sdk
{
	class c_console : public c_singleton<c_console>
	{
	public:
		void allocate_console( );
		void free_console( );

		// void set_title( const std::string& console_title );
	
	private:
		void init_streams( );
		void free_streams( );

	private:
		std::string m_console_title = "console";
	};

}

#endif // CONSOLE_H
