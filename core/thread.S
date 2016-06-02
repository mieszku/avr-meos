;
; Copyright (c) 2016 Mieszko Mazurek
;

#include "thread.S.h"

rtmp	= 0
rnull	= 1

sreg	= 0x3F
spl	= 0x3D
sph	= 0x3E

.macro	push_context
	push	r0
	push	r1
	push	r2
	push	r3
	push	r4
	push	r5
	push	r6
	push	r7
	push	r8
	push	r9
	push	r10
	push	r11
	push	r12
	push	r13
	push	r14
	push	r15
	push	r16
	push	r17
	push	r18
	push	r19
	push	r20
	push	r21
	push	r22
	push	r23
	push	r24
	push	r25
	push	r26
	push	r27
	push	r28
	push	r29
	push	r30
	push	r31
	in	rtmp,	sreg
	push	rtmp
.endm

.macro	pop_context
	pop	rtmp
	out	sreg,	rtmp
	pop	r31
	pop	r30
	pop	r29
	pop	r28
	pop	r27
	pop	r26
	pop	r25
	pop	r24
	pop	r23
	pop	r22
	pop	r21
	pop	r20
	pop	r19
	pop	r18
	pop	r17
	pop	r16
	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	r11
	pop	r10
	pop	r9
	pop	r8
	pop	r7
	pop	r6
	pop	r5
	pop	r4
	pop	r3
	pop	r2
	pop	r1
	pop	r0
.endm

.global thread_current

.section .data
	current:
	thread_current:
		.word	current
		.word	current
		.word	0	; stack
		.word	0	; stack end
		.byte	0	; flags
		.byte	0	; pending
		.word	name
	
	name:
		.byte 'm'
		.byte 'a'
		.byte 'i'
		.byte 'n'
		.byte 0
		
.global thread_exec
.global thread_kill
.global	thread_fork

.global _switch

.section .text
	thread_exec:
		; r24-r25 run
		; r22-r23 obj
		; r20-r22 name
		; r18-r19 thread
		; r16-r17 stack
		; r14-r15 size
		push	r6
		push	r7
		push	r8
		push	r9
		push	r10
		push	r11
		push	r12
		push	r13

		movw	r12,	r24	; run
		movw	r10,	r22	; obj
		movw	r8,	r20	; name
		movw	r6,	r18	; thread

		movw	r24,	r16
		add	r24,	r14
		adc	r25,	r15
		sbiw	r24,	1
		rcall	thread_fork

		cpse	r24,	rnull
		rjmp	.Lchild

		movw	r30,	r6
		std	Z+OFF_SPTRL,	r22
		std	Z+OFF_SPTRH,	r23
		
		movw	r24,	r6
		rcall	_insert_node
		
		pop	r13
		pop	r12
		pop	r11
		pop	r10
		pop	r9
		pop	r8
		pop	r7
		pop	r6
		ret
	.Lchild:
		call	foo
		movw	r30,	r12
		movw	r24,	r10
		icall
		ret

	thread_fork:
		pop	r19
		pop	r18
		push	r18
		push	r19
		in	r20,	spl
		in	r21,	sph
		out	spl,	r24
		out	sph,	r25
		push	r18
		push	r19
		ser	r24
		ser	r25
		push_context
		in	r22,	spl
		in	r23,	sph
		clr	r24
		clr	r25
		out	spl,	r20
		out	sph,	r21
		ret

	thread_kill:
		ret
	
	_switch:
		push_context
		lds	r28,	current
		lds	r29,	current+1
		in	r20,	spl
		in	r21,	sph
		std	Y+OFF_SPTR,	r20
		std	Y+OFF_SPTR+1,	r21

		ldd	r30,	Y+OFF_NEXT
		ldd	r31,	Y+OFF_NEXT+1
		movw	r30,	r28
		ldd	r20,	Z+OFF_SPTR
		ldd	r21,	Z+OFF_SPTR+1

		out	spl,	r20
		out	sph,	r21
		sts	current,	r30
		sts	current+1,	r31
		pop_context
		ret

	_insert_node:
		push	r28
		push	r29

		lds	r30,	current
		lds	r31,	current+1
		ldd	r28,	Z+OFF_PREVL
		ldd	r29,	Z+OFF_PREVH

		std	Y+OFF_NEXTL,	r24
		std	Y+OFF_NEXTH,	r25
		std	Z+OFF_PREVL,	r24
		std	Z+OFF_PREVH,	r25
		movw	r18,	r30
		movw	r30,	r24

		std	Z+OFF_PREVL,	r28
		std	Z+OFF_PREVH,	r29
		std	Z+OFF_NEXTL,	r18
		std	Z+OFF_NEXTH,	r19

		pop	r29
		pop	r28
		ret
