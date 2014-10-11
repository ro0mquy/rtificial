#include "stdmath.h"

float abs(float x) {
	RT_ASM(fld     dword ptr[x]);
	RT_ASM(fabs);
	RT_ASM(fstp    dword ptr[x]);

	return x;
}

float sqrt(float x) {
	RT_ASM(fld     dword ptr[x]);
	RT_ASM(fsqrt);
	RT_ASM(fstp    dword ptr[x]);

	return x;
}


float sin(float x) {
	RT_ASM(fld     dword ptr[x]);
	RT_ASM(fsin);
	RT_ASM(fstp    dword ptr[x]);

	return x;
}

float acos(float x) {
	RT_ASM(fld     dword ptr[x]);
	RT_ASM(fld     st(0));
	RT_ASM(fld     st(0));
	RT_ASM(fmul);
	RT_ASM(fld1);
	RT_ASM(fsubr);
	RT_ASM(fsqrt);
	RT_ASM(fxch);
	RT_ASM(fpatan);
	RT_ASM(fstp    dword ptr[x]);

	return x;
}
