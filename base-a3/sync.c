#define _REENTRANT
#include "sync.h"

//Spinlock routines
int my_spinlock_init(my_spinlock_t *lock)
{
	//can be allocated dynamically
	//simply initialize the lockstatus as 0
	lock->lockstatus = 0;
	lock->tid = 0;
	lock->lockCount = 0;
}

int my_spinlock_destroy(my_spinlock_t *lock)
{
	//may do nothing or just deallocate it
	return 0;
}

int my_spinlock_unlock(my_spinlock_t *lock)
{
	//if tid are equal
	if(lock->tid == pthread_self())
	{
		lock->lockCount--;
		if(lock->lockCount == 0)
		{
			//reset tid
			lock->tid = 0;
			//simply set the lockstatus back into 0 again
			//so other threads can obtain the lock
			lock->lockstatus = 0;
		}	
	}
}

int my_spinlock_lockTAS(my_spinlock_t *lock)
{	
	//tas() == 0 if the lock is free
	//tas() != 0 if the lock is occupied
	//keep looping until you get a lock

	//check if the lock was acquired by itself
	if(lock->tid != pthread_self()){
		while(tas(&lock->lockstatus) != 0) {}
		lock->lockCount++;
		lock->tid = pthread_self();
	}else{
		lock->lockCount++;
	}	
	return 0;
}

int my_spinlock_lockTTAS(my_spinlock_t *lock)
{
	//keep looping and see if the lock is free(lockstatus==0)
	//if the lock is free then we do tas
	while(1)
	{
		if(lock->tid == pthread_self()){
			lock->lockCount++;
			break;
		}else if(lock->lockstatus == 0){
			if(tas(&lock->lockstatus) == 0){
				lock->lockCount++;
				lock->tid = pthread_self();
				break;	
			}
		}
	}
	return 0;
}

//for the recursive part we need keep track of the amount of one thread 
//that is taking the lock just so we know how many to unlock it afterwards
int my_spinlock_trylock(my_spinlock_t *lock)
{
	//this is not blocking/waiting
	//probably for recursion
	//basically just check if the lock is free
	//if yes then return true and take the lock
	//if no just return false
	if(tas(&lock->lockstatus) == 0)
	{
		lock->lockCount++;
		lock->tid = pthread_self();
	}
}


//Mutex routines
int my_mutex_init(my_mutex_t *lock)
{
	//simply initialize the lockstatus as 0
	lock->lockstatus = 0;
	lock->expBackoff = rand()%10+1;
	lock->lockCount = 0;
	lock->tid = 0;
}

int my_mutex_destroy(my_mutex_t *lock)
{
	//may do nothing or just deallocate it
	return 0;
}

int my_mutex_unlock(my_mutex_t *lock)
{
	//simply set the lockstatus back into 0 again
	//so other threads can obtain the lock
	if(lock->tid == pthread_self())
	{
		lock->lockCount--;
		if(lock->lockCount == 0)
		{
			lock->tid = 0;
			lock->lockstatus = 0;	
		}
	}
}

int my_mutex_lock(my_mutex_t *lock)
{
	if(lock->tid != pthread_self()){
		//when lock is occupied, keep looping
		while(tas(&lock->lockstatus) != 0)
		{
			lock->sleep.tv_nsec = lock->expBackoff;
			nanosleep(&lock->sleep, NULL);
			if(lock->expBackoff >= 10000000) lock->expBackoff = rand()%10+1;
			else if(lock->expBackoff < 10000000) lock->expBackoff *= lock->expBackoff;
		}
		lock->tid = pthread_self();
		lock->expBackoff = rand()%10+1;
		lock->lockCount++;
	}else{	
		lock->lockCount++;
	}
	return 0;

	// while(1)
	// {
	// 	if(lock->tid == pthread_self()){
	// 		lock->lockCount++;
	// 		break;
	// 	}else if(lock->lockstatus == 0){	
	// 		//when lock is occupied, keep looping
	// 		if(tas(&lock->lockstatus) == 0)
	// 		{
	// 			lock->tid = pthread_self();
	// 			lock->expBackoff = rand()%10+1;
	// 			lock->lockCount++;
	// 			break;
	// 		}
	// 		else if(tas(&lock->lockstatus) != 0)
	// 		{
	// 			lock->sleep.tv_nsec = lock->expBackoff;
	// 			nanosleep(&lock->sleep, NULL);
	// 			if(lock->expBackoff >= 10000000) lock->expBackoff = rand()%10+1;
	// 			else if(lock->expBackoff < 10000000) lock->expBackoff *= lock->expBackoff;
	// 		}
	// 	}
	// }
	// return 0;
}

int my_mutex_trylock(my_mutex_t *lock)
{
	if(tas(&lock->lockstatus) == 0)
	{
		lock->lockCount++;
		lock->tid = pthread_self();
	}
}


//Queue Lock
int my_queuelock_init(my_queuelock_t *lock)
{
	lock->nowServing = 0;
	lock->nextTicket = 0;
	lock->lockCount = 0;
	lock->tid = 0;
}

int my_queuelock_destroy(my_queuelock_t *lock)
{
	//may do nothing or just deallocate it
	return 0;
}

int my_queuelock_unlock(my_queuelock_t *lock)
{
	if(lock->tid == pthread_self())
	{
		lock->lockCount--;
		if(lock->lockCount == 0)
		{
			lock->tid = 0;
			//once the lock is released we add 1 to nowServing
			__sync_fetch_and_add(&lock->nowServing, 1);
		}
	}
}

int my_queuelock_lock(my_queuelock_t *lock)
{
	if(lock->tid != pthread_self()){
		int myTicket = __sync_fetch_and_add(&lock->nextTicket, 1);
		while(lock->nowServing != myTicket) {}
		lock->tid = pthread_self();
		lock->lockCount++;
	}else{
		lock->lockCount++;
	}
	return 0;
}

int my_queuelock_trylock(my_queuelock_t *lock)
{
	return lock->nowServing;
}

