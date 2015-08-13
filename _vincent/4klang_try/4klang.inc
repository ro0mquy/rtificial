%macro export_func 1
   global _%1
   _%1:
%endmacro
%define SAMPLE_RATE	44100
%define MAX_INSTRUMENTS	8
%define MAX_VOICES 2
%define HLD 1
%define BPM 146.000000
%define MAX_PATTERNS 207
%define PATTERN_SIZE_SHIFT 6
%define PATTERN_SIZE (1 << PATTERN_SIZE_SHIFT)
%define	MAX_TICKS (MAX_PATTERNS*PATTERN_SIZE)
%define	SAMPLES_PER_TICK 566
%define DEF_LFO_NORMALIZE 0.0000551776
%define	MAX_SAMPLES	(SAMPLES_PER_TICK*MAX_TICKS)
;%define 	GO4K_USE_16BIT_OUTPUT
;%define 	GO4K_USE_GROOVE_PATTERN
%define 	GO4K_USE_ENVELOPE_RECORDINGS
%define 	GO4K_USE_NOTE_RECORDINGS
%define 	GO4K_CLIP_OUTPUT
%define 	GO4K_USE_DST
%define 	GO4K_USE_DLL
%define 	GO4K_USE_PAN
%define 	GO4K_USE_GLOBAL_DLL
%define 	GO4K_USE_ENV_CHECK
%define 	GO4K_USE_ENV_MOD_GM
%define 	GO4K_USE_VCO_CHECK
%define 	GO4K_USE_VCO_PHASE_OFFSET
%define 	GO4K_USE_VCO_SHAPE
%define 	GO4K_USE_VCO_MOD_TM
%define 	GO4K_USE_VCO_MOD_DM
%define 	GO4K_USE_VCO_MOD_CM
%define 	GO4K_USE_VCO_MOD_SM
%define 	GO4K_USE_VCF_CHECK
%define 	GO4K_USE_VCF_MOD_FM
%define 	GO4K_USE_VCF_HIGH
%define 	GO4K_USE_VCF_BAND
%define 	GO4K_USE_VCF_PEAK
%define 	GO4K_USE_DST_CHECK
%define		GO4K_USE_DLL_NOTE_SYNC
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
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	120, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 120, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	120, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, HLD, 0, 0, 0, 0, 0, 0, 0, 120, HLD, 0, 0, 0, 0, 0, 0, 120, HLD, HLD, 0, 0, 0, 0, 0, 0, 120, HLD, 0, 0, 0, 0, 0, 0, 0, 120, HLD, 0, 0, 0, 0, 0, 0, 0, 120, HLD, 0, 
	db	0, 0, 0, 0, 0, 120, HLD, 0, 0, 0, 0, 0, 0, 0, 120, HLD, 0, 0, 0, 0, 0, 0, 0, 120, HLD, 119, HLD, 118, HLD, 117, HLD, 116, HLD, HLD, 115, HLD, 114, HLD, 113, HLD, 112, HLD, 111, HLD, 110, HLD, HLD, 109, HLD, 108, HLD, 107, HLD, 106, HLD, 105, HLD, 104, HLD, HLD, 103, HLD, 102, HLD, 
	db	101, HLD, 100, HLD, 99, HLD, HLD, 98, HLD, 97, HLD, 96, HLD, 95, HLD, 94, HLD, 93, HLD, HLD, 92, HLD, 91, HLD, 90, HLD, 89, HLD, 88, HLD, 87, HLD, HLD, 86, HLD, 85, HLD, 84, HLD, 83, HLD, 82, HLD, HLD, 81, HLD, 80, HLD, 79, HLD, 78, HLD, HLD, HLD, 77, HLD, HLD, HLD, HLD, 76, HLD, HLD, HLD, 75, 
	db	HLD, HLD, HLD, 74, HLD, HLD, HLD, HLD, 73, HLD, HLD, HLD, 72, HLD, HLD, HLD, HLD, 71, HLD, HLD, HLD, 70, HLD, HLD, HLD, 69, HLD, HLD, HLD, HLD, 68, HLD, HLD, HLD, 67, HLD, HLD, HLD, 66, HLD, HLD, HLD, HLD, 65, HLD, HLD, HLD, 64, HLD, HLD, HLD, 63, HLD, HLD, HLD, HLD, 62, HLD, HLD, HLD, 61, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 60, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 59, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 58, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, 57, HLD, HLD, 
	db	HLD, 58, HLD, HLD, HLD, 60, HLD, HLD, HLD, HLD, 67, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 74, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 60, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 62, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 64, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 69, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 69, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 47, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 44, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 47, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 45, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, 
	db	0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, 58, HLD, HLD, HLD, 60, HLD, HLD, HLD, HLD, 67, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, 74, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, 
	db	0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, 
	db	0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, 
	db	57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 44, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 47, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 79, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 91, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 91, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 93, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 91, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 96, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 93, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 88, HLD, 89, 90, HLD, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 105, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 0, 100, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 105, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 100, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 108, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 107, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 107, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 105, HLD, HLD, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 100, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 105, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 100, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 107, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 107, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 105, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 100, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 105, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 105, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 100, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 107, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 107, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 105, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 105, HLD, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	41, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	83, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 103, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 103, HLD, HLD, HLD, 0, 0, 0, 0, 0, 100, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	103, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 103, HLD, HLD, HLD, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 108, HLD, HLD, HLD, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, 0, 0, 0, 0, 100, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 100, HLD, HLD, HLD, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 105, HLD, HLD, HLD, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	105, HLD, HLD, HLD, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 103, HLD, HLD, HLD, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 107, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 107, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 107, HLD, HLD, HLD, 0, 0, 0, 0, 0, 107, HLD, HLD, HLD, 0, 0, 0, 0, 107, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 108, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 105, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	105, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 
	db	86, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 89, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	81, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 89, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 84, HLD, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	88, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, 
	db	HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 86, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 
	db	0, 0, 0, 86, HLD, HLD, HLD, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, HLD, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 
	db	HLD, 0, 81, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 
	db	0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 
	db	0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 
	db	0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 83, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 
	db	0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 
	db	0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 
	db	0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 
	db	0, 86, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 
	db	88, HLD, HLD, HLD, HLD, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 
	db	88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 
	db	84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 
	db	81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 
	db	84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 
	db	83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 
	db	88, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 83, 
	db	HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 79, HLD, HLD, HLD, HLD, 0, 79, 
	db	0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 84, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 
	db	0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, 
	db	HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 
	db	0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, 
	db	HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, 
	db	HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 83, HLD, 
	db	HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 
	db	0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, 
	db	HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 
	db	0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 
	db	0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, 
	db	HLD, HLD, HLD, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 
	db	0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 81, HLD, HLD, 
	db	HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, 
	db	HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, 
	db	HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, 
	db	HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, 
	db	HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 
	db	0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 
	db	0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, 
	db	HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 
	db	0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, 
	db	HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, 
	db	HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 
	db	HLD, 0, 88, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 88, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, 
	db	HLD, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, HLD, 108, 0, 0, 0, 0, 0, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, 103, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, 
	db	HLD, HLD, HLD, HLD, HLD, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, 103, 0, 0, 0, 0, 0, 103, 0, 0, 0, 0, 0, 100, HLD, HLD, HLD, HLD, 0, 100, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, 0, 103, 0, 0, 0, 0, 103, 0, 0, 0, 0, 0, 105, 
	db	HLD, HLD, HLD, HLD, 0, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, 108, 0, 0, 0, 0, 0, 108, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, 0, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, 103, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, 103, 0, 0, 0, 0, 0, 103, 0, 0, 0, 0, 0, 100, HLD, HLD, HLD, HLD, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, HLD, HLD, HLD, HLD, HLD, 103, 
	db	0, 0, 0, 0, 0, 103, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, 0, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, HLD, 108, 0, 0, 0, 0, 108, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, 0, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 
	db	HLD, HLD, HLD, HLD, HLD, 103, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 107, HLD, HLD, HLD, HLD, 107, 0, 0, 0, 0, 0, 107, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, 0, 105, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 107, HLD, HLD, HLD, HLD, HLD, 107, 0, 0, 0, 0, 0, 107, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, 0, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, HLD, HLD, HLD, HLD, HLD, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 103, HLD, HLD, HLD, HLD, HLD, 103, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, HLD, HLD, HLD, HLD, HLD, 105, 0, 0, 0, 0, 0, 105, 0, 0, 0, 0, 0, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 88, HLD, HLD, HLD, HLD, HLD, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, 
	db	0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 
	db	0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 
	db	0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 
	db	0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 83, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 
	db	0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 
	db	0, 83, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 
	db	79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 86, 0, 0, 0, 0, 0, 
	db	83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 
	db	84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 
	db	88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 
	db	88, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 
	db	81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 
	db	81, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 79, 
	db	HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 88, 0, 0, 0, 0, 0, 83, 
	db	HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 88, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, 
	db	HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, 
	db	HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, 
	db	HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 
	db	0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, 
	db	HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, 0, 89, 0, 
	db	0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, 
	db	HLD, HLD, HLD, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, 
	db	HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, 
	db	HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, 
	db	HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 0, 0, 0, 83, HLD, 
	db	HLD, HLD, HLD, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 84, HLD, 
	db	HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, 0, 88, 0, 0, 
	db	0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, 
	db	HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 
	db	0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 89, HLD, HLD, HLD, HLD, 0, 89, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, 0, 81, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 
	db	0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, 
	db	HLD, HLD, 0, 79, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 88, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 88, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 0, 86, 0, 0, 
	db	0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 86, HLD, HLD, HLD, HLD, 86, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 79, HLD, HLD, HLD, HLD, 0, 79, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, 
	db	HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 58, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 79, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 79, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 84, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, 0, 0, 0, 0, 83, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 79, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 
	db	81, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
go4k_patterns_end
%ifdef USE_SECTIONS
section .g4kmuc2 data align=1
%else
section .data
%endif
go4k_pattern_lists
Instrument0List		db	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 16, 17, 18, 19, 20, 0, 0, 0, 0, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 0, 0, 0, 0, 0, 0, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 48, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 71, 72, 73, 74, 75, 76, 0, 0, 0, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 0, 0, 0, 0, 0, 0, 0, 87, 88, 89, 90, 43, 91, 45, 92, 93, 94, 95, 96, 97, 98, 99, 53, 100, 101, 102, 103, 104, 105, 60, 106, 62, 107, 108, 109, 110, 111, 112, 113, 114, 115, 113, 116, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 117, 118, 119, 120, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 0, 0, 0, 0, 
Instrument1List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 105, 143, 106, 144, 107, 145, 146, 147, 148, 149, 150, 151, 38, 0, 0, 0, 0, 0, 0, 0, 0, 152, 128, 153, 154, 155, 156, 157, 158, 159, 160, 161, 138, 162, 140, 163, 164, 105, 165, 166, 144, 167, 168, 146, 169, 148, 170, 150, 171, 172, 113, 173, 174, 175, 0, 0, 87, 176, 177, 178, 179, 180, 42, 181, 44, 182, 16, 183, 161, 184, 0, 0, 0, 0, 185, 54, 186, 56, 187, 188, 146, 189, 190, 170, 191, 192, 193, 194, 173, 195, 196, 0, 0, 0, 0, 0, 0, 0, 0, 0, 197, 44, 198, 199, 183, 125, 200, 127, 201, 129, 202, 203, 132, 204, 205, 135, 206, 137, 207, 190, 140, 208, 142, 209, 210, 106, 211, 110, 212, 213, 147, 214, 215, 216, 116, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 143, 106, 144, 107, 145, 146, 147, 148, 149, 0, 0, 0, 0, 
Instrument2List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 0, 0, 0, 0, 0, 0, 291, 292, 293, 0, 294, 295, 296, 0, 0, 297, 298, 299, 0, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 0, 319, 320, 321, 322, 323, 324, 325, 326, 0, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 0, 0, 0, 0, 
Instrument3List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 424, 425, 426, 427, 428, 429, 430, 431, 432, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 0, 0, 0, 0, 
Instrument4List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 529, 530, 531, 532, 533, 534, 535, 536, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument5List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 571, 572, 573, 574, 575, 576, 577, 578, 72, 73, 74, 75, 579, 580, 581, 582, 583, 78, 79, 80, 584, 585, 67, 586, 587, 588, 589, 0, 0, 0, 0, 0, 0, 0, 0, 152, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 580, 581, 582, 583, 78, 79, 80, 584, 585, 67, 586, 607, 608, 0, 0, 0, 0, 609, 610, 611, 612, 613, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 71, 72, 73, 74, 75, 178, 0, 0, 0, 77, 78, 79, 80, 584, 585, 67, 586, 587, 614, 0, 0, 0, 0, 0, 0, 0, 87, 615, 616, 617, 618, 619, 620, 621, 572, 573, 574, 575, 576, 577, 578, 72, 73, 74, 622, 623, 624, 625, 626, 604, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument6List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 571, 572, 573, 574, 639, 149, 0, 0, 0, 0, 640, 75, 579, 641, 146, 582, 583, 78, 128, 642, 584, 585, 643, 644, 587, 588, 589, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 645, 581, 582, 583, 625, 646, 80, 584, 647, 648, 586, 587, 649, 650, 651, 652, 653, 654, 610, 611, 655, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 71, 72, 73, 656, 657, 0, 0, 0, 0, 77, 78, 79, 658, 146, 585, 67, 586, 201, 0, 0, 0, 0, 0, 0, 0, 0, 87, 615, 616, 617, 659, 660, 620, 621, 661, 662, 574, 575, 663, 664, 578, 72, 73, 665, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument7List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 666, 667, 668, 669, 670, 625, 0, 0, 0, 671, 672, 673, 674, 675, 676, 677, 676, 665, 678, 679, 680, 681, 682, 0, 0, 0, 683, 684, 685, 686, 687, 688, 689, 690, 691, 0, 0, 0, 0, 0, 
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
	db GO4K_FST_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument2)
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
	db GO4K_DST_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument3)
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
	db GO4K_DST_ID
	db GO4K_DLL_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument4)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FST_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_FST_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_DLL_ID
	db GO4K_PAN_ID
	db GO4K_DLL_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument5)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
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
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument7)
	db GO4K_ENV_ID
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FST_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_FST_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_DLL_ID
	db GO4K_PAN_ID
	db GO4K_DLL_ID
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
	GO4K_ENV	ATTAC(0),DECAY(70),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_DST	DRIVE(32), SNHFREQ(128)
	GO4K_FST	AMOUNT(80),DEST(6*MAX_WORKSPACE_SLOTS+1)
	GO4K_FOP	OP(FOP_POP)
	GO4K_VCO	TRANSPOSE(46),DETUNE(64),PHASE(0),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(TRISAW)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(89), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument1)
	GO4K_ENV	ATTAC(0),DECAY(76),SUSTAIN(0),RELEASE(0),GAIN(32)
	GO4K_FST	AMOUNT(128),DEST(0*MAX_WORKSPACE_SLOTS+2)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(64),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(NOISE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_VCF	FREQUENCY(80),RESONANCE(128),VCFTYPE(LOWPASS)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(95), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument2)
	GO4K_ENV	ATTAC(32),DECAY(76),SUSTAIN(0),RELEASE(32),GAIN(81)
	GO4K_FST	AMOUNT(106),DEST(0*MAX_WORKSPACE_SLOTS+2)
	GO4K_FST	AMOUNT(84),DEST(11*MAX_WORKSPACE_SLOTS+4)
	GO4K_ENV	ATTAC(0),DECAY(60),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_FST	AMOUNT(50),DEST(7*MAX_WORKSPACE_SLOTS+6)
	GO4K_FOP	OP(FOP_POP)
	GO4K_VCO	TRANSPOSE(64),DETUNE(70),PHASE(40),GATES(85),COLOR(64),SHAPE(80),GAIN(126),FLAGS(SINE)
	GO4K_VCO	TRANSPOSE(64),DETUNE(48),PHASE(48),GATES(85),COLOR(64),SHAPE(96),GAIN(128),FLAGS(SINE)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_VCF	FREQUENCY(30),RESONANCE(128),VCFTYPE(LOWPASS)
	GO4K_DST	DRIVE(107), SNHFREQ(128)
	GO4K_VCF	FREQUENCY(35),RESONANCE(128),VCFTYPE(ALLPASS)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(94), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument3)
	GO4K_ENV	ATTAC(0),DECAY(48),SUSTAIN(128),RELEASE(64),GAIN(128)
	GO4K_FST	AMOUNT(116),DEST(0*MAX_WORKSPACE_SLOTS+2)
	GO4K_ENV	ATTAC(0),DECAY(56),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_VCO	TRANSPOSE(89),DETUNE(64),PHASE(0),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(SINE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_VCF	FREQUENCY(95),RESONANCE(53),VCFTYPE(BANDPASS)
	GO4K_DST	DRIVE(118), SNHFREQ(128)
	GO4K_DLL	PREGAIN(128),DRY(128),FEEDBACK(127),DAMP(8),FREQUENCY(0),DEPTH(0),DELAY(0),COUNT(1)
	GO4K_VCF	FREQUENCY(81),RESONANCE(51),VCFTYPE(LOWPASS)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(38)
	GO4K_OUT	GAIN(12), AUXSEND(36)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument4)
	GO4K_ENV	ATTAC(64),DECAY(70),SUSTAIN(70),RELEASE(88),GAIN(128)
	GO4K_VCO	TRANSPOSE(69),DETUNE(64),PHASE(0),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(SINE|LFO)
	GO4K_FST	AMOUNT(72),DEST(11*MAX_WORKSPACE_SLOTS+2)
	GO4K_FOP	OP(FOP_POP)
	GO4K_VCO	TRANSPOSE(73),DETUNE(64),PHASE(0),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(SINE|LFO)
	GO4K_FST	AMOUNT(56),DEST(12*MAX_WORKSPACE_SLOTS+2)
	GO4K_FST	AMOUNT(56),DEST(12*MAX_WORKSPACE_SLOTS+5)
	GO4K_FOP	OP(FOP_POP)
	GO4K_VCO	TRANSPOSE(77),DETUNE(64),PHASE(0),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(SINE|LFO)
	GO4K_FST	AMOUNT(72),DEST(13*MAX_WORKSPACE_SLOTS+2)
	GO4K_FOP	OP(FOP_POP)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(85),COLOR(0),SHAPE(64),GAIN(128),FLAGS(TRISAW)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(85),COLOR(0),SHAPE(64),GAIN(128),FLAGS(TRISAW)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(85),COLOR(0),SHAPE(64),GAIN(128),FLAGS(TRISAW)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_VCF	FREQUENCY(104),RESONANCE(96),VCFTYPE(LOWPASS)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_DLL	PREGAIN(64),DRY(64),FEEDBACK(64),DAMP(0),FREQUENCY(0),DEPTH(0),DELAY(17),COUNT(1)
	GO4K_PAN	PANNING(64)
	GO4K_DLL	PREGAIN(128),DRY(0),FEEDBACK(0),DAMP(0),FREQUENCY(0),DEPTH(0),DELAY(18),COUNT(1)
	GO4K_OUT	GAIN(71), AUXSEND(64)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument5)
	GO4K_ENV	ATTAC(0),DECAY(64),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(64),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(NOISE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_VCF	FREQUENCY(128),RESONANCE(128),VCFTYPE(HIGHPASS)
	GO4K_PAN	PANNING(73)
	GO4K_OUT	GAIN(99), AUXSEND(100)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument6)
	GO4K_ENV	ATTAC(0),DECAY(71),SUSTAIN(0),RELEASE(0),GAIN(32)
	GO4K_FST	AMOUNT(128),DEST(0*MAX_WORKSPACE_SLOTS+2)
	GO4K_ENV	ATTAC(0),DECAY(70),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_DST	DRIVE(8), SNHFREQ(128)
	GO4K_FST	AMOUNT(76),DEST(7*MAX_WORKSPACE_SLOTS+1)
	GO4K_FOP	OP(FOP_POP)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(64),GATES(85),COLOR(64),SHAPE(64),GAIN(64),FLAGS(NOISE)
	GO4K_VCO	TRANSPOSE(59),DETUNE(109),PHASE(64),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(SINE)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(64), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument7)
	GO4K_ENV	ATTAC(0),DECAY(70),SUSTAIN(0),RELEASE(70),GAIN(128)
	GO4K_ENV	ATTAC(0),DECAY(80),SUSTAIN(0),RELEASE(80),GAIN(128)
	GO4K_FST	AMOUNT(96),DEST(18*MAX_WORKSPACE_SLOTS+4)
	GO4K_FST	AMOUNT(96),DEST(19*MAX_WORKSPACE_SLOTS+4)
	GO4K_FOP	OP(FOP_POP)
	GO4K_FOP	OP(FOP_PUSH)
	GO4K_VCO	TRANSPOSE(72),DETUNE(64),PHASE(0),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(SINE|LFO)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FST	AMOUNT(32),DEST(13*MAX_WORKSPACE_SLOTS+5)
	GO4K_FOP	OP(FOP_POP)
	GO4K_VCO	TRANSPOSE(96),DETUNE(64),PHASE(0),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(SINE|LFO)
	GO4K_FST	AMOUNT(80),DEST(15*MAX_WORKSPACE_SLOTS+2)
	GO4K_FOP	OP(FOP_POP)
	GO4K_VCO	TRANSPOSE(76),DETUNE(64),PHASE(0),GATES(85),COLOR(128),SHAPE(64),GAIN(128),FLAGS(PULSE)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(85),COLOR(64),SHAPE(64),GAIN(128),FLAGS(PULSE)
	GO4K_VCO	TRANSPOSE(76),DETUNE(64),PHASE(0),GATES(85),COLOR(0),SHAPE(64),GAIN(128),FLAGS(TRISAW)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_VCF	FREQUENCY(64),RESONANCE(96),VCFTYPE(LOWPASS)
	GO4K_VCF	FREQUENCY(64),RESONANCE(96),VCFTYPE(LOWPASS)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_DLL	PREGAIN(64),DRY(128),FEEDBACK(64),DAMP(32),FREQUENCY(0),DEPTH(0),DELAY(19),COUNT(1)
	GO4K_PAN	PANNING(64)
	GO4K_DLL	PREGAIN(128),DRY(0),FEEDBACK(0),DAMP(0),FREQUENCY(0),DEPTH(0),DELAY(18),COUNT(1)
	GO4K_OUT	GAIN(44), AUXSEND(32)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Global)
	GO4K_ACC	ACCTYPE(AUX)
	GO4K_DLL	PREGAIN(28),DRY(0),FEEDBACK(114),DAMP(40),FREQUENCY(0),DEPTH(0),DELAY(9),COUNT(8)
	GO4K_FOP	OP(FOP_XCH)
	GO4K_DLL	PREGAIN(64),DRY(128),FEEDBACK(64),DAMP(64),FREQUENCY(0),DEPTH(0),DELAY(9),COUNT(8)
	GO4K_FOP	OP(FOP_XCH)
	GO4K_ACC	ACCTYPE(OUTPUT)
	GO4K_FOP	OP(FOP_ADDP2)
	GO4K_OUT	GAIN(12), AUXSEND(0)
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
	dw 0
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
	dw 27184
	dw 512
	dw 13592
%endif
