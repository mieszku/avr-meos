Copyright (c) 2016 Mieszko Mazurek

avr meos is a 'simple' library for ATmega microcontrollers that main aim is to
provide REAL threading system and some additional features like dedicated memory 
allocator, mutexes or basic functions to count time, register tasks and
other things.

Currently it supports only ATmega32 using timer1 or timer0 and timer2, and ATmega328p
using timer1 but it would works as well on one of the lower resolution timers.

In general, most significant code was written in assembler avoiding hardware-depends 
functions, that were moved to assembler files called 'ports', each for one specific
microcontroller.
Each port have to implement functions to initialize system timer, synchronize
thread yield clock, get tick/int flag, clear int flag and enable/disable interrupts.

#config file and system clock

core/config.h file contain some confuguration definitions, among others
target MCU, timer which shall be used as system clock and system clock multiplier - 
base frequency is 1000Hz, but it can be multiplied by 2, 4 or even 8.

I recommend to use x2 multiplier or none because x4 and x8 may causes unexpected
errors, restarts or just stop work. However I tested atmega328p on arduino using x8 
multiplier and it worked well with some threads, tasks and text lcd writing using mutexes. 
Although, Same code didn't seem to work on atmega32 on which I got 2000Hz as the 
highest "stable" frequency.

Thread at every moment can as well hand over its time by call system_yield. 
system_yield for first disables system clock interrupt, then synchronize clock to 
make sure that next yield will happen, at the latest, after one full clock period,
and switch to next thread just like system interrupt do.

#memalloc

Another important feature is dedicated memory allocator - memalloc. I had to make
it because malloc from stdlib didn't work with thread other that main one.
Potentially, it may work bit slower than malloc, but I think it could be
better in memory managing and avoiding fragmentation.

#running thread

The simplest way to run new thread, is call thread_run_alloc. thread_run_alloc 
takes as first argument pointer to function (prototype below), argument,
then name of thread and size of stack. thread_run_alloc will use memalloc to
allocate desired size of memory and return if memalloc returned NULL, or
call thread_run otherwise. thread_run takes one more parameter: 
pointer to memory (and size of it at the end) which shall be used as system stack,
and thread context data. Then thread_run jumps into thread_exec.
thread_exec takes another one argument: pointer to thread context data, 
which keeps thread flags, pointer to stack (updated after every yield),
stack limit (uses to detect whether it was exceed), thread name and pointer to next
and previous thread. This is because all threads are stored as fast linked list.

void thread_function (void* obj);

...

thread_run_alloc (thread_function, NULL, "my thread", 40);

...

#tasks

To make something done after specific delay, or periodicaly every time, you can
use tasks system. To register new task, we need to create task function (prototype below),
and give it as first argument to task_register, and then argument to it, then task delay
and (optionally) task period. First task call will happen after $delay milliseconds,
and it will repeats every $period time as long as task function returns value other than zero.

uint8_t task_function (void* obj);

...

task_register (task_function, NULL, "my task", 1000, 500);

...

#mutexes

If there is a code runned by some threads, that should be executed by only one at once,
mutex can be used to lock that section at the beggining, and unlock at the end.
If one thread tried to lock mutex which had been locked before, it will wait as long as
mutex will be locked to make sure that one thread cannot start do something until
other finished it.

mutex_t mtx;

...

mutex_lock (&mtx);

// do something

mutex_unlock (&mtx);

...

#panic mode

Sometimes when something goes wrong, for example memalloc failed without checking return
value, or one thread stack was smashed, panic mode could be very helpful. Entering to
panic mode disables all threads except current one and system thread. Then stack pointer is
safely restored to value saved at initialization, and panic handler is called.
panic function (prototype below) shall be implemented by default like main. It takes
only error code which can be used to detect what happened. Possible error codes are defined
in core/error.h. Panic mode keeps last thread_current state, so it also can be used as a hint
to qualify what/where/when went wrong. Two most often panic errors are xmemalloc fail and stack
smash. First shows that there was no enough memory to allocate desired block, and second
that during last thread switch, stack pointer exceed admissible limit. After panic handler
return, it falls into infinite loop.

void panic (error_t err)
{
	switch (err) {
	case ERROR_XMEMALLOC: break;
	case ERROR_STACK_SMASH: break;
	case ERROR_PURE_VIRTUAL: break;
	case ERROR_INVALID_ARGUMENT: break;
	default:
		// unknown error
	}
}
