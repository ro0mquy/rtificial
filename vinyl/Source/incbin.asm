bits 32
section .data

%ifdef SYNTH_V2
global _soundtrack
_soundtrack incbin "../../Source/music/soundtrack.v2m"
%endif
%ifdef SYNTH_DUAL_V2_4KLANG
global _soundtrack
_soundtrack incbin "../../Source/music/soundtrack.v2m"
%endif
%ifdef SYNTH_VORBIS
global _soundtrack
_soundtrack incbin "../../Source/music/soundtrack.ogg"
_soundtrack_end:
global _soundtrack_size
_soundtrack_size dd _soundtrack_end - _soundtrack
%endif

; Some replacements for things VC2005 needs when compiling without CRT
%ifdef NDEBUG
; http://stackoverflow.com/questions/1583196/building-visual-c-app-that-doesnt-use-crt-functions-still-references-some
; already defined in WindowsFrontend.cpp
;global __fltused
;__fltused dd 0

section .text

global __chkstk
__chkstk ret
%endif
