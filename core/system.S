;
; Copyright (c) 2016 Mieszko Mazurek
;

#include "config.h"

sreg	= 0x3F
rnull	= 1
rtmp	= 0

.global	system_get_time
.global	system_sleep
.global system_yield
.global	system_enter_critical
.global	system_exit_critical
.global	system_sei
.global	system_rand

.global rand_seed
.weak	rand_seed

.global __system_awake
.global	__system_clock_mono
.global	__system_clock

.macro	inc_systime	rtmp
	lds	\rtmp,	systime
	subi	\rtmp,	0xFF
	sts	systime,	\rtmp
	lds	\rtmp,	systime+1
	sbci	\rtmp,	0xFF
	sts	systime+1,	\rtmp
	lds	\rtmp,	systime+2
	sbci	\rtmp,	0xFF
	sts	systime+2,	\rtmp
	lds	\rtmp,	systime+3
	sbci	\rtmp,	0xFF
	sts	systime+3,	\rtmp
.endm

.section .bss
	systime:
		.word	0
		.word	0

	tick:
		.byte	0
	
.section .text
	system_get_time:
		rcall	_get_time
		#ifdef __system_timer_x2__
		lsr	r25
		ror	r24
		ror	r23
		ror	r22
		#endif
		ret
	
	system_sleep:
		#ifdef __system_timer_x2__
		lsl	r24
		rol	r25
		#endif
		movw	r26,	r24
		rcall	_get_time
		push	r10
		push	r11
		push	r12
		push	r13
		movw	r10,	r22
		movw	r12,	r24
		add	r10,	r26
		adc	r11,	r27
		adc	r12,	rnull
		adc	r13,	rnull
	.Lsleep:
		rcall	_get_time
		cp	r22,	r10
		cpc	r23,	r11
		cpc	r24,	r12
		cpc	r25,	r13
		brsh	.Lwakeup
		rcall	system_yield
		rjmp	.Lsleep
	.Lwakeup:
		pop	r13
		pop	r12
		pop	r11
		pop	r10
		ret

	system_yield:
		jmp	__hwport_sync_clock

	system_enter_critical:
		jmp 	__hwport_disable

	system_exit_critical:
		call	__hwport_enable
		jmp	__hwport_check_int
	
	system_sei:
		sei
		call	__hwport_check_int
		ret

	system_rand:
		rcall	_get_time
		mul	r22,	r23
		swap	r0
		adc	r23,	r0
		or	r23,	r1
		clr	r1
		eor	r24,	r23
		neg	r25
		add	r24,	r25
		neg	r25
		swap	r25
		eor	r24,	r25
		push	r24
		call	rand_seed
		pop	r25
		eor	r24,	r25
		ret
	
	rand_seed:
		rcall	_get_time
		swap	r24
		adc	r22,	r23
		swap	r22
		eor	r24,	r25
		eor	r24,	r23
		sbc	r24,	r22
		ret

	__system_awake:
		jmp	__hwport_callback

	__system_clock_mono:
		push	r16
		in	r16,	sreg
		push	r16
		ser	r16
		sts	tick,	r16
		inc_systime	r16
		pop	r16
		out	sreg,	r16
		pop	r16
		ret
	
	__system_clock:
		jmp	__thread_yield
	
	_get_time:
		sts	tick,	rnull
		lds	r25,	systime+3
		lds	r24,	systime+2
		lds	r23,	systime+1
		lds	r22,	systime
		lds	rtmp,	tick
		cpse	rtmp,	rnull
		rjmp	_get_time
		ret

.global __do_clear_bss
