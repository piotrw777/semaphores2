#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MEM_SIZE 64

static const char name[] = "sem22";

sem_t *sem;

void * counter(void *arg)
{
    for (int i = 0; i < 1e3; i++)
    {
        sem_wait(sem);
        for (int j = 0; j < 5; j++)
        {

            printf("Lp: %d Hello from thread!\n", i);
            sleep(1);
        }
        sem_post(sem);
        sleep(1);
    }
}

pthread_t counting_thread;


int main()
{
    //create shared_memory
    printf("Hello from prog2!\n");

    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, MEM_SIZE, 0644 | IPC_CREAT);
    if (shmid < 0)
    {
        perror("shmget error: ");
        exit(1);
    }
    int *shared_mem;
    if ((shared_mem = (int *) shmat(shmid, NULL, 0)) == (void *) -1)
    {
        perror("shmat error: ");
        exit(2);
    }

    //create semaphore

    //sem = sem_open(name, O_CREAT | O_EXCL, 0666, 1);
    //sem_t *sem = sem_open(name, 0, S_IRUSR|S_IWUSR, 0);
    sem = sem_open(name, 0);
    if (sem == SEM_FAILED)
    {
        perror("sem_open error: ");
        exit(1);
    }
    sleep(1);
    pthread_create(&counting_thread, NULL, counter, NULL);

    for (int k = 0; k < 50; k++)
    {
    	int sem_val;
    	sem_wait(sem);
        for (int i = 0; i < 5; i++)
        {
            printf("%02d ", shared_mem[i]);
        }
    	sem_post(sem);
    	sem_getvalue(sem, &sem_val);
    	//sem_post(sem);
            printf("\n");
    	    printf("The value of the semaphore: %d\n", sem_val);
    }

    //close the semaphore
	if ( -1 == sem_close(sem) )
    {
        err(EXIT_FAILURE, "sem close");
    }
    if ( -1 == sem_unlink(name) )
    {
        err(EXIT_FAILURE, "sem_unlink");
    }
    //sem_destroy(sem);
    //close shared memory file
    shmdt(shared_mem);
    shmctl(shmid, IPC_RMID, NULL);
}
