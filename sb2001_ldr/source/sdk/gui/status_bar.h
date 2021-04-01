#ifndef STATUS_BAR_H
#define STATUS_BAR_H

namespace gui
{
	// lets make it with singleton for now so we can access it in views
	class c_status_bar : public c_singleton< c_status_bar >
	{
	public:
		void set( const char* message );
		void make( );

	private:
		std::string m_message = "starting...";
	};
}

#endif // STATUS_BAR_H
