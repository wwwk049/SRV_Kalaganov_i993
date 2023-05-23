#include <iostream>
#include <pthread.h>
#include <cstring>
#include <sys/neutrino.h>

int main()
{
    char smsg[30] = "Kalaganov Kirill I993";
    char rmsg[200];
    int coid;
    long serv_pid;
    std::cout << "Prog client, Vvedite PID servera: ";
    std::cin >> serv_pid;
    coid = ConnectAttach(0, serv_pid, 1, 0, 0);
    std::cout << "Connect res " << coid << ", vvedite soobshenie: " << std::endl;
    std::cout << "Vveli " << smsg << std::endl;
    if (MsgSend(coid, smsg, strlen(smsg) + 1, rmsg, sizeof(rmsg)) == -1)
    {
        std::cout << "Error MsgSend" << std::endl;
    }
    std::cout << "Otvet ot servera" << std::endl;
    std::cout << rmsg << std::endl;
    ConnectDetach(coid);
    return 1;
}
