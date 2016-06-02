;
; Copyright (c) 2016 Mieszko Mazurek
;

.section .init8
	init_system:
		call	__hwport_init

.section .fini8
	call	thread_kill
