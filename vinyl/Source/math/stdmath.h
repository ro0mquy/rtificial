#ifndef RT_STDMATH
#define RT_STDMATH

#ifdef _MSC_VER
#	define RT_ASM(asm_literal) \
		__asm { \
			asm_literal \
		};
#elif __GNUC__ || __clang__
#	define RT_ASM(asm_literal) \
		__asm__(".intel_syntax noprefix \n" \
			#asm_literal \
				"\n .att_syntax prefix" \
		 : : );
#endif


float abs(float x) noexcept;

float sqrt(float x) noexcept;

float sin(float x) noexcept;
float acos(float x) noexcept;

float log10(float x) noexcept;
double pow(double base, double exponent);
float exp(float x) noexcept;


#endif
