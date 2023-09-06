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
    int lfd = 0 ,cfd = 0;
    int ret;
    char buf [BUFSIZ];
    struct sockaddr_in serv_addr,clit_addr;
    socklen_t clit_addr_len;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SEVER_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    // 检查参数
    if (lfd == -1 )
    {
        sys_err("socket error ");
    }
    int b = bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    if (b == -1)
    {
        sys_err("bind error "); 
    }

    int l = listen(lfd, 128);
    
    if (l == -1)
    {
        sys_err("listen error ");
    }
    clit_addr_len = sizeof(clit_addr);
    cfd = accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);
    printf("client ip is %s,port is %d\n",inet_ntoa(clit_addr.sin_addr),ntohs(clit_addr.sin_port));//打印
    if (cfd == -1)
    {
        sys_err("accept error ");
    }
    
    


    while (1)
    {
        ret = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);
        for (int i = 0; i < ret; i++)
        {
            buf[i] = toupper(buf[i]);
            
        }
        write(cfd, buf, ret);
    }
    close(lfd);
    close(cfd); 
    return 0;
}
