#ifdef _WINDOWS

#include "stdlib.h"
#include <windows.h>

void* __cdecl operator new(unsigned int bytes) {
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bytes);
}

void* __cdecl operator new [](unsigned int bytes) {
	return operator new(bytes);
}

void* __cdecl operator new(unsigned int bytes, void* here) {
	return here;
}

void __cdecl operator delete(void* ptr) {
	if (ptr) HeapFree(GetProcessHeap(), 0, ptr);
}

void __cdecl operator delete [](void* ptr) {
	operator delete(ptr);
}


#pragma data_seg(".CRT$XCA")
ePVFV __xc_a[] = { nullptr };
#pragma data_seg(".CRT$XCZ")
ePVFV __xc_z[] = { nullptr };
#pragma data_seg() // reset data segment


void memset2(void* dst, unsigned char val, unsigned int count) {
	__asm {
		mov     eax, dword ptr[val]
		mov     ecx, dword ptr[count]
		mov     edi, dword ptr[dst]
		rep     stosb
	}
}


static void initTerm(ePVFV *pfbegin, ePVFV *pfend) {
	while (pfbegin < pfend) {
		if (*pfbegin) {
			(**pfbegin)();
		}

		pfbegin++;
	}
}

static ePVFV g_atExitList[MAX_ATEXITS];

static void initAtExit() {
	memset2(g_atExitList, 0, sizeof(g_atExitList));
}

static void doAtExit() {
	initTerm(g_atExitList, g_atExitList + MAX_ATEXITS);
}

int __cdecl atexit(ePVFV func) {
	// get next free entry in atexist list
	unsigned int index = 0;
	while (g_atExitList[index++]);

	if (index < MAX_ATEXITS) {
		g_atExitList[index] = func;
		return 0;
	}

	return -1;
}


extern "C" int _cdecl _purecall(void) {
	return 0;
}


void GlobalsStaticsInit() {
#	ifdef NDEBUG
	initAtExit();
	initTerm(__xc_a, __xc_z);
#	endif
}

void GlobalsStaticsFree() {
#	ifdef NDEBUG
	doAtExit();
#	endif
}

#endif