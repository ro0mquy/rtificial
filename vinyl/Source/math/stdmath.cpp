#include "stdmath.h"

float abs(float x) {
#ifdef _MSC_VER
	__asm {
		fld x
		fabs
		fstp x
	}
#elif __GNUC__ || __clang__
	asm("fabs" : "+t" (x));
#endif
	return x;

	/*
	RT_ASM(fld     dword ptr[x]);
	RT_ASM(fabs);
	RT_ASM(fstp    dword ptr[x]);

	return x;
	// */
}

float sqrt(float x) {
#ifdef _MSC_VER
	__asm {
		fld x
		fsqrt
		fstp x
	}
#elif __GNUC__ || __clang__
	asm("fsqrt" : "+t" (x));
#endif
	return x;

	/*
	RT_ASM(fld     dword ptr[x]);
	RT_ASM(fsqrt);
	RT_ASM(fstp    dword ptr[x]);

	return x;
	// */
}


float sin(float x) {
#ifdef _MSC_VER
	__asm {
		fld x
		fsin
		fstp x
	}
#elif __GNUC__ || __clang__
	asm("fsin" : "+t" (x));
#endif
	return x;

	/*
	RT_ASM(fld     dword ptr[x]);
	RT_ASM(fsin);
	RT_ASM(fstp    dword ptr[x]);

	return x;
	// */
}

float acos(float x) {
	//*
#ifdef _MSC_VER
	__asm {
		fld x
		fld st(0)
		fld st(0)
		fmul
		fld1
		fsubr
		fsqrt
		fxch
		fpatan
		fstp x
	}
#elif __GNUC__ || __clang__
	asm(R"assembly(

		fld %%st(0)
		fmul %%st(0), %%st(0)
		fld1
		fsubp %%st(0), %%st(1)
		fsqrt
		fxch %%st(1)
		fpatan

		)assembly" : "+t" (x));
#endif
	return x;
	// */

	/*
	float in_param = x;
	RT_ASM(fld     dword ptr[in_param]);
	RT_ASM(fld     st(0));
	RT_ASM(fld     st(0));
	RT_ASM(fmul);
	RT_ASM(fld1);
	RT_ASM(fsubr);
	RT_ASM(fsqrt);
	RT_ASM(fxch);
	RT_ASM(fpatan);
	RT_ASM(fstp    dword ptr[in_param]);
	x = in_param;

	return x;
	// */
}
