/*
多进程并发服务器
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "wrap.h"

#define SRV_PORT 9999
#define SRV_PROT 9999

int main(int argc, char *argv[])
{
    int lfd,cfd;
    pid_t pid;
    int ret ,i;

    char buf [BUFSIZ];

    lfd = Socket(AF_INET, SOCK_STREAM, 0);
    printf("%d",lfd);
    struct sockaddr_in servaddr,clt_addr;
    socklen_t clt_addr_len;
    memset(&servaddr, 0, sizeof(servaddr)); // 将地址清零
    //bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SRV_PROT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(lfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    Listen(lfd, 128);
    clt_addr_len = sizeof(clt_addr);
    while (1)
    {
        
        cfd = Accept(lfd, (struct sockaddr *)&clt_addr,&clt_addr_len);

        pid = fork();
        if(pid < 0)
        {
            perr_exit("fork error");
            exit(1);
        }else if (pid == 0)
        {
            close(lfd);
            break;
        }else
        {
            close(cfd);
            continue;
        }

        if (pid == 0)
        {
            for(;;)
            {
                ret == Read(cfd, buf, sizeof(buf));
                if (ret == 0)
                {
                    close(cfd);
                    exit(1);
                }
                
                for ( i = 0; i < ret; i++)
                {
                    buf[i] = toupper(buf[i]);
                }
                write(cfd, buf, ret);
                write(STDOUT_FILENO, buf, ret);
                    
            }
        }
         

    }
    return 0;
    

}