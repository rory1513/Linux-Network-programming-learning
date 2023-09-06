//错误封装
#include "wrap.h"
void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int Socket(int domin, int type, int protocol)
{
    int n;
    n = socket(domin, type, protocol);
    if (n == -1)
    {
        sys_err("socket error");
    }

    return n;

}

int Listen(int fd, int backlog)
{
    int n = 0;
    n = listen(fd, backlog);
    if (n == -1)   
    {
        sys_err("listen error");
    }
    return n;
}