// provide stlib implementations
#ifdef NDEBUG

#ifdef _WINDOWS
#include <Windows.h>

#define assert(x)

#ifdef ALWAYS_USE_STB_VORBIS_ALLOC
#define alloca(a)    0  // and don't use the alloca() path!
#else  // use malloc/free instead of alloca
#define alloca(a)    malloc(a)  // this is actually the macro above
#define dealloca(p)  free(p)    // this is actually the macro above
#endif

static void *malloc(int bytes){
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bytes);
}

static void free(void *ptr){
	if (ptr) HeapFree(GetProcessHeap(), 0, ptr);
}

static void *realloc(void *ptr, size_t size) {
	if (ptr) HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ptr, size);
	return NULL;
}


#define pow rt_pow
/*
// http://wurstcaptures.untergrund.net/assembler_tricks.html#fastpowf
// hopefully not too inacurate for audio
static float rt_pow(float a, float b){
	return (a / (b - a * b + a));
}
*/

// brain control code

// optimizations are breaking this one in release mode
#pragma optimize("", off)

// faster pow based on code by agner fog
// code taken from chaos/farbrausch
static double ePow64(double a, double b){
	__asm {
		fld     qword ptr[b]
		fld     qword ptr[a]

		ftst
		fstsw   ax
		sahf
		jz      zero

		fyl2x
		fist    dword ptr[a]
		sub     esp, 12
		mov     dword ptr[esp], 0
		mov     dword ptr[esp + 4], 0x80000000
		fisub   dword ptr[a]
		mov     eax, dword ptr[a]
		add     eax, 0x3fff
		mov[esp + 8], eax
		jle     underflow
		cmp     eax, 0x8000
		jge     overflow
		f2xm1
		fld1
		fadd
		fld     tbyte ptr[esp]
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
			fld     dword ptr[esp]
			add     esp, 16
			jmp     end

		zero:
		fstp    st(1)

		end:
	}
}

#pragma optimize("", on)

static float rt_pow(float base, float exp){
	return (float)ePow64(base, exp);
}


#define floor rt_floor
/*
// self made, hopefully not wrong
static float rt_floor(float x) {
	if (x >= 0){
		return (float)((int)x);
	}
	else {
		return (float)((-1.0)*((int)x*(-1)) - 1.0);
	}
}
*/
static float rt_floor(float x){
	int holder, setter, res;

	__asm {
		fld     dword ptr[x]
		fnstcw  dword ptr[holder]
		movzx   eax, [holder]
		and     eax, 0xfffff3ff
		or      eax, 0x00000400
		mov     dword ptr[setter], eax
		fldcw   dword ptr[setter]
		fistp   dword ptr[res]
		fldcw   dword ptr[holder]
	}

	return (float) res;
}

static float abs(float x){
	if (x >= 0){
		return x;
	}
	else {
		return (-1)*x;
	}
}

// http://stackoverflow.com/questions/2938966/how-to-use-vc-intrinsic-functions-w-o-run-time-library
#pragma function(memset)
static void* __cdecl memset(void *base, int val, size_t len) {
	size_t i;
	for (i = 0; i < len; ++i) ((char *)base)[i] = val;
	return NULL;
}

#define memcmp rt_memcmp
static int rt_memcmp(void *p1, void *p2, int len) {
	unsigned char *q1 = (unsigned char *)p1;
	unsigned char *q2 = (unsigned char *)p2;
	int i;
	for (i = 0; i < len; ++i)
		if (q1[i] < q2[i]) return -1;
		else if (q1[i] > q2[i]) return 1;
		return 0;
}

#define memcpy rt_memcpy
static void rt_memcpy(void *dest, void *src, int num) {
	int i;
	for (i = 0; i < num; ++i)
		((char *)dest)[i] = ((char *)src)[i];
}

static float ldexp(float Value, int Exponent) {
	return((float)((double)Value * pow(2.0, (double)Exponent)));
}

/*
 * simple and clean quicksort
 * http://insanecoding.blogspot.de/2007/03/quicksort.html
 */
 static void swap_internal(char *a, char *b, size_t size){
	if (a != b)	{
		char t;
		while (size--) {
			t = *a;
			*a++ = *b;
			*b++ = t;
		}
	}
}
static void qsort_internal(char *begin, char *end, size_t size, int(*compar)(const void *, const void *)){
	if (end > begin) {
		char *pivot = begin;
		char *l = begin + size, *r = end;

		while (l < r) {
			if (compar(l, pivot) <= 0) {
				l += size;
			} else {
				r -= size;
				swap_internal(l, r, size);
			}
		}
		l -= size;
		swap_internal(begin, l, size);
		qsort_internal(begin, l, size, compar);
		qsort_internal(r, end, size, compar);
	}
}
static void qsort(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *)) {
	qsort_internal((char *)base, (char *)base + nmemb*size, size, compar);
}

#define exp rt_exp
static float rt_exp(float x){
	__asm {
		fld     dword ptr[x]
		fldl2e
		fmulp   st(1), st
		fld1
		fld     st(1)
		fprem
		f2xm1
		faddp   st(1), st
		fscale
		fstp    st(1)
		fstp    dword ptr[x]
	}
	return x;
}

#define log rt_log
static float rt_log(float x) {
	__asm {
		fld1
		fld     dword ptr[x]
		fyl2x
		fldl2e
		fdiv
		fstp    dword ptr[x]
	}

	return x;
}

#define sin rt_sin
static float rt_sin(float x) {
	__asm {
		fld     dword ptr[x]
		fsin
		fstp    dword ptr[x]
	}

	return x;
}

#define cos rt_cos
static float rt_cos(float x) {
	__asm {
		fld     dword ptr[x]
		fcos
		fstp    dword ptr[x]
	}
	return x;
}

#endif

#ifdef __linux
#endif

#endif

#include "stb_vorbis.c"

