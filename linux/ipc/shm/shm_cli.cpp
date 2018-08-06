#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int value = 0;
    key_t key = ftok(".", 0xFF);
    int sem_id = semget(key, 1, IPC_CREAT|0644);
    if(-1 == sem_id)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    if(-1 == (semctl(sem_id, 0, SETVAL, value)))
    {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    int shm_id = shmget(key, 1024, IPC_CREAT|0644);
    if(-1 == shm_id)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    //attach the shm_id to this process
    char *shm_ptr = (char*)shmat(shm_id, NULL, 0);
    if(NULL == shm_ptr)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    struct sembuf sem_b;
    sem_b.sem_num = 0;      //first sem(index=0)
    sem_b.sem_flg = SEM_UNDO;
    sem_b.sem_op = 1;         //Increase 1,make sem=1
    
    printf("\nMessage sender is running:\n");
    while(1)
    {
        if(0 == (value = semctl(sem_id, 0, GETVAL)))
        {
            printf("\tInput the message to send:  ");
            scanf("%s", shm_ptr);

            if(-1 == semop(sem_id, &sem_b, 1))
            {
                perror("semop");
                exit(EXIT_FAILURE);
            }
        }

        if(0 == (strcmp(shm_ptr ,"exit")))
        {
            printf("\nExit sender process now!\n");
            break;
        }
    }

    shmdt(shm_ptr);

    return 0;
}
