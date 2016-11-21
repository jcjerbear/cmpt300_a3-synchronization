#include "main.h"

void *fnC()
{
	int i;
    for(i=0;i<1000000;i++)
    {   
    	c++;
    }   
}

pthread_mutex_t count_mutex;

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
	
		pthread_spin_lock(&count_mutex);
		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
		pthread_spin_unlock(&count_mutex);    	
    }   
}

void *mySpinTASTest()
{

}

void *mySpinTTASTest()
{

}

void *myMutexTASTest()
{

}

void *myQueueTest()
{

}

int runTest(int testID)
{
	/*You must create all data structures and threads for you experiments*/
	if (testID == 0 || testID == 1 ) /*Pthread Mutex*/
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

		printf("Threaded Run Pthread (Mutex) Total Count: %d\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread Mutex time(ms): %llu\n",result/1000000);
	}

	if(testID == 0 || testID == 2) /*Pthread Spinlock*/
	{
		/*Pthread Spinlock goes here*/
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

		printf("Threaded Run Pthread (Spinlock) Total Count: %d\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread Spinlock time(ms): %llu\n",result/1000000);
	}

	/*....you must implement the other tests....*/
	if(testID == 0 || testID == 3) /*MySpinlockTAS*/
	{
		/*mySpinlock TAS goes here*/
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

		printf("Threaded Run mySpinTAS Total Count: %d\n", c);
		result=timespecDiff(&stop,&start);
		printf("mySpinTAS time(ms): %llu\n",result/1000000);
	}

	if(testID == 0 || testID == 4) /*mySpinLockTTAS*/
	{
		/* mySpinLockTTAS goes here*/
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

		printf("Threaded Run mySpinTTAS Total Count: %d\n", c);
		result=timespecDiff(&stop,&start);
		printf("mySpinTTAS time(ms): %llu\n",result/1000000);
	}

	if(testID == 0 || testID == 5) /*myMutexTAS*/
	{
		/* myMutexTAS goes here*/
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
			if( rt=(pthread_create(threads+i, NULL, &myMutexTASTest, NULL)) )
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

		printf("Threaded Run myMutexTAS Total Count: %d\n", c);
		result=timespecDiff(&stop,&start);
		printf("myMutexTAS time(ms): %llu\n",result/1000000);
	}

	if(testID == 0 || testID == 6) /*myQueueLock*/
	{
		/* myQueueLock goes here*/
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

		printf("Threaded Run myQueueLock Total Count: %d\n", c);
		result=timespecDiff(&stop,&start);
		printf("myQueueLock time(ms): %llu\n",result/1000000);

	}
	return 0;
}

int testAndSetExample()
{
	volatile long test = 0; //Test is set to 0
	printf("Test before atomic OP:%d\n",test);
	tas(&test);
	printf("Test after atomic OP:%d\n",test);
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
	if(argc == 1 && argv[0] == "./mylocks") return 0; 
	//if inputs are given then replace the values
	for(int i=0;i<argc;i++)
	{
		if(argv[i] == "-t") numThreads = argv[i+1];
		else if(argv[i] == "-i") numItterations = argv[i+1];
		else if(argv[i] == "-o") workOutsideCS = argv[i+1];
		else if(argv[i] == "-c") workInsideCS = argv[i+1];
		else if(argv[i] == "-d") testID = argv[i+1];
	}	
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
