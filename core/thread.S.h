/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#define OFF_NEXT	 0x0
#define OFF_PREV	 0x2
#define OFF_SPTR	 0x4
#define OFF_SPND	 0x6
#define OFF_FLAGS	 0x8
#define OFF_NAME	 0x9

#define CTX_SIZE	 35
#define THREAD_SIZE	 11
#define STACK_MARGIN	 15

#define OFF_NEXTL	 OFF_NEXT
#define OFF_NEXTH	 OFF_NEXT+1
#define OFF_PREVL	 OFF_PREV
#define OFF_PREVH	 OFF_PREV+1
#define OFF_SPTRL	 OFF_SPTR
#define OFF_SPTRH	 OFF_SPTR+1
#define OFF_SPNDL	 OFF_SPND
#define OFF_SPNDH	 OFF_SPND+1
#define OFF_NAMEL	 OFF_NAME
#define OFF_NAMEH	 OFF_NAME+1

#define FLAG_FREE	 0x01
