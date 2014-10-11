#include "stdmath.h"

float abs(float x) {
	__asm {
		fld     dword ptr[x]
		fabs
		fstp    dword ptr[x]
	}

	return x;
}


float sqrt(float x) {
	__asm {
		fld     dword ptr[x]
		fsqrt
		fstp    dword ptr[x]
	}

	return x;
}


float sin(float x) {
	__asm {
		fld     dword ptr[x]
		fsin
		fstp    dword ptr[x]
	}

	return x;
}

float acos(float x) {
	__asm {
		fld     dword ptr[x]
		fld     st(0)
		fld     st(0)
		fmul
		fld1
		fsubr
		fsqrt
		fxch
		fpatan
		fstp    dword ptr[x]
	}

	return x;
}
