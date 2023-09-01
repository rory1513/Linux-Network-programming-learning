#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include<arpa/inet.h>
#include <pthread.h>
#include<sys/socket.h>
#include<ctype.h>

#define SEVER_PORT 9527
void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char *argv[])
{
    int cfd = 0;
    int conter = 10;
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1)
    {
        sys_err("socket error");
    }
    struct sockaddr_in serv_addr; //服务器结构地址
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SEVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr("10.0.2.15");
    int ret = connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
    {
        sys_err("connect error");
    }
    char buf[1024];
    // 循环读取
    while (--conter)
    {   
       
        write(cfd, "hello\n", 6);
        
        ret = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);
        sleep(1);
        
        
    }
    close(cfd);
    return 0;
}
