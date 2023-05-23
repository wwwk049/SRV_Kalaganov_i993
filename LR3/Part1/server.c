#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>

int main(int argc, char *argv)
{
    int rcvid;
    int chid;
    char message[40];
    char r_message[40];
    char valid[] = "Kalaganov Kirill I993";
    printf("Prog server \n");
    printf("Server start working/n");
    chid = ChannelCreate(0);
    printf("Chanel id : %d \n", chid);
    printf("Pid: %d \n", getpid());

    while (1)
    {
        rcvid = MsgReceive(chid, message, sizeof(message), NULL);
        printf("Polychili soobshenie, rcid %X \n", rcvid);
        if (rcvid > 0)
        {
            #ifdef DEBUG
            printf("Soobshenie takoe : \"%s\"%d.\n", message,strlen(message));
            printf("Test takoi \"%s\"%d.\n",valid,strlen(valid));
            #endif
            if (!strcmp(message, valid))
                sprintf(r_message, "v Test passed");
            else
                sprintf(r_message, "x Test denied");

            MsgReply(rcvid, EOK, r_message, sizeof(r_message));
            printf("\"%s\". \n", r_message);
        }
    }
    sleep(5);
    return (1);
}
