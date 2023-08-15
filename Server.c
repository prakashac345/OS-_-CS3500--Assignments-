#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


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
    int flag1 = 0;
    int x;
    int filedescriptor = 0;
    key1 = 1235;
    key2 = 1234;
    msgid1 = msgget(key1,0666 | IPC_CREAT);
    msgid2 = msgget(key2,0666 | IPC_CREAT);

    int tloop = 1;
    while(tloop == 1)
    {
        struct msg_buffer messager;
        messager.msg_type = 1;
        msgrcv(msgid1, &messager, sizeof(messager), 1, 0);
        struct opcode opc;
        opc.msg_type = 2;

        if(strncmp(messager.msg_text,"END",3) == 0)
        {
            tloop = 0;
            continue;
        }
        if(tloop == 1)
        {
            if(strncmp(messager.msg_text,"ls",2) == 0)
            {
                x = system("ls -l");
                sprintf(opc.op,"%d",x);
                //printf("dd %s",opc.op);
            }

            else if(strncmp(messager.msg_text,"mkdir",5) == 0)
            {
                x = mkdir("prakashdir",0777);
                sprintf(opc.op,"%d",x);
            }

            else if(strncmp(messager.msg_text,"rmdir",5) == 0)
            {
                x = rmdir("prakashdir");
                sprintf(opc.op,"%d",x);
            }

            else if(strncmp(messager.msg_text,"rename",6) == 0)
            {
                if(flag1 == 0) 
                {
                    x = rename("cs20b061file","prakashfile");
                    sprintf(opc.op,"%d",x);
                    flag1 = 1;
                }
                else if(flag1 == 1) 
                {
                    x = rename("prakashfile","cs20b061file");
                    sprintf(opc.op,"%d",x);
                    flag1 = 0;
                }
            }

            else if(strncmp(messager.msg_text,"open",4) == 0)
            {
                x = open("cs20b061file",O_CREAT);
                filedescriptor = x;
                sprintf(opc.op,"%d",x);
            }

            else if(strncmp(messager.msg_text,"close",5) == 0)
            {
                x = close(filedescriptor);
                filedescriptor = x;
                sprintf(opc.op,"%d",x);
            }

            else if(strncmp(messager.msg_text,"exit",4) == 0)
            {
                //_exit(0);
                tloop = 0;
                opc.msg_type=2;
                strncpy(opc.op,"END",3);
            }

            else 
            {
                printf("No system call available \n");
                opc.msg_type=2;
                strncpy(opc.op,"END",3);
                msgsnd(msgid2, &opc, sizeof(opc), 0);
                tloop = 0;
                continue;
            }

        }

        opc.msg_type=2;
        msgsnd(msgid2, &opc, sizeof(opc), 0);

        printf("Data Received from Client is : %s \n", messager.msg_text);

        printf("Return value of the System Call is : %d \n",x);

    }
    msgctl(msgid1, IPC_RMID, NULL);
    msgctl(msgid2, IPC_RMID, NULL);
}