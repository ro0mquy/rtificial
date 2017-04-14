#ifdef _WINDOWS

#ifndef RT_STDLIB
#define RT_STDLIB

void* __cdecl operator new(unsigned int bytes);
void* __cdecl operator new[](unsigned int bytes);
void* __cdecl operator new(unsigned int bytes, void* here);

void __cdecl operator delete(void* ptr);
void __cdecl operator delete[](void* ptr);
void __cdecl operator delete(void* ptr, unsigned int);


typedef void(__cdecl *ePVFV)();

void memset2(void* dst, unsigned char val, unsigned int count);

#define MAX_ATEXITS 32
static void initTerm(ePVFV *pfbegin, ePVFV *pfend);
static void initAtExit();
static void doAtExit();
int __cdecl atexit(ePVFV func);

extern "C" int _cdecl _purecall(void);

void GlobalsStaticsInit();
void GlobalsStaticsFree();

#endif

#endif