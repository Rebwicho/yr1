#ifndef ANTI_DEBUG_H
#define ANTI_DEBUG_H

namespace n_game
{
	namespace n_security
	{
		class c_anti_debug : public c_singleton< c_anti_debug >
		{
		private:
			std::deque< u8 > m_original_bytes = { }; //	store original bytes
			u32 m_patch_location = 0x0; //				location where function do exit on debugger detection
			u32 m_sleep_location = 0x0; //				location where function do sleep when there is no debugger detected
			u32 m_func_location = 0x0; //				location where function is in memory

		public:
			// c_anti_debug( );
			// ~c_anti_debug( );

		public:
			void set_function( u32 location );
			void restore( );
			bool patch( );

		private:
			void store_original_bytes( ); //		saves original bytes of patch location to restore later
			bool find_patch_location( ); //			finds location for patching
			bool find_sleep_call_location( ); //	finds location where we will jump to
			u8 calculate_jnz( ); //					calculates jnz

			u32 byte_search( std::deque< u8 > bytes_to_find ); // returns location of bytes that we looking for
		};
		
	}
}

#endif // ANTI_DEBUG_H
