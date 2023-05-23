#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/neutrino.h>

int main(int argc, char* argv[])
{
    int rcvid;
    int chid;
    char message[30];
    std::cout << "Prog server with Kalaganov Kirill I993" << std::endl;
    std::cout << "Server start working" << std::endl;
    chid = ChannelCreate(0);
    std::cout << "Chanel id: " << chid << std::endl;
    std::cout << "Pid: " << getpid() << std::endl;

    while (1)
    {
        rcvid = MsgReceive(chid, message, sizeof(message), NULL);
        std::cout << "Polychili soobshenie, rcid " << rcvid << std::endl;
        std::cout << "Soobshenie takoe: \"" << message << "\"" << std::endl;
        MsgReply(rcvid, 0, message, sizeof(message));
        std::cout << "\"" << message << "\"" << std::endl;
    }
    sleep(5);
    return 1;
}
