/*
	Allows user to choose N workers, assigns them each a random time X, and forces the threads to wait for all other threads to be done before being released
	Created by: Matthew King
*/


#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

//global variables
sem_t semaphore;
pthread_mutex_t mutexLock;
int workers_done = 0, num_threads = 0, sleep_counter = 0;
int workers_working;
int worker_count = 0;


//function to be executed by all threads
void *workerTask(void *arg)
{	
	
	//semaphore waits
	sem_wait(&semaphore);
		
	//initialize variables
	int workerTime = rand() % 10 + 1;
	int seconds = workerTime;
	long PID = pthread_self();
	int workerId = worker_count;
	worker_count++;
	
	printf("Worker %d [ %ld ] will work for %d seconds\n", workerId, PID, seconds);
	
	//decrements after every loop, prints that the worker is still working
	for(int i = workerTime; i > 0; i--)
	{
		sleep(1);
		printf("Worker %d [ %ld ] is working ... %d \n", workerId, PID, (seconds - 1));
		seconds = seconds - 1;
	}

	
	//mutex locks, variables are updated, mutex unlocks	
	pthread_mutex_lock(&mutexLock);
	printf("Worker %d [ %ld ] has incremented counter by 1. The total number of workers done is now [ %d ]\n", workerId, PID, (workers_done) + 1);
	printf("Worker %d [ %ld ] is done with its job but has to wait for [ %d ] workers to finish their jobs...\n", workerId, PID, (workers_working) - 1);
	workers_done++;
	workers_working--;
	pthread_mutex_unlock(&mutexLock);
	
	//wait for the other threads to be done before incrementing the semaphore
	while(workers_working != 0)
	{
		/* wait for other threads... */
	}
	
	//semaphore posts once the worker is done waiitng
	sem_post(&semaphore);
	printf("Worker %d [ %ld ] increments the semaphore value by 1\n", workerId, PID);
}


//main method
int main()
{
	
	//used to randomize the random numbers
	srand(time(0));
	
	//takes in argument from command line
	printf("Enter number of threads: \n");
	scanf("%d", &num_threads);
	
	//initialize mutex lock, semaphore integer, and variable
	sem_init(&semaphore, 0, num_threads);
	pthread_t workerThreads[num_threads];
	
	//create worker thread based on command line input
	workers_working = num_threads;
	
	//create multiple "workers" (threads)
	for(int i = 0; i < num_threads; i++)
	{
		pthread_create(&workerThreads[i], NULL, workerTask, NULL);
	}
	
	//synchronizes all of the threads after they have finished their work
	for(int i = 0; i < num_threads; i++)
	{
		pthread_join(workerThreads[i], NULL);
	}
	
	
	sem_destroy(&semaphore);
	
	printf("All workers are done waiting! All workers released.\n");
	
	return 0;
	
}




