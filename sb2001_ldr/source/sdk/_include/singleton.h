#ifndef SINGLETON_H
#define SINGLETON_H

template<typename owner_type_t>
class c_singleton
{
public:
	static owner_type_t& get( )
	{
		static owner_type_t instance;
		return instance;
	}

private:
	c_singleton( c_singleton&& ) = delete;
	c_singleton( c_singleton const& ) = delete;
	c_singleton& operator=( c_singleton const& ) = delete;

protected:
	c_singleton( ) { }
	~c_singleton( ) { }
};

#endif // SINGLETON_H
