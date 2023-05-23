#include <iostream>
#include <ctime>
#include <pthread.h>

pthread_barrier_t barrier;

void* thread1(void* not_used)
{
    time_t now;
    char buf[27];
    time(&now);
    std::cout << "Potok 1, vremia starta " << ctime_r(&now, buf);
    std::cout << "Kalaganov Kirill" << std::endl;
    sleep(8);
    pthread_barrier_wait(&barrier);
    time(&now);
    std::cout << "barier v potoke 1, vrimia srabativania " << ctime_r(&now, buf);
    return NULL;
}

void* thread2(void* not_used)
{
    time_t now;
    char buf[27];
    time(&now);
    std::cout << "Potok 2, vremia starta " << ctime_r(&now, buf);
    std::cout << "I993" << std::endl;
    sleep(12);
    pthread_barrier_wait(&barrier);
    time(&now);
    std::cout << "barier v potoke 2, vrimia srabativania " << ctime_r(&now, buf);
    return NULL;
}

int main()
{
    time_t now;
    char buf[27];
    pthread_barrier_init(&barrier, NULL, 3);
    std::cout << "Start" << std::endl;
    
    pthread_t threadId1, threadId2;
    pthread_create(&threadId1, NULL, thread1, NULL);
    pthread_create(&threadId2, NULL, thread2, NULL);
    
    time(&now);
    std::cout << "Main(): oshidanie y bariera, vrimia " << ctime_r(&now, buf);
    pthread_barrier_wait(&barrier);
    time(&now);
    std::cout << "barier v main(), vremia srabativania " << ctime_r(&now, buf);
    
    pthread_join(threadId1, NULL);
    pthread_join(threadId2, NULL);
    
    sleep(5);
    
    pthread_barrier_destroy(&barrier);
    
    return 0;
}
