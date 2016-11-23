#include "main.h"

pthread_mutex_t count_mutex;
pthread_spinlock_t count_spin;	
my_spinlock_t count_myspin;
my_mutex_t count_mymutex;
my_queuelock_t count_myqueue;

void *fnC()
{
	int i;
    for(i=0;i<1000000;i++)
    {   
    	c++;
    }   
}

void *pthreadMutexTest()
{
    int i;
	int j;
	int k;
	int localCount = 0;
	
    for(i=0;i<numItterations;i++)
    {
		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
		{
			localCount++;
		}
	
		pthread_mutex_lock(&count_mutex);
		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
		pthread_mutex_unlock(&count_mutex);    	
    }   
}

void *pthreadSpinTest()
{
    int i;
	int j;
	int k;
	int localCount = 0;
	
    for(i=0;i<numItterations;i++)
    {
		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
		{
			localCount++;
		}
	
		pthread_spin_lock(&count_spin);
		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
		pthread_spin_unlock(&count_spin);    	
    }
}

void *mySpinTASTest()
{
	int i;
	int j;
	int k;
	int localCount = 0;
	
    for(i=0;i<numItterations;i++)
    {
		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
		{
			localCount++;
		}
		//my_spinlock_lockTAS(&count_myspin); recursive
		my_spinlock_lockTAS(&count_myspin);
		//only the thread that gets the lock
		//gets to run this critical section to increment c
		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
		//unlock the lock
		my_spinlock_unlock(&count_myspin);
		//my_spinlock_unlock(&count_myspin); recursive  	
    }   
}

void *mySpinTTASTest()
{
	int i;
	int j;
	int k;
	int localCount = 0;
	
    for(i=0;i<numItterations;i++)
    {
		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
		{
			localCount++;
		}
	
		my_spinlock_lockTTAS(&count_myspin);
		//only the thread that gets the lock
		//gets to run this critical section to increment c
		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
		//unlock the lock
		my_spinlock_unlock(&count_myspin);    	
    }   
}

void *myMutexTTASTest()
{
	int i;
	int j;
	int k;
	int localCount = 0;
	
    for(i=0;i<numItterations;i++)
    {
		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
		{
			localCount++;
		}
		
		my_mutex_lock(&count_mymutex);
		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
		my_mutex_unlock(&count_mymutex);    	
    }
}

void *myQueueTest()
{
	int i;
	int j;
	int k;
	int localCount = 0;
	
    for(i=0;i<numItterations;i++)
    {
		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
		{
			localCount++;
		}
	
		my_queuelock_lock(&count_myqueue);
		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
		my_queuelock_unlock(&count_myqueue);    	
    }   
}

int runTest(int testID)
{
	/*You must create all data structures and threads for you experiments*/
	if (testID == 0 || testID == 1) /*Pthread Mutex*/
	{
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result; //64 bit integer

		pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++)
		{
			if( rt=(pthread_create(threads+i, NULL, &pthreadMutexTest, NULL)) )
			{
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++) //Wait for all threads to finish
		{
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("Threaded Run Pthread (Mutex) Total Count: %llu\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread Mutex time(ms): %lld\n",result/numItterations /*1000000*/);
	}

	if(testID == 0 || testID == 2) /*Pthread Spinlock*/
	{
		/*Pthread Spinlock goes here*/
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result; //64 bit integer
		pthread_spin_init(&count_spin, 0);

		pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++)
		{
			if( rt=(pthread_create(threads+i, NULL, &pthreadSpinTest, NULL)) )
			{
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++) //Wait for all threads to finish
		{
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("Threaded Run Pthread (Spinlock) Total Count: %llu\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread Spinlock time(ms): %lld\n",result/numItterations /*1000000*/);
	}

	// /*....you must implement the other tests....*/
	if(testID == 0 || testID == 3) /*MySpinlockTAS*/
	{
		/*mySpinlock TAS goes here*/
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result; //64 bit integer
		my_spinlock_init(&count_myspin); //setting lockstatus to 0(free)

		pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++)
		{
			if( rt=(pthread_create(threads+i, NULL, &mySpinTASTest, NULL)) )
			{
				printf("Thread creation failed: %d\n", rt);
				return -1;
			}
		}
		
		for(i=0;i<numThreads;i++) //Wait for all threads to finish
		{
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("Threaded Run mySpinTAS Total Count: %llu\n", c);
		result=timespecDiff(&stop,&start);
		printf("mySpinTAS time(ms): %lld\n",result/numItterations /*1000000*/);
	}

	if(testID == 0 || testID == 4) /*mySpinLockTTAS*/
	{
		/* mySpinLockTTAS goes here*/
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result; //64 bit integer
		my_spinlock_init(&count_myspin); //setting lockstatus to 0(free)

		pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++)
		{
			if( rt=(pthread_create(threads+i, NULL, &mySpinTTASTest, NULL)) )
			{
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++) //Wait for all threads to finish
		{
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("Threaded Run mySpinTTAS Total Count: %llu\n", c);
		result=timespecDiff(&stop,&start);
		printf("mySpinTTAS time(ms): %lld\n",result/numItterations /*1000000*/);
	}

	if(testID == 0 || testID == 5) /*myMutexTAS*/
	{
		/* myMutexTAS goes here*/
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result; //64 bit integer
		my_mutex_init(&count_mymutex);

		pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++)
		{
			if( rt=(pthread_create(threads+i, NULL, &myMutexTTASTest, NULL)) )
			{
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++) //Wait for all threads to finish
		{
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("Threaded Run myMutexTAS Total Count: %llu\n", c);
		result=timespecDiff(&stop,&start);
		printf("myMutexTAS time(ms): %lld\n",result/numItterations /*1000000*/);
	}

	if(testID == 0 || testID == 6) /*myQueueLock*/
	{
		/* myQueueLock goes here*/
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result; //64 bit integer
		my_queuelock_init(&count_myqueue);

		pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++)
		{
			if( rt=(pthread_create(threads+i, NULL, &myQueueTest, NULL)) )
			{
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++) //Wait for all threads to finish
		{
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("Threaded Run myQueueLock Total Count: %llu\n", c);
		result=timespecDiff(&stop,&start);
		printf("myQueueLock time(ms): %lld\n",result/numItterations /*1000000*/);
	}
	return 0;
}

int testAndSetExample()
{
	volatile long test = 0; //Test is set to 0
	printf("Test before atomic OP:%ld\n",test);
	tas(&test);
	printf("Test after atomic OP:%ld\n",test);
}

int processInput(int argc, char *argv[])
{
	/*testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock*/
	/*You must write how to parse input from the command line here, your software should default to the values given below if no input is given*/
	numThreads=4;
	numItterations=1000000;
	testID=0;
	workOutsideCS=0;
	workInsideCS=1;
	
	//if there is only 1 argument and with no input given then return 0 with default values
	if(argc==1)
	{
		printf("numThreads: %d\n", numThreads);
		printf("numItterations: %d\n", numItterations);
		printf("testID: %d\n", testID);
		printf("workOutsideCS: %d\n", workOutsideCS);
		printf("workInsideCS: %d\n", workInsideCS);
		printf("\n");
		return 0;
	}
	//if inputs are given then replace the values
	for(int i=0;i<argc;++i)
	{
		if(strcmp(argv[i], "-t") == 0) numThreads = atoi(argv[i+1]);
		else if(strcmp(argv[i], "-i") == 0) numItterations = atoi(argv[i+1]);
		else if(strcmp(argv[i],  "-o") == 0) workOutsideCS = atoi(argv[i+1]);
		else if(strcmp(argv[i], "-c") == 0) workInsideCS = atoi(argv[i+1]);
		else if(strcmp(argv[i], "-d") == 0) testID = atoi(argv[i+1]);
	}	

	printf("numThreads: %d\n", numThreads);
	printf("numItterations: %d\n", numItterations);
	printf("testID: %d\n", testID);
	printf("workOutsideCS: %d\n", workOutsideCS);
	printf("workInsideCS: %d\n", workInsideCS);
	printf("\n");

	return 0;
}


int main(int argc, char *argv[])
{
	printf("Usage of: %s -t #threads -i #Itterations -o #OperationsOutsideCS -c #OperationsInsideCS -d testid\n", argv[0]);
	printf("testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock, \n");	
	
	//testAndSetExample(); //Uncomment this line to see how to use TAS
	
	processInput(argc,argv);
	runTest(testID);
	return 0;
}
