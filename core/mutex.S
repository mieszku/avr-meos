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
		movw	r26,	r24
	.Ltryacquire:
		call	__hwport_disable
		ld	rtmp,	X
		cpse	rtmp,	rnull
		rjmp	.Ltryagain
		com	rtmp
		st	X,	rtmp
		call	__hwport_enable
		jmp	__hwport_check_int
	.Ltryagain:
		call	system_yield
		rjmp	.Ltryacquire
		ret
	
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
