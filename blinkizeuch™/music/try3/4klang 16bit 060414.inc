%macro export_func 1
   global _%1
   _%1:
%endmacro
%define SAMPLE_RATE	44100
%define MAX_INSTRUMENTS	15
%define MAX_VOICES 1
%define HLD 1
%define BPM 136.000000
%define MAX_PATTERNS 113
%define PATTERN_SIZE_SHIFT 4
%define PATTERN_SIZE (1 << PATTERN_SIZE_SHIFT)
%define	MAX_TICKS (MAX_PATTERNS*PATTERN_SIZE)
%define	SAMPLES_PER_TICK 4863
%define DEF_LFO_NORMALIZE 0.0000513983
%define	MAX_SAMPLES	(SAMPLES_PER_TICK*MAX_TICKS)
%define 	GO4K_USE_16BIT_OUTPUT
;%define 	GO4K_USE_GROOVE_PATTERN
%define 	GO4K_USE_ENVELOPE_RECORDINGS
%define 	GO4K_USE_NOTE_RECORDINGS
%define 	GO4K_CLIP_OUTPUT
%define 	GO4K_USE_DST
%define 	GO4K_USE_DLL
%define 	GO4K_USE_PAN
%define 	GO4K_USE_GLOBAL_DLL
%define 	GO4K_USE_FSTG
%define 	GO4K_USE_ENV_CHECK
%define 	GO4K_USE_ENV_MOD_GM
%define 	GO4K_USE_VCO_CHECK
%define 	GO4K_USE_VCO_PHASE_OFFSET
%define 	GO4K_USE_VCO_SHAPE
%define 	GO4K_USE_VCO_MOD_TM
%define 	GO4K_USE_VCF_CHECK
%define 	GO4K_USE_VCF_HIGH
%define 	GO4K_USE_VCF_BAND
%define 	GO4K_USE_VCF_PEAK
%define 	GO4K_USE_DST_CHECK
%define 	GO4K_USE_DST_SH
%define		GO4K_USE_DLL_CHORUS_CLAMP
%define 	GO4K_USE_DLL_DAMP
%define 	GO4K_USE_DLL_DC_FILTER
%define 	GO4K_USE_FSTG_CHECK
%define		GO4K_USE_WAVESHAPER_CLIP
%define	MAX_DELAY			65536
%define MAX_WORKSPACE_SLOTS	8
%define GO4K_BEGIN_CMDDEF(def_name)
%define GO4K_END_CMDDEF db 0
%define GO4K_BEGIN_PARAMDEF(def_name)
%define GO4K_END_PARAMDEF
GO4K_ENV_ID		equ		1
%macro	GO4K_ENV 5
	db	%1
	db	%2
	db	%3
	db	%4
	db	%5
%endmacro
%define	ATTAC(val)		val	
%define	DECAY(val)		val	
%define	SUSTAIN(val)	val	
%define	RELEASE(val)	val	
%define	GAIN(val)		val	
struc	go4kENV_val
	.attac		resd	1
	.decay		resd	1
	.sustain	resd	1
	.release	resd	1
	.gain		resd	1
	.size
endstruc
struc	go4kENV_wrk
	.state		resd	1
	.level		resd	1
	.gm			resd	1
	.am			resd	1
	.dm			resd	1
	.sm			resd	1
	.rm			resd	1
	.size
endstruc
%define ENV_STATE_ATTAC		0
%define ENV_STATE_DECAY		1
%define ENV_STATE_SUSTAIN	2
%define ENV_STATE_RELEASE	3
%define ENV_STATE_OFF		4
GO4K_VCO_ID		equ		2
%macro	GO4K_VCO 8
	db	%1
	db	%2
%ifdef GO4K_USE_VCO_PHASE_OFFSET	
	db	%3
%endif	
%ifdef GO4K_USE_VCO_GATE	
	db	%4
%endif	
	db	%5
%ifdef GO4K_USE_VCO_SHAPE	
	db	%6
%endif	
	db	%7
	db	%8
%endmacro
%define	TRANSPOSE(val)	val	
%define	DETUNE(val)		val	
%define	PHASE(val)		val	
%define	GATES(val)		val	
%define	COLOR(val)		val	
%define	SHAPE(val)		val 
%define	FLAGS(val)		val	
%define SINE		0x01
%define TRISAW		0x02
%define PULSE		0x04
%define NOISE		0x08
%define LFO			0x10
%define GATE		0x20
struc	go4kVCO_val
	.transpose	resd	1
	.detune		resd	1
%ifdef GO4K_USE_VCO_PHASE_OFFSET	
	.phaseofs	resd	1
%endif	
%ifdef GO4K_USE_VCO_GATE	
	.gate		resd	1
%endif	
	.color		resd	1
%ifdef GO4K_USE_VCO_SHAPE	
	.shape		resd	1
%endif	
	.gain		resd	1
	.flags		resd	1	
	.size
endstruc
struc	go4kVCO_wrk
	.phase		resd	1
	.tm			resd	1
	.dm			resd	1
	.fm			resd	1
	.pm			resd	1
	.cm			resd	1
	.sm			resd	1
	.gm			resd	1
	.size
endstruc
GO4K_VCF_ID		equ		3
%macro	GO4K_VCF 3
	db	%1
	db	%2
	db	%3
%endmacro
%define LOWPASS		0x1
%define HIGHPASS	0x2
%define BANDPASS	0x4
%define	BANDSTOP	0x3
%define ALLPASS		0x7
%define	PEAK		0x8
%define	FREQUENCY(val)	val
%define	RESONANCE(val)	val
%define	VCFTYPE(val)	val
struc	go4kVCF_val
	.freq		resd	1
	.res		resd	1
	.type		resd	1
	.size
endstruc
struc	go4kVCF_wrk
	.low		resd	1
	.high		resd	1
	.band		resd	1
	.freq		resd	1
	.fm			resd	1
	.rm			resd	1	
	.size
endstruc
GO4K_DST_ID		equ		4
%macro	GO4K_DST 2
	db	%1
%ifdef GO4K_USE_DST_SH	
	db	%2
%endif	
%endmacro
%define	DRIVE(val)		val
%define	SNHFREQ(val)	val
struc	go4kDST_val
	.drive		resd	1
%ifdef GO4K_USE_DST_SH	
	.snhfreq	resd	1
%endif	
	.size
endstruc
struc	go4kDST_wrk
%ifdef GO4K_USE_DST_SH	
	.out		resd	1
	.snhphase	resd	1
%endif	
	.dm			resd	1
	.sm			resd	1
	.size
endstruc
GO4K_DLL_ID		equ		5
%macro	GO4K_DLL 8
	db	%1
	db	%2
	db	%3
%ifdef GO4K_USE_DLL_DAMP	
	db	%4
%endif		
%ifdef GO4K_USE_DLL_CHORUS	
	db	%5
	db	%6
%endif
	db	%7
	db	%8
%endmacro
%define PREGAIN(val)	val
%define	DRY(val)		val
%define	FEEDBACK(val)	val
%define	DEPTH(val)		val
%define DAMP(val)		val
%define	DELAY(val)		val
%define	COUNT(val)		val
struc	go4kDLL_val
	.pregain	resd	1
	.dry		resd	1
	.feedback	resd	1
%ifdef GO4K_USE_DLL_DAMP	
	.damp		resd	1	
%endif
%ifdef GO4K_USE_DLL_CHORUS
	.freq		resd	1
	.depth
%endif
	.delay		resd	1
	.count		resd	1
	.size
endstruc
struc	go4kDLL_wrk
	.index		resd	1
	.store		resd	1
	.dcin		resd	1
	.dcout		resd	1
%ifdef GO4K_USE_DLL_CHORUS
	.phase		resd	1
%endif
	.buffer		resd	MAX_DELAY
	.size
endstruc
struc	go4kDLL_wrk2
 .pm			resd	1
 .fm			resd	1
 .im			resd	1
 .dm			resd	1
 .sm			resd	1
 .am			resd	1
 .size
endstruc
GO4K_FOP_ID	equ			6
%macro	GO4K_FOP 1
	db	%1
%endmacro
%define	OP(val)			val
%define FOP_POP			0x1
%define FOP_ADDP		0x2
%define FOP_MULP		0x3
%define FOP_PUSH		0x4
%define FOP_XCH			0x5
%define FOP_ADD			0x6
%define FOP_MUL			0x7
%define FOP_ADDP2		0x8
%define FOP_LOADNOTE	0x9
struc	go4kFOP_val
	.flags		resd	1
	.size
endstruc
struc	go4kFOP_wrk
	.size
endstruc
GO4K_FST_ID		equ		7
%macro	GO4K_FST 2
	db	%1
	db	%2
%endmacro
%define	AMOUNT(val)		val
%define	DEST(val)		val
struc	go4kFST_val
	.amount		resd	1
	.op1		resd	1
	.size
endstruc
struc	go4kFST_wrk
	.size
endstruc
GO4K_PAN_ID		equ		8
%macro	GO4K_PAN 1
%ifdef GO4K_USE_PAN
	db	%1
%endif
%endmacro
%define	PANNING(val)	val
struc	go4kPAN_val
%ifdef GO4K_USE_PAN
	.panning	resd	1
%endif
	.size
endstruc
struc	go4kPAN_wrk
	.pm			resd	1
	.size
endstruc
GO4K_OUT_ID		equ		9
%macro	GO4K_OUT 2
	db	%1
%ifdef GO4K_USE_GLOBAL_DLL	
	db	%2
%endif	
%endmacro
%define	AUXSEND(val)	val
struc	go4kOUT_val
	.gain		resd	1
%ifdef GO4K_USE_GLOBAL_DLL	
	.auxsend	resd	1
%endif
	.size
endstruc
struc	go4kOUT_wrk
	.am			resd	1
	.gm			resd	1
	.size
endstruc
GO4K_ACC_ID		equ		10
%macro	GO4K_ACC 1
	db	%1
%endmacro
%define OUTPUT			0
%define	AUX				8
%define ACCTYPE(val)	val
struc	go4kACC_val
	.acctype	resd	1
	.size
endstruc
struc	go4kACC_wrk
	.size
endstruc
%ifdef GO4K_USE_FLD
GO4K_FLD_ID	equ		11
%macro	GO4K_FLD 1
	db	
%endmacro
%define	VALUE(val)	val
struc	go4kFLD_val
	.value		resd	1
	.size
endstruc
struc	go4kFLD_wrk
	.vm			resd	1
	.size
endstruc
%endif
%ifdef GO4K_USE_FSTG
GO4K_FSTG_ID	equ		12
%macro	GO4K_FSTG 2
	db	%1
	dd	%2
%endmacro
struc	go4kFSTG_val
	.amount		resd	1
	.op1		resd	1
	.size
endstruc
struc	go4kFSTG_wrk
	.size
endstruc
%endif
struc	go4k_instrument
	.release	resd	1
	.note		resd	1
	.workspace	resd	256
	.dlloutl	resd	1
	.dlloutr	resd	1
	.outl		resd	1
	.outr		resd	1
	.size
endstruc
struc	go4k_synth
	.instruments	resb	go4k_instrument.size * MAX_INSTRUMENTS * MAX_VOICES
	.global			resb	go4k_instrument.size * MAX_VOICES
	.size
endstruc
%ifdef USE_SECTIONS
section .g4kmuc1 data align=1
%else
section .data align=1
%endif
go4k_patterns
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, 0, 48, 48, 0, 
	db	48, 0, 48, 0, 48, HLD, 0, 0, 48, 0, 48, HLD, 0, 0, 48, 0, 
	db	0, 48, 48, HLD, 77, 36, 48, 0, 48, 48, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 48, HLD, 0, 48, 48, 0, 48, 0, 48, 0, 48, HLD, 0, 0, 
	db	48, 0, 48, HLD, 0, 0, 48, 0, 0, 48, 48, HLD, 77, 36, 48, 0, 
	db	48, 48, 48, HLD, 0, 48, 0, 0, 48, 0, 48, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 127, 74, 
	db	0, 0, 77, 0, 48, HLD, 0, 48, 0, 0, 48, 0, 48, 0, 48, HLD, 
	db	0, 0, 48, 0, 48, HLD, 0, 0, 0, 0, 0, 0, 48, HLD, 48, HLD, 
	db	0, 48, 0, 0, 48, HLD, 0, 48, 48, 0, 48, 0, 48, 0, 48, HLD, 
	db	0, 0, 48, 0, 48, HLD, 0, 0, 48, 0, 0, 48, 48, HLD, 77, 36, 
	db	48, 0, 48, 48, 48, HLD, 0, 48, 0, 0, 48, 0, 48, 0, 48, HLD, 
	db	0, 0, 48, 0, 48, HLD, 0, 0, 48, 0, 48, HLD, 0, 0, 0, 48, 
	db	0, 48, 0, 48, 48, HLD, 0, 48, 0, 0, 48, 0, 48, 0, 48, HLD, 
	db	0, 0, 48, 0, 48, HLD, 0, 0, 48, 0, 48, HLD, 0, 0, 0, 0, 
	db	48, 0, 48, 0, 48, HLD, 0, 48, 0, 0, 48, 0, 48, 0, 48, HLD, 
	db	48, 0, 48, 0, 48, HLD, 0, 48, 0, 0, 48, 0, 48, 0, 0, 0, 
	db	48, HLD, 0, 48, 48, 0, 48, 0, 48, 0, 48, HLD, 0, 0, 48, 0, 
	db	48, HLD, 0, 0, 48, HLD, 0, 48, 0, 0, 48, 0, 48, 0, 0, 0, 
	db	0, 0, 0, 0, 48, HLD, 0, 0, 0, 0, 48, HLD, 0, 0, 0, 0, 
	db	48, HLD, 0, 0, 0, 0, 0, 0, 48, HLD, 48, HLD, 0, 0, 0, 0, 
	db	48, HLD, 0, 0, 0, 0, 0, 0, 48, HLD, 0, 0, 0, 0, 0, 0, 
	db	48, HLD, 0, 48, 0, 0, 0, 0, 0, 0, 48, HLD, 0, 0, 0, 0, 
	db	48, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, 0, 
	db	48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	48, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, 0, 0, 0, 0, 
	db	48, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, 0, 0, 48, HLD, 
	db	48, HLD, 0, 0, 48, HLD, 0, 0, 48, HLD, 0, 0, 48, HLD, 0, 0, 
	db	48, HLD, 0, 48, 0, 0, 48, 0, 48, 0, 48, HLD, 0, 0, 48, 0, 
	db	48, HLD, 0, 0, 0, 0, 0, 0, 48, HLD, 48, HLD, 0, 48, 0, 0, 
	db	48, HLD, 0, 0, 48, 0, 48, HLD, 0, 0, 0, 0, 48, 0, 0, 0, 
	db	48, HLD, 0, 0, 48, 0, 48, HLD, 0, 0, 0, 0, 48, 0, 48, HLD, 
	db	0, 0, 48, HLD, 0, 0, 0, 0, 48, HLD, 48, HLD, 48, HLD, 48, HLD, 
	db	48, HLD, 48, HLD, 48, HLD, 0, 0, 48, HLD, 48, HLD, 48, HLD, 48, HLD, 
	db	0, 0, 89, 0, 81, 0, 88, 0, 81, HLD, 86, HLD, 81, HLD, 84, HLD, 
	db	86, HLD, 89, 0, 81, HLD, 88, HLD, 81, HLD, 86, HLD, 81, 0, 84, HLD, 
	db	86, HLD, 89, HLD, 81, HLD, 88, HLD, 81, HLD, 86, HLD, 81, HLD, 84, HLD, 
	db	86, HLD, 89, HLD, 81, 0, 88, HLD, 81, HLD, 86, HLD, 81, HLD, 0, 0, 
	db	0, 0, 0, 0, 89, 0, 81, 0, 88, 0, 81, HLD, 86, HLD, 81, HLD, 
	db	84, HLD, 86, HLD, 89, 0, 81, HLD, 88, HLD, 81, HLD, 86, HLD, 81, 0, 
	db	84, HLD, 86, HLD, 89, HLD, 81, HLD, 88, HLD, 81, HLD, 86, HLD, 81, HLD, 
	db	84, HLD, 86, HLD, 89, HLD, 81, 0, 88, HLD, 81, HLD, 86, HLD, 81, HLD, 
	db	93, HLD, HLD, HLD, 89, 0, 81, 0, 88, 0, 81, HLD, 86, HLD, 81, HLD, 
	db	93, HLD, HLD, HLD, 89, 0, 57, 0, 57, 0, 57, 0, 57, 0, 57, 0, 
	db	57, 0, 57, 0, 57, 0, 57, 0, 57, 0, 57, 0, 57, 0, 57, 0, 
	db	57, 0, 57, 0, 52, 0, 52, 0, 52, 0, 52, 0, 52, 0, 52, 0, 
	db	52, 0, 52, 0, 53, 0, 53, 0, 53, 0, 53, 0, 53, 0, 53, 0, 
	db	53, 0, 53, 0, 47, 0, 47, 0, 47, 0, 47, 0, 47, 0, 47, 0, 
	db	47, 0, 47, 0, 48, 0, 48, 0, 48, 0, 48, 0, 48, 0, 48, 0, 
	db	81, HLD, 84, 0, 88, 0, 81, 0, 81, HLD, 84, 0, 89, 0, 81, 0, 
	db	83, 0, 86, 0, 91, 0, 83, 0, 84, 0, 88, 0, 93, 0, 84, 0, 
	db	88, 0, 93, 0, 96, 0, 88, 0, 89, 0, 93, 0, 96, 0, 89, 0, 
	db	91, 0, 95, 0, 98, 0, 91, 0, 93, 0, 96, 0, 100, 0, 93, 0, 
	db	93, HLD, 98, 0, 101, 0, 93, 0, 93, HLD, 96, 0, 100, 0, 93, 0, 
	db	89, 0, 81, 0, 88, 0, 81, HLD, 86, HLD, 81, HLD, 84, HLD, 86, HLD, 
	db	89, 0, 81, HLD, 88, HLD, 81, HLD, 86, HLD, 81, 0, 84, HLD, 86, HLD, 
	db	89, HLD, 81, HLD, 88, HLD, 81, HLD, 86, HLD, 81, HLD, 84, HLD, 86, HLD, 
	db	89, HLD, 81, 0, 88, HLD, 81, HLD, 86, HLD, 81, HLD, 93, HLD, HLD, HLD, 
	db	89, HLD, 81, 0, 88, HLD, 81, HLD, 86, HLD, 81, HLD, 93, HLD, 0, 0, 
	db	0, 0, 0, 0, 81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 
	db	81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 
	db	81, 0, 81, 0, 91, 0, 93, 0, 95, 0, 96, 0, 93, 0, 93, 0, 
	db	93, 0, 93, 0, 81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 
	db	81, 0, 84, 0, 88, 0, 81, 0, 81, 0, 84, 0, 89, 0, 81, 0, 
	db	93, 0, 98, 0, 101, 0, 93, 0, 93, 0, 96, 0, 100, 0, 93, 0, 
	db	0, 0, 93, 0, 0, 0, 93, 0, 0, 0, 0, 0, 96, 0, 98, 0, 
	db	101, 0, 93, 0, 100, 0, 93, 0, 98, 0, 93, 0, 96, 0, 98, 0, 
	db	101, 0, 101, 0, 100, 0, 93, 0, 98, 0, 93, 0, 105, 0, 105, 0, 
	db	81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 0, 0, 
	db	93, 81, 93, 81, 93, 81, 93, 81, 93, 81, 93, 81, 93, 81, 93, 81, 
	db	93, 81, 93, 81, 93, 81, 93, 81, 93, 81, 93, 81, 93, 81, 0, 0, 
	db	0, 0, 0, 0, 84, 0, 84, 0, 84, 0, 84, 0, 84, 0, 84, 0, 
	db	84, 0, 84, 0, 81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 
	db	81, 0, 81, 0, 84, 0, 84, 0, 84, 0, 84, 0, 84, 0, 84, 0, 
	db	84, 0, 84, 0, 86, 0, 86, 0, 86, 0, 86, 0, 86, 0, 86, 0, 
	db	86, 0, 86, 0, 88, 0, 88, 0, 88, 0, 88, 0, 88, 0, 88, 0, 
	db	88, 0, 88, 0, 89, 0, 89, 0, 89, 0, 89, 0, 89, 0, 89, 0, 
	db	89, 0, 89, 0, 93, 0, 93, 0, 93, 0, 93, 0, 93, 0, 93, 0, 
	db	93, 0, 93, 0, 88, 0, 88, 0, 88, 0, 88, 0, 88, 0, 88, 0, 
	db	88, 0, 88, 0, 96, 0, 96, 0, 96, 0, 96, 0, 96, 0, 96, 0, 
	db	96, 0, 96, 0, 81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 81, 0, 
	db	81, 0, 81, 0, 93, 0, 93, 0, 93, 0, 93, 0, 93, 0, 93, 0, 
	db	81, 0, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	69, 0, 69, 0, 57, 0, 57, 0, 57, 0, 57, 0, 57, 0, 57, 0, 
	db	57, 0, 57, 0, 79, 0, 81, 0, 83, 0, 84, 0, 81, 0, 81, 0, 
	db	81, 0, 81, 0, 57, 0, 57, 0, 57, 0, 57, 0, 57, 0, 57, 0, 
	db	81, 84, 88, 81, 84, 88, 81, 84, 81, 84, 89, 81, 84, 89, 81, 84, 
	db	83, 86, 91, 83, 86, 91, 83, 86, 84, 88, 93, 84, 88, 93, 84, 88, 
	db	88, 93, 96, 88, 93, 96, 88, 93, 89, 93, 96, 89, 89, 93, 96, 89, 
	db	91, 95, 98, 91, 95, 98, 91, 95, 93, 96, 100, 93, 96, 100, 93, 96, 
	db	93, 98, 101, 93, 98, 101, 93, 98, 93, 96, 100, 93, 96, 100, 93, 96, 
	db	0, 89, 93, 93, 0, 100, 93, 93, 0, 98, 0, 93, 96, 96, 98, 98, 
	db	101, 101, 93, 93, 100, 100, 93, 93, 98, 98, 93, 93, 96, 96, 98, 98, 
	db	101, 101, 101, 93, 100, 100, 93, 93, 98, 98, 93, 93, 105, 105, 105, 105, 
	db	93, 0, 93, 0, 93, 0, 93, 0, 93, 0, 93, 0, 93, 0, 93, 0, 
	db	88, 0, 88, 0, 88, 0, 88, 0, 88, 0, 88, 0, 88, 0, 88, 0, 
	db	96, 0, 96, 0, 96, 0, 96, 0, 96, 0, 96, 0, 96, 0, 96, 0, 
	db	96, 84, 96, 84, 96, 84, 96, 84, 96, 84, 96, 84, 96, 84, 96, 84, 
	db	93, 84, 93, 84, 93, 84, 93, 84, 93, 84, 93, 84, 93, 84, 81, 0, 
	db	93, 84, 93, 84, 93, 84, 93, 84, 93, 84, 93, 84, 93, 84, 0, 0, 
	db	84, HLD, 86, HLD, 89, HLD, 81, 0, 88, HLD, 81, HLD, 89, 0, 81, 0, 
	db	88, 0, 81, HLD, 86, HLD, 81, HLD, 84, HLD, 86, HLD, 89, 0, 81, HLD, 
	db	88, HLD, 81, HLD, 86, HLD, 81, 0, 84, HLD, 86, HLD, 89, HLD, 81, HLD, 
	db	88, HLD, 81, HLD, 86, HLD, 81, HLD, 84, HLD, 86, HLD, 89, HLD, 81, 0, 
	db	88, HLD, 81, HLD, 86, HLD, 81, HLD, 93, HLD, HLD, HLD, 89, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, 
	db	0, 0, 0, 0, 0, 0, 48, HLD, 0, 0, 0, 0, 0, 0, 48, HLD, 
	db	0, 0, 0, 0, 0, 0, 48, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 33, 0, 0, 0, 33, 0, 0, 0, 33, 0, 
	db	0, 0, 57, 0, 0, 0, 33, 0, 0, 0, 33, 0, 0, 0, 33, 0, 
	db	0, 0, 33, 0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 33, 0, 
	db	0, 0, 33, 0, 0, 0, 33, 0, 0, 0, 57, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 52, 0, 52, 0, 52, 0, 52, 0, 52, 0, 52, 0, 
	db	52, 0, 57, 0, 52, 0, 52, 0, 52, 0, 52, 0, 52, 0, 52, 0, 
	db	0, 0, 33, 0, 0, 0, 33, 0, 0, 0, 33, 0, 0, 0, 0, 0, 
	db	0, 0, 33, 0, 0, 0, 33, 0, 0, 0, 33, 0, 0, 0, 57, 0, 
	db	57, 0, 57, 57, 57, 0, 57, 57, 57, 0, 57, 57, 0, 0, 0, 0, 
	db	69, 0, 69, 69, 69, 0, 69, 69, 69, 0, 69, 69, 0, 0, 0, 0, 
	db	81, 0, 81, 81, 81, 0, 81, 81, 81, 0, 81, 81, 0, 0, 0, 81, 
	db	0, 81, 81, 81, 0, 81, 81, 81, 0, 81, 81, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 48, HLD, 0, 0, 0, 0, 0, 0, 48, HLD, 0, 0, 
	db	127, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 48, HLD, 0, 0, 48, HLD, 0, 0, 48, HLD, 
	db	0, 0, 48, HLD, 0, 0, 48, HLD, 0, 0, 48, HLD, 0, 0, 48, HLD, 
	db	0, 0, 48, HLD, 0, 0, 48, HLD, 0, 0, 48, HLD, 0, 0, 48, 0, 
	db	0, 0, 48, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 48, HLD, 0, 0, 48, HLD, 0, 0, 48, 0, 0, 0, 0, 0, 
	db	0, 0, 48, HLD, 0, 0, 48, HLD, 0, 0, 48, 0, 0, 0, 48, HLD, 
	db	48, HLD, 48, HLD, 48, HLD, 48, HLD, 48, HLD, 48, HLD, 48, HLD, 0, 0, 
	db	48, HLD, 48, HLD, 48, HLD, 48, HLD, 48, HLD, 0, 0, 0, 0, 0, 0, 
	db	48, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 48, HLD, 48, HLD, 48, HLD, 48, HLD, 48, HLD, 
	db	48, HLD, 48, HLD, 0, 0, 48, HLD, 48, HLD, 48, HLD, 48, HLD, 48, HLD, 
	db	48, HLD, 0, 0, 0, 0, 48, HLD, 48, HLD, 48, HLD, 48, HLD, 48, HLD, 
	db	0, 0, 0, 0, 45, 0, 0, 0, 45, 0, 0, 0, 41, 0, 0, 0, 
	db	41, 0, 0, 0, 43, 0, 0, 0, 43, 0, 0, 0, 45, 0, 0, 0, 
	db	45, 0, 0, 0, 45, 0, 0, 0, 45, 0, 0, 0, 41, 0, 0, 0, 
	db	45, 0, 0, 0, 50, 0, 0, 0, 50, 0, 0, 0, 45, 0, 0, 0, 
	db	45, 0, 0, 0, 43, 0, 0, 0, 43, 0, 0, 0, 45, 0, 0, 0, 
	db	41, 0, 0, 0, 43, 0, 0, 0, 43, 0, 0, 0, 45, 0, 52, 0, 
	db	57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 72, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 45, 0, 0, 0, 45, 0, 0, 0, 41, 0, 0, 0, 41, 0, 
	db	0, 0, 43, 0, 0, 0, 43, 0, 0, 0, 45, 0, 0, 0, 45, 0, 
	db	0, 0, 50, 0, 0, 0, 50, 0, 0, 0, 45, 0, 0, 0, 45, 0, 
	db	0, 0, 43, 0, 0, 0, 43, 0, 0, 0, 45, 0, 52, 0, 57, 0, 
	db	50, 0, 38, 0, 0, 0, 38, 0, 0, 0, 45, 0, 0, 0, 45, 0, 
	db	0, 0, 33, 0, 0, 0, 33, 0, 33, 0, 33, 0, 0, 0, 0, 0, 
	db	0, 0, 33, 0, 0, 33, 36, 0, 0, 0, 33, 0, 0, 0, 57, 0, 
	db	0, 0, 33, 0, 0, 0, 33, 0, 0, 0, 33, 0, 33, 0, 0, 33, 
	db	0, 0, 33, 0, 33, 0, 33, 0, 33, 0, 33, 0, 33, 0, 33, 0, 
	db	33, 0, 33, 0, 33, 33, 36, 0, 33, 0, 36, 33, 33, 33, 57, 0, 
	db	57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 9, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, HLD, HLD, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, 0, 0, 
	db	48, HLD, 0, 0, 48, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	48, HLD, 0, 0, 48, HLD, 0, 0, 48, HLD, 0, 0, 0, 0, 0, 0, 
	db	49, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	49, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 
	db	49, 0, 35, 0, 0, 0, 0, 0, 0, 0, 49, 0, 35, 0, 0, 49, 
	db	35, 0, 0, 0, 49, 35, 0, 0, 0, 49, 0, 35, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 49, 0, 35, 0, 0, 49, 
	db	0, 35, 0, 0, 49, 35, 0, 0, 0, 49, 35, 0, 0, 0, 49, 0, 
	db	35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
go4k_patterns_end
%ifdef USE_SECTIONS
section .g4kmuc2 data align=1
%else
section .data
%endif
go4k_pattern_lists
Instrument0List		db	0, 0, 1, 1, 1, 1, 1, 0, 0, 2, 3, 4, 0, 5, 6, 7, 0, 0, 0, 0, 8, 9, 10, 11, 12, 13, 10, 11, 14, 15, 10, 11, 16, 17, 10, 11, 16, 18, 19, 20, 19, 0, 0, 0, 0, 21, 22, 23, 23, 24, 22, 24, 22, 24, 22, 0, 0, 24, 22, 24, 25, 0, 26, 27, 0, 25, 25, 28, 29, 30, 30, 0, 0, 30, 30, 0, 0, 0, 0, 30, 30, 0, 0, 30, 30, 30, 30, 25, 0, 0, 0, 31, 32, 19, 33, 31, 32, 19, 34, 31, 32, 19, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument1List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 36, 25, 0, 37, 38, 39, 40, 41, 42, 43, 44, 41, 42, 43, 44, 41, 42, 43, 44, 45, 42, 43, 44, 45, 42, 43, 44, 45, 42, 43, 44, 46, 47, 47, 47, 47, 47, 47, 47, 48, 49, 50, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 52, 53, 54, 55, 56, 55, 54, 55, 57, 58, 59, 60, 57, 58, 59, 60, 57, 58, 59, 60, 57, 58, 59, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument2List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 63, 63, 63, 62, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 64, 63, 64, 65, 64, 0, 0, 0, 0, 0, 0, 0, 0, 66, 53, 54, 55, 67, 55, 54, 55, 66, 53, 54, 55, 67, 55, 54, 55, 0, 0, 66, 53, 54, 55, 67, 55, 54, 55, 68, 69, 69, 70, 0, 0, 63, 71, 72, 72, 72, 72, 72, 72, 72, 73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument3List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 75, 76, 75, 74, 77, 78, 79, 80, 81, 82, 83, 84, 81, 82, 83, 85, 0, 0, 0, 62, 64, 65, 64, 86, 87, 88, 87, 0, 0, 0, 0, 0, 0, 0, 0, 89, 90, 91, 92, 93, 92, 91, 92, 89, 90, 91, 92, 93, 92, 91, 92, 0, 0, 89, 90, 91, 92, 93, 92, 91, 92, 94, 95, 95, 96, 97, 98, 99, 71, 97, 98, 100, 101, 97, 98, 100, 102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument4List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, 42, 43, 44, 45, 42, 43, 103, 104, 105, 106, 107, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, 58, 59, 61, 57, 58, 59, 60, 57, 58, 59, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument5List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 108, 109, 110, 0, 109, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 111, 111, 111, 111, 111, 111, 111, 111, 112, 111, 111, 111, 112, 111, 111, 111, 113, 113, 113, 114, 115, 49, 50, 51, 116, 49, 50, 51, 115, 49, 50, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 117, 117, 117, 118, 117, 117, 117, 118, 119, 120, 121, 122, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument6List		db	0, 0, 0, 108, 109, 109, 109, 110, 0, 0, 0, 0, 0, 109, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 123, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 25, 0, 0, 0, 0, 0, 0, 0, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 0, 0, 0, 0, 0, 0, 0, 0, 124, 124, 124, 124, 124, 124, 124, 124, 0, 0, 124, 124, 124, 124, 124, 124, 124, 124, 125, 0, 0, 0, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument7List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 126, 127, 127, 128, 127, 127, 127, 128, 127, 127, 127, 128, 129, 0, 0, 0, 129, 0, 0, 0, 126, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 129, 127, 127, 127, 127, 0, 0, 0, 0, 0, 0, 0, 0, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 0, 0, 0, 0, 127, 127, 127, 130, 127, 127, 127, 131, 127, 127, 127, 130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument8List		db	0, 0, 0, 0, 0, 0, 132, 133, 0, 1, 132, 133, 0, 5, 6, 134, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 135, 136, 137, 136, 137, 136, 137, 136, 137, 136, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 138, 139, 140, 139, 141, 142, 140, 143, 144, 0, 138, 139, 140, 139, 141, 142, 140, 143, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument9List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 145, 146, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument10List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 148, 147, 148, 147, 148, 147, 148, 149, 148, 147, 150, 0, 0, 147, 148, 147, 148, 151, 148, 147, 150, 0, 0, 0, 0, 0, 0, 0, 0, 152, 117, 152, 153, 152, 154, 155, 156, 157, 158, 146, 159, 0, 0, 0, 0, 0, 0, 
Instrument11List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 160, 161, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument12List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument13List		db	0, 0, 0, 0, 0, 163, 30, 164, 0, 163, 30, 165, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument14List		db	166, 166, 166, 166, 166, 167, 167, 168, 169, 0, 0, 170, 171, 172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
go4k_pattern_lists_end
%ifdef USE_SECTIONS
section .g4kmuc3 data align=1
%else
section .data
%endif
go4k_synth_instructions
GO4K_BEGIN_CMDDEF(Instrument0)
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_ENV_ID
	db GO4K_DST_ID
	db GO4K_FST_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument1)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_DST_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_DLL_ID
	db GO4K_DLL_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument2)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_DST_ID
	db GO4K_DLL_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument3)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_DLL_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument4)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument5)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument6)
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_ENV_ID
	db GO4K_DST_ID
	db GO4K_FST_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
	db GO4K_VCF_ID
	db GO4K_DST_ID
	db GO4K_DLL_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument7)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
	db GO4K_DST_ID
	db GO4K_DLL_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument8)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument9)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
	db GO4K_DST_ID
	db GO4K_DLL_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument10)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_DST_ID
	db GO4K_VCF_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument11)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FSTG_ID
	db GO4K_FOP_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument12)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_DLL_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument13)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_DLL_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument14)
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_ENV_ID
	db GO4K_DST_ID
	db GO4K_FST_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Global)
	db GO4K_ACC_ID
	db GO4K_DLL_ID
	db GO4K_FOP_ID
	db GO4K_DLL_ID
	db GO4K_FOP_ID
	db GO4K_ACC_ID
	db GO4K_FOP_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
go4k_synth_instructions_end
%ifdef USE_SECTIONS
section .g4kmuc4 data align=1
%else
section .data
%endif
go4k_synth_parameter_values
GO4K_BEGIN_PARAMDEF(Instrument0)
	GO4K_ENV	ATTAC(0),DECAY(64),SUSTAIN(96),RELEASE(64),GAIN(128)
	GO4K_FST	AMOUNT(128),DEST(0*MAX_WORKSPACE_SLOTS+2)
	GO4K_ENV	ATTAC(0),DECAY(68),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_DST	DRIVE(32), SNHFREQ(128)
	GO4K_FST	AMOUNT(80),DEST(6*MAX_WORKSPACE_SLOTS+1)
	GO4K_FOP	OP(FOP_POP)
	GO4K_VCO	TRANSPOSE(46),DETUNE(64),PHASE(0),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(TRISAW)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(128), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument1)
	GO4K_ENV	ATTAC(3),DECAY(25),SUSTAIN(105),RELEASE(10),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(128),FLAGS(SINE)
	GO4K_DST	DRIVE(94), SNHFREQ(77)
	GO4K_VCF	FREQUENCY(36),RESONANCE(116),VCFTYPE(PEAK)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_DLL	PREGAIN(53),DRY(44),FEEDBACK(114),DAMP(98),FREQUENCY(0),DEPTH(0),DELAY(8),COUNT(8)
	GO4K_DLL	PREGAIN(92),DRY(82),FEEDBACK(50),DAMP(0),FREQUENCY(0),DEPTH(0),DELAY(16),COUNT(1)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(64), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument2)
	GO4K_ENV	ATTAC(64),DECAY(64),SUSTAIN(64),RELEASE(64),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(128),FLAGS(PULSE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_DST	DRIVE(64), SNHFREQ(128)
	GO4K_DLL	PREGAIN(64),DRY(128),FEEDBACK(64),DAMP(64),FREQUENCY(0),DEPTH(0),DELAY(17),COUNT(1)
	GO4K_PAN	PANNING(15)
	GO4K_OUT	GAIN(33), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument3)
	GO4K_ENV	ATTAC(64),DECAY(64),SUSTAIN(64),RELEASE(64),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(128),FLAGS(PULSE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_DLL	PREGAIN(64),DRY(128),FEEDBACK(64),DAMP(64),FREQUENCY(0),DEPTH(0),DELAY(17),COUNT(1)
	GO4K_PAN	PANNING(106)
	GO4K_OUT	GAIN(64), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument4)
	GO4K_ENV	ATTAC(49),DECAY(66),SUSTAIN(4),RELEASE(74),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(64),GATES(85),COLOR(64),SHAPE(96),GAIN(128),FLAGS(SINE)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(64),GATES(85),COLOR(64),SHAPE(96),GAIN(128),FLAGS(SINE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(64), AUXSEND(93)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument5)
	GO4K_ENV	ATTAC(0),DECAY(74),SUSTAIN(0),RELEASE(74),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(64),GATES(85),COLOR(64),SHAPE(96),GAIN(128),FLAGS(SINE)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(64),GATES(85),COLOR(64),SHAPE(96),GAIN(128),FLAGS(SINE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(64), AUXSEND(93)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument6)
	GO4K_ENV	ATTAC(0),DECAY(75),SUSTAIN(0),RELEASE(0),GAIN(32)
	GO4K_FST	AMOUNT(128),DEST(0*MAX_WORKSPACE_SLOTS+2)
	GO4K_ENV	ATTAC(0),DECAY(70),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_DST	DRIVE(8), SNHFREQ(128)
	GO4K_FST	AMOUNT(76),DEST(7*MAX_WORKSPACE_SLOTS+1)
	GO4K_FOP	OP(FOP_POP)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(64),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(NOISE)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(64),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(SINE)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_VCF	FREQUENCY(112),RESONANCE(101),VCFTYPE(LOWPASS)
	GO4K_VCF	FREQUENCY(27),RESONANCE(128),VCFTYPE(HIGHPASS)
	GO4K_DST	DRIVE(116), SNHFREQ(128)
	GO4K_DLL	PREGAIN(22),DRY(82),FEEDBACK(118),DAMP(7),FREQUENCY(0),DEPTH(0),DELAY(8),COUNT(8)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(128), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument7)
	GO4K_ENV	ATTAC(0),DECAY(64),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(64),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(NOISE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_VCF	FREQUENCY(128),RESONANCE(128),VCFTYPE(HIGHPASS)
	GO4K_DST	DRIVE(122), SNHFREQ(128)
	GO4K_DLL	PREGAIN(13),DRY(88),FEEDBACK(115),DAMP(8),FREQUENCY(0),DEPTH(0),DELAY(8),COUNT(8)
	GO4K_PAN	PANNING(41)
	GO4K_OUT	GAIN(128), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument8)
	GO4K_ENV	ATTAC(57),DECAY(64),SUSTAIN(64),RELEASE(64),GAIN(128)
	GO4K_VCO	TRANSPOSE(115),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(72),GAIN(128),FLAGS(NOISE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(64), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument9)
	GO4K_ENV	ATTAC(3),DECAY(64),SUSTAIN(64),RELEASE(64),GAIN(128)
	GO4K_VCO	TRANSPOSE(0),DETUNE(60),PHASE(0),GATES(0),COLOR(64),SHAPE(38),GAIN(128),FLAGS(SINE|LFO)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(128),FLAGS(NOISE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_VCF	FREQUENCY(35),RESONANCE(116),VCFTYPE(PEAK)
	GO4K_DST	DRIVE(89), SNHFREQ(128)
	GO4K_DLL	PREGAIN(76),DRY(85),FEEDBACK(64),DAMP(64),FREQUENCY(0),DEPTH(0),DELAY(8),COUNT(8)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(128), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument10)
	GO4K_ENV	ATTAC(40),DECAY(107),SUSTAIN(102),RELEASE(61),GAIN(128)
	GO4K_VCO	TRANSPOSE(76),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(65),GAIN(128),FLAGS(SINE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_DST	DRIVE(86), SNHFREQ(128)
	GO4K_VCF	FREQUENCY(31),RESONANCE(103),VCFTYPE(LOWPASS)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(128), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument11)
	GO4K_ENV	ATTAC(64),DECAY(64),SUSTAIN(64),RELEASE(64),GAIN(128)
	GO4K_VCO	TRANSPOSE(8),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(128),FLAGS(SINE|LFO)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_FSTG	AMOUNT(22),DEST(10*go4k_instrument.size*MAX_VOICES+1*MAX_WORKSPACE_SLOTS*4+1*4+go4k_instrument.workspace)
	GO4K_FOP	OP(FOP_PUSH)
	GO4K_OUT	GAIN(0), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument12)
	GO4K_ENV	ATTAC(64),DECAY(64),SUSTAIN(64),RELEASE(64),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(128),FLAGS(SINE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_DLL	PREGAIN(64),DRY(128),FEEDBACK(64),DAMP(64),FREQUENCY(0),DEPTH(0),DELAY(17),COUNT(1)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(64), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument13)
	GO4K_ENV	ATTAC(64),DECAY(64),SUSTAIN(64),RELEASE(64),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(128),FLAGS(SINE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_DLL	PREGAIN(64),DRY(128),FEEDBACK(64),DAMP(64),FREQUENCY(0),DEPTH(0),DELAY(17),COUNT(1)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(64), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument14)
	GO4K_ENV	ATTAC(59),DECAY(64),SUSTAIN(96),RELEASE(64),GAIN(128)
	GO4K_FST	AMOUNT(128),DEST(0*MAX_WORKSPACE_SLOTS+2)
	GO4K_ENV	ATTAC(0),DECAY(68),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_DST	DRIVE(32), SNHFREQ(128)
	GO4K_FST	AMOUNT(80),DEST(6*MAX_WORKSPACE_SLOTS+1)
	GO4K_FOP	OP(FOP_POP)
	GO4K_VCO	TRANSPOSE(46),DETUNE(64),PHASE(0),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(TRISAW)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(128), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Global)
	GO4K_ACC	ACCTYPE(AUX)
	GO4K_DLL	PREGAIN(40),DRY(128),FEEDBACK(125),DAMP(64),FREQUENCY(0),DEPTH(0),DELAY(0),COUNT(8)
	GO4K_FOP	OP(FOP_XCH)
	GO4K_DLL	PREGAIN(40),DRY(128),FEEDBACK(125),DAMP(64),FREQUENCY(0),DEPTH(0),DELAY(8),COUNT(8)
	GO4K_FOP	OP(FOP_XCH)
	GO4K_ACC	ACCTYPE(OUTPUT)
	GO4K_FOP	OP(FOP_ADDP2)
	GO4K_OUT	GAIN(64), AUXSEND(0)
GO4K_END_PARAMDEF
go4k_synth_parameter_values_end
%ifdef USE_SECTIONS
section .g4kmuc5 data align=1
%else
section .data
%endif
%ifdef GO4K_USE_DLL
global _go4k_delay_times
_go4k_delay_times
	dw 1116
	dw 1188
	dw 1276
	dw 1356
	dw 1422
	dw 1492
	dw 1556
	dw 1618
	dw 1140
	dw 1212
	dw 1300
	dw 1380
	dw 1446
	dw 1516
	dw 1580
	dw 1642
	dw 4863
	dw 19455
%endif
