#ifndef RT_STDMATH
#define RT_STDMATH

#ifdef _MSC_VER
#	define RT_ASM(asm_literal) \
		__asm { \
			asm_literal \
		};
#elif __GNUC__ || __clang__
#	define RT_ASM(asm_literal) \
		"__asm__(\"" \
			#asm_literal \
		"\" : : );"
#endif


float abs(float x);

float sqrt(float x);

float sin(float x);
float acos(float x);

#endif
