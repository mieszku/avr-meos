/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "task.h"
#include "system.h"
#include "mutex.h"

#include "memutils.h"

#define MAX_TASKS	16

struct task
{
	task_func_t	func;
	void* 		object;
	uint32_t	time;
	uint16_t	period;
};

static struct task 	heap [MAX_TASKS];
static volatile uint8_t	heap_size;
static mutex_t		mutex;

static void push_task (struct task *t)
{
	uint8_t i, p;

	mutex_lock (&mutex);

	memcopy (heap + heap_size, t, sizeof (struct task));
	i = heap_size++;
	while (i && heap [i].time < heap [p = (i >> 1) + (i & 1) - 1].time) {
		memswap (heap + i, heap + p, sizeof (struct task));
		i = p;
	}

	mutex_unlock (&mutex);
}

static void bubble_task (void)
{
	uint8_t c, i = 0;

	while ((c = (i << 1) + 1) < heap_size) {
		if (c + 1 < heap_size && heap [c + 1].time < heap [c].time)
			c++;

		if (heap [c].time < heap [i].time)
			memswap (heap + i, heap + c, sizeof (struct task));
		else
			break;
		i = c;
	}
}

void task_register (task_func_t	task,
		    void*	object,
		    uint16_t	delay,
		    uint16_t	period)
{
	struct task t = {
		.func 	= task,
		.object = object,
		.time	= system_get_time () + delay,
		.period	= period
	};
	push_task (&t);
}

void __task_handle (void)
{
	char loop;

	while (heap_size && heap->time <= system_get_time ()) {
		mutex_lock (&mutex);

		if (heap->period) {
			mutex_unlock (&mutex);
			loop = heap->func (heap->object);
			mutex_lock (&mutex);
		} else {
			loop = 0;
			heap->func (heap->object);
		}

		if (loop) {
			heap->time += heap->period;
		} else {
			memcopy (heap, heap + --heap_size, sizeof (struct task));
		}

		bubble_task ();

		mutex_unlock (&mutex);
	}
}
