;
; Copyright (c) 2016 Mieszko Mazurek
;

rtmp	= 0
rnull	= 1

.global	mutex_lock
.global	mutex_unlock
.global	mutex_unlock_later

.section .text
	mutex_lock:
		push	r28
		push	r29
		movw	r28,	r24
	.Ltryacquire:
		call	__hwport_disable
		ld	rtmp,	Y
		cpse	rtmp,	rnull
		rjmp	.Ltryagain
		com	rtmp
		st	Y,	rtmp
		call	__hwport_enable
		pop	r29
		pop	r28
		;jmp	__hwport_check_int
		call	__hwport_get_int
		cpse	r24,	rnull
		jmp	system_yield
		ret
	.Ltryagain:
		call	system_yield
		rjmp	.Ltryacquire
	
	mutex_unlock:
		movw	r26,	r24
		st	X,	rnull
		clr	r24
		ret
	
	mutex_unlock_later:
		movw	r20,	r22
		movw	r22,	r24
		clr	r18
		clr	r19
		ldi	r24,	lo8(gs(mutex_unlock))
		ldi	r25,	hi8(gs(mutex_unlock))
		jmp	task_register
