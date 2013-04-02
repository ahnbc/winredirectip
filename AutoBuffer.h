#ifndef _H_AUTO_BUFF
#define _H_AUTO_BUFF
#include <memory>
#include <cassert>
template <typename T>

class CAutoBuffer:public std::auto_ptr<unsigned char>
{
public:
		explicit CAutoBuffer(){};
		explicit CAutoBuffer(int size):std::auto_ptr<unsigned char>(new unsigned char[size*sizeof(T)]){};
		void reset(int size){std::auto_ptr<unsigned char>::reset(new unsigned char[size*sizeof(T)]);};
		void clear(int size){memset(get(),0,size*sizeof(T));};
		template <typename T2>
			operator T2* () { return  reinterpret_cast<T2*>((T2*)get());};
		T& operator [] (size_t post) {assert(post>=0); return  *(reinterpret_cast<T *>(get())+post);};
		T operator [] (size_t post) const {assert(post>=0); return  *(reinterpret_cast<T *>(get())+post);};
		T& operator [] (int post) {assert(post>=0); return  *(reinterpret_cast<T *>(get())+post);};
		T operator [] (int post) const { assert(post>=0);return  *(reinterpret_cast<T *>(get())+post);};
		T * operator + (int post){assert(post>=0);return (reinterpret_cast<T *>(get())+post);};
		T * operator - (int post){assert(post>=0);return (reinterpret_cast<T *>(get())-post);};
};

#endif