#ifndef ENUM_FILE_H
#define ENUM_FILE_H

namespace n_sdk::n_enum
{
	enum e_state
	{
		started,
		waiting,
		failed,
		success
	};
}

namespace n_sdk
{
	typedef n_sdk::n_enum::e_state state_t;
}

#endif // ENUM_FILE_H
