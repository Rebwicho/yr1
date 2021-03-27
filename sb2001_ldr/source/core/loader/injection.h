#ifndef INJECTION_H
#define INJECTION_H

namespace n_core
{
	class c_injection : public c_singleton< c_injection >
	{
	public:
		enum e_injection_mode
		{
			manualmap,
			// ...

			invalid
		};
	
	public:
		// set injectable
		// set inject mode
		// inject

		// set injectable for injection
		void set( const n_core::injectable_t  injectable );

		// set injection mode for our injectable
		void set_mode( e_injection_mode injection_mode );

		// execute our injection
		bool execute( );

	private:
		bool is_valid_injectable( );
		bool is_valid_mode( );

	private:
		n_core::injectable_t m_injectable = { };
		e_injection_mode m_injection_mode = invalid;
	
	};

	typedef n_core::c_injection::e_injection_mode injection_mode_t;
}

#endif // INJECTION_H
