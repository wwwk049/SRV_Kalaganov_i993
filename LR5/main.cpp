#include <iostream>
#include <pthread.h>
#include <cstdint>
#include <cerrno>
#include <ctime>
#include <sys/neutrino.h>
#include <unistd.h>

#define SEC_NSEC 1000000000LL // 1 секунда = 1 миллиард наносекунд

void* long_thread(void* notused)
{
    std::cout << "Etot potok vipolnaetsa bolee 10 sekynd" << std::endl;
    sleep(20);
    return NULL;
}

int main()
{
    uint64_t timeout;
    struct sigevent event;
    int rval;
    pthread_t thread_id;
    std::cout << "Kalaganov Kirill I993" << std::endl;
    std::cout << "Prog timer" << std::endl;
    event.sigev_notify = SIGEV_UNBLOCK;
    pthread_create(&thread_id, NULL, long_thread, NULL);
    timeout = 10LL * SEC_NSEC;
    TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_JOIN, &event, &timeout, NULL);
    rval = pthread_join(thread_id, NULL);
    if (rval == ETIMEDOUT)
    {
        std::cout << "Istekli 10 sekynd, potok " << thread_id << " vipolniaetsia!" << std::endl;
    }
    sleep(10);
    TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_JOIN, &event, &timeout, NULL);
    rval = pthread_join(thread_id, NULL);
    if (rval == ETIMEDOUT)
    {
        std::cout << "Potok " << thread_id << " >25 sek!" << std::endl;
    }
    else
    {
        std::cout << "Potok " << thread_id << " zavershon kak nado" << std::endl;
    }
    return 1;
}
