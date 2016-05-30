;
; Copyright (c) 2016 Mieszko Mazurek
;

.global	system_enter_critical
.global	system_exit_critical

.global	__system_clock_mono

.section .text
	system_enter_critical:
		ret
	
	system_exit_critical:
		ret

	__system_clock_mono:
		ret
