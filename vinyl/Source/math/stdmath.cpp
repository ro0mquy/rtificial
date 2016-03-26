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

float log10(float x)
{
    __asm
    {
        fld1
        fld     dword ptr [x]
        fyl2x
        fldl2t
        fdiv
        fstp    dword ptr [x]
    }
    return x;
}

double pow(double a, double b)
{
  __asm
  {
    fld     qword ptr [b]
    fld     qword ptr [a]

    ftst
    fstsw   ax
    sahf
    jz      zero

    fyl2x
    fist    dword ptr [a]
    sub     esp, 12
    mov     dword ptr [esp], 0
    mov     dword ptr [esp+4], 0x80000000
    fisub   dword ptr [a]
    mov     eax, dword ptr [a]
    add     eax, 0x3fff
    mov     [esp+8], eax
    jle     underflow
    cmp     eax, 0x8000
    jge     overflow
    f2xm1
    fld1
    fadd
    fld     tbyte ptr [esp]
    add     esp, 12
    fmul
    jmp     end

underflow:
    fstp    st
    fldz
    add     esp, 12
    jmp     end

overflow:
    push    0x7f800000
    fstp    st
    fld     dword ptr [esp]
    add     esp, 16
    jmp     end

zero:
    fstp    st(1)

end:
  }
}

float exp(float x)
{
    __asm
    {
        fld     dword ptr [x]
        fldl2e
        fmulp   st(1), st
        fld1
        fld     st(1)
        fprem
        f2xm1
        faddp   st(1), st
        fscale
        fstp    st(1)
        fstp    dword ptr [x]
    }

    return x;
}
