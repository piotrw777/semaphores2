#include <stdio.h>
#include <stdlib.h>
#include <time.h>  //sleep
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MEM_SIZE 64

static const char name[] = "sem22";



static key_t key;
static int shmid;
static int *shared_mem;

int main()
{
    printf("Hello from prog1!\n");
    //create shared memory
     key = ftok("shmfile", 65);
    int shmid = shmget(key, MEM_SIZE, 0644 | IPC_CREAT);
    if (shmid < 0)
    {
        perror("shmget error: ");
        exit(1);
    }
    if ((shared_mem = (int *) shmat(shmid, NULL, 0)) == (void *) -1)
    {
        perror("shmat error: ");
        exit(2);
    }
    printf("Shared memory initialized succesfully!!!\n");

    //create semaphore
    sem_unlink(name);
    sem_t *sem = sem_open(name, O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 1);
    if (sem == SEM_FAILED)
    {
        perror("sem_open error");
        exit(1);
    }
    for (int k = 0; k < 5; k++)
    {
        shared_mem[k] = 11;
    }
    for (int k = 0; k < 20; k++)
        {
        int sem_value;
	    sem_wait(sem);
        sem_getvalue(sem, &sem_value);
        printf("After wait: The value of the semaphore is: %d\n", sem_value);
        for (int i = 0; i < 5; i++)
        {
            shared_mem[i]++;
            sleep(1);
        }
	   fflush(stdout);
	   sem_post(sem);
       sem_getvalue(sem, &sem_value);
       printf("After sem_post: The value of the semaphore is: %d\n", sem_value);
        }
    sem_destroy(sem);
    shmdt(shared_mem);

}
