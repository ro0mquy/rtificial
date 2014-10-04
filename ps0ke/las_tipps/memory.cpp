#include "memory.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

#ifdef HG_INTRO

void* __cdecl operator new(uint bytes) {
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bytes);
}

void* __cdecl operator new [](uint bytes) {
	return operator new(bytes);
}

void* __cdecl operator new(uint bytes, void* here ) {
    return here;
}

void __cdecl operator delete(void* ptr) {
	if(ptr) HeapFree(GetProcessHeap() , 0 ,ptr);
}

void __cdecl operator delete [](void* ptr) {
	operator delete(ptr);
}


extern "C" int _cdecl _purecall(void)
{
  return 0;
}

void* __cdecl memset(void* p, int v, size_t n) {
	char* cp = (char*)p;
	for(size_t i = 0; i < n; i++) {
		cp[i] = v;
	}
	return p;
}

void* __cdecl memcpy(void* dest, const void* src, size_t n) {
	//for(size_t i=n-1; i>=0; i--) {
	for (uint i = 0; i < n; ++i) {
		((char*)dest)[i] = ((const char*)src)[i];
	}

	return dest;
}


unsigned int strlen( const char *str ){
	const char *str0 = str;
	while( *str ){ str++; }
	return long( str )-long( str0 );
}

int strcmp( const char *str1, const char *str2 ){
	char* s1=( char* )str1;
	char* s2=( char* )str2;
	while( *s1==*s2 ){
		s1++;
		s2++;
	}
	if( ( *s1 == 0 ) && ( *s2 == 0 ) ){ return 0; }
	if( *s1>*s2 ){ return 1; }
	return -1;
}

#endif