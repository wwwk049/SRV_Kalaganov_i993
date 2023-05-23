#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio> 
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>

#define MEM_NAME "ourmemory"

int threads(void);
void *thread_1(void *);
void *thread_2(void *);
int process_1(void);
int process_2(void);

typedef struct
{
    sem_t sem;
    int value;
} sh_mem_t;

pthread_mutex_t mut;
sh_mem_t *sm;

int main()
{
    int md;
    int childpid;
    std::cout << "Kalaganov Kirill I993" << std::endl;
    shm_unlink(MEM_NAME);
    md = shm_open(MEM_NAME, O_RDWR | O_CREAT, 0777);

    if (md == -1)
    {
        std::perror("shm_open()");
        return -1;
    }
    if (ftruncate(md, sizeof(*sm)) == -1)
    {
        std::perror("ftruncate()");
        return -1;
    }
    sm = (sh_mem_t *)mmap(NULL, sizeof(*sm), PROT_READ | PROT_WRITE, MAP_SHARED, md, 0);
    if (sm == MAP_FAILED)
    {
        std::perror("mmap()");
        return -1;
    }
    if (sem_init(&(sm->sem), 1, 1) == -1)
    {
        std::perror("sem_init()");
        exit(EXIT_FAILURE);
    }

    sm->value = 0;

    childpid = fork();
    if (childpid == -1)
    {
        std::perror("fork()");
    }
    if (childpid)
    {
        if (process_1())
        {
            std::perror("process_1()");
            exit(EXIT_FAILURE);
        }
        wait(NULL);
    }
    else
    {
        if (process_2())
        {
            std::perror("process2()");
            exit(EXIT_FAILURE);
        }
        if (threads())
        {
            std::perror("threads()");
            exit(EXIT_FAILURE);
        }
    }
    close(md);
    shm_unlink(MEM_NAME);
    pthread_mutex_destroy(&mut);
    return EXIT_SUCCESS;
}

int process_1(void)
{
    sem_wait(&(sm->sem));
    std::cout << "First process." << std::endl;
    std::cout << "(1) Previous value: " << sm->value << std::endl;

    sm->value = 10;
    sm->value += 2;
    sm->value *= 4;
    sm->value -= 15;
    std::cout << "(1) Result: " << sm->value << std::endl;

    sem_post(&(sm->sem));
    return 0;
}

int process_2(void)
{
    sem_wait(&(sm->sem));
    std::cout << "Second process." << std::endl;
    std::cout << "(2) Previous value: " << sm->value << std::endl;

    sm->value = 7;
    sm->value += 5;
    sm->value *= 2;
    sm->value -= 1;
    std::cout << "(2) Result: " << sm->value << std::endl;

    sem_post(&(sm->sem));
    return 0;
}

int threads(void)
{
    pthread_t thid_1, thid_2;
    if (pthread_mutex_init(&mut, NULL) == -1)
    {
        std::perror("pthread_mutex_init()");
        return -1;
    }

    if (pthread_create(&thid_1, NULL, thread_1, NULL) == -1 || pthread_create(&thid_2, NULL, thread_2, NULL) == -1)
    {
        std::perror("pthread_create()");
        return -1;
    }
    pthread_join(thid_1, NULL);
    pthread_join(thid_2, NULL);
    return 0;
}

void *thread_1(void *arg)
{
    pthread_mutex_lock(&mut);
    std::cout << "First lock" << std::endl;
    sleep(2);
    std::cout << "First unlock" << std::endl;
    pthread_mutex_unlock(&mut);
    return NULL;
}

void *thread_2(void *arg)
{
    pthread_mutex_lock(&mut);
    std::cout << "Second lock" << std::endl;
    sleep(2);
    std::cout << "Second unlock" << std::endl;
    pthread_mutex_unlock(&mut);
    return NULL;
}
