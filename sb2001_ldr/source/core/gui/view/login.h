#ifndef LOGIN_H
#define LOGIN_H

namespace gui::view
{
	class c_login final : public n_sdk::c_view, public c_singleton< c_login >
	{
	public:
		void make( ) override;
		bool is_fulfilled( ) override;

	public:
		void on_login( );
	
	private: /* define controls here */
		controls::input_str m_ilogin;
		controls::input_str m_ipassword;
		
		controls::button m_bsend;

	private:
		bool m_is_fulfilled = 0;
	};
}

#endif // LOGIN_H
