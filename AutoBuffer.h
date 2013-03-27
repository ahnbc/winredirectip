#ifndef _H_AUTO_BUFF
#define _H_AUTO_BUFF
#include <memory>
template <typename T>
class CAutoBuffer:public std::auto_ptr<T>
{
public:
	explicit CAutoBuffer(){};
	explicit CAutoBuffer(int size):std::auto_ptr<T>(new T[size]){};
	void reset(int size){std::auto_ptr<T>::reset(new T[size]);};
	template <typename T2>
	operator T2* () { return  reinterpret_cast< T2*>(get());};
	T& operator [] (size_t post) { return  *(get()+post);};
	T operator [] (size_t post) const { return  *(get()+post);};
	T& operator [] (int post) { return  *(get()+post);};
	T operator [] (int post) const { return  *(get()+post);};
	T * operator + (int post){return (get()+post);};
	T * operator - (int post){return (get()-post);};
};

#endif