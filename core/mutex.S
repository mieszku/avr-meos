;
; Copyright (c) 2016 Mieszko Mazurek
;

rtmp	= 0
rnull	= 1

.global	mutex_lock
.global	mutex_unlock

.section .text
	mutex_lock:
		movw	r26,	r24
	.Ltryacquire:
		call	system_enter_critical
		ld	rtmp,	X
		cpse	rtmp,	rnull
		rjmp	.Ltryagain
		com	rtmp
		st	X,	rtmp
		jmp	system_exit_critical
	.Ltryagain:
		call	system_exit_critical
		call	system_yield
		rjmp	.Ltryacquire
		ret
	
	mutex_unlock:
		movw	r26,	r24
		st	X,	rnull
		ret
