#ifndef VIEW_H
#define VIEW_H

namespace n_sdk
{
	// this is pure virtual class every view class need this
	// as base 
	class c_view 
	{
	protected:
		~c_view( ) = default;
	
	public:
		// virtual void action( /* */ );
		//virtual void pre_make( ) = 0; // takes stuff from the stack and executes it according
		// to action it should take on that stuff
		// like: wrong login or pass so we send to this view that server responded negative to
		//		 login request and we log somewhere that credentials were wrong? or show some info popup /shrug
		
		virtual void make( ) = 0;
		//  virtual bool is_exception( ) = 0; // this function return 1 if
		// for example: client tried to login to his account but
		//				server responded with negative that says account is banned
		//				so we terminate process or do some action related to it
		//				like: unknown error during injection? so we send to server logs ???
		//				or: 
		virtual bool is_fulfilled( ) = 0; // this function returns if a view did its job
		// for example: login view returns 1 when server responded positive to login request
		//				with client credentials
		//		   2nd: settings window returns 1 when save or close button was clicked
		//		   etc...

		// ideas:
		//	- view can have popup and returns bool etc?
		//	  like: client was in settings view changed something
		//			then he clicked close so we render popup window
		//			with info "you have unsaved new settings do you want to discard them?"
		//			or smth xd
		//	  etc...
	};
}

#endif // VIEW_H
