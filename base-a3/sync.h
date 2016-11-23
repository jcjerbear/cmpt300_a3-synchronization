#ifndef _m_SYNC_H_
#define _m_SYNC_H_

#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "atomic_ops.h"

struct my_spinlock_struct
{
	volatile long unsigned int lockstatus;
	pthread_t tid;
	int lockCount;
};

struct my_mutex_struct
{
	struct timespec sleep;
	volatile long unsigned int lockstatus;
	int expBackoff;
	pthread_t tid;
	int lockCount;
};

struct my_queuelock_struct
{
	volatile long unsigned int nowServing;
	volatile long unsigned int nextTicket;
	pthread_t tid;
	int lockCount;
};

typedef struct my_spinlock_struct my_spinlock_t;
typedef struct my_mutex_struct my_mutex_t;
typedef struct my_queuelock_struct my_queuelock_t;

//spinlock
int my_spinlock_init(my_spinlock_t *lock);
int my_spinlock_destroy(my_spinlock_t *lock);
int my_spinlock_unlock(my_spinlock_t *lock);

int my_spinlock_lockTAS(my_spinlock_t *lock);
int my_spinlock_lockTTAS(my_spinlock_t *lock);
int my_spinlock_trylock(my_spinlock_t *lock);

//mutex lock
int my_mutex_init(my_mutex_t *lock);
int my_mutex_unlock(my_mutex_t *lock);
int my_mutex_destroy(my_mutex_t *lock);

int my_mutex_lock(my_mutex_t *lock);
int my_mutex_trylock(my_mutex_t *lock);

//queue lock
int my_queuelock_init(my_queuelock_t *lock);
int my_queuelock_destroy(my_queuelock_t *lock);
int my_queuelock_unlock(my_queuelock_t *lock);

int my_queuelock_lock(my_queuelock_t *lock);
int my_queuelock_trylock(my_queuelock_t *lock);

#endif
