/*
 * NAME, etc.
 *
 * sync.c
 *
 */

#define _REENTRANT

#include "sync.h"


/*
 * Spinlock routines
 */

int my_spinlock_init(my_spinlock_t *lock)
{
	//can be allocated dynamically
}

int my_spinlock_destroy(my_spinlock_t *lock)
{
	//may do nothing or just deallocate it
}

int my_spinlock_unlock(my_spinlock_t *lock)
{
	//
}

int my_spinlock_lockTAS(my_spinlock_t *lock)
{
	while(tas(lock)) {}
	return 1;
}

int my_spinlock_lockTTAS(my_spinlock_t *lock)
{
	while(1){
		while(!tas(lock)){
			return 1;
		}
	}
}

//for the recursive part we need keep track of the amount of one thread that is taking the lock just so we know how many to unlock it afterwards
//

int my_spinlock_trylock(my_spinlock_t *lock)
{
	//this is not blocking/waiting
	//probably for recursion
	//basically just check if the lock is free, if yes then return true and take the lock
	//if no just return false 
}


/*
 * Mutex routines
 */

int my_mutex_init(my_mutex_t *lock)
{
}

int my_mutex_destroy(my_mutex_t *lock)
{
}

int my_mutex_unlock(my_mutex_t *lock)
{
}

int my_mutex_lock(my_mutex_t *lock)
{
}

int my_mutex_trylock(my_mutex_t *lock)
{
}

/*
 * Queue Lock
 */

int my_queuelock_init(my_queuelock_t *lock)
{
}

int my_queuelock_destroy(my_queuelock_t *lock)
{
}

int my_queuelock_unlock(my_queuelock_t *lock)
{
}

int my_queuelock_lock(my_queuelock_t *lock)
{
}

int my_queuelock_trylock(my_queuelock_t *lock)
{
}

