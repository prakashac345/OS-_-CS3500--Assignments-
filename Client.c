#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX 100

// structure for message queue
struct msg_buffer 
{
    long msg_type;
	char msg_text[100];
};

struct opcode
{
    long msg_type;
    char op[100];
};

int main()
{
    key_t key1,key2;
    int msgid1,msgid2;
    key1 = 1235;
    key2 = 1234;
    msgid1 = msgget(key1,0666 | IPC_CREAT);
    msgid2 = msgget(key2,0666 | IPC_CREAT);

    int tloop = 1;

    while(tloop)
    {
        struct msg_buffer message;
        message.msg_type = 1;

        printf("Write Data to Server : ");

        fgets(message.msg_text,MAX,stdin);

        msgsnd(msgid1, &message, sizeof(message), 0);

        if(strncmp(message.msg_text,"END",3) == 0)
        {
            tloop = 0;
            continue;
        }

        struct opcode opcoder;

        opcoder.msg_type = 2;
        msgrcv(msgid2, &opcoder, sizeof(opcoder), 2, 0);

        if(strncmp(opcoder.op,"END",3) == 0)
        {
            tloop = 0;
            continue;
        }

        printf("Output from Server / Return value of System Call is : %s \n" ,opcoder.op);

    }

    msgctl(msgid1, IPC_RMID, NULL);
    msgctl(msgid2, IPC_RMID, NULL);
}