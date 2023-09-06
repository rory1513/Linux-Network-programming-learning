//错误封装
#include "wrap.h"
void perr_exit(const char *str)
{
    perr_exit(str);
    exit(1);
}

int Socket(int domain, int type, int protocol)
{
    int n;
    if ((n = socket(domain, type, protocol)) < 0)
    {
        perr_exit("socket error");
    }

    return n;
}

int Bind(int sockfd, const struct sockaddr *addr,
         socklen_t addrlen)
{
    int n;
    if ((n = bind(sockfd, addr, addrlen)) < 0)
    {
        perr_exit("bind error");
    }
    return n;
}

int Listen(int sockfd, int backlog)
{
    int n;
    if ((n = listen(sockfd, backlog)) < 0)
    {
        perr_exit("listen error");
    }
    return n;
}
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int n;
again:
    if ((n = accept(sockfd, addr, addrlen)) < 0)
    {
        if ((errno == ECONNABORTED) || (errno == EINTR))
        {
            goto again;
        }
        else
        {
            perr_exit("accept error");
        }
    }
    return n;
}
int Connect(int sockfd, const struct sockaddr *addr,
            socklen_t addrlen)
{
    int n;
    if ((n = connect(sockfd, addr, addrlen)) < 0)
    {
        perr_exit("connect error");
    }
    return n;
}
ssize_t Read(int fd, void *buf, size_t count)
{
    ssize_t n;
again:
    if ((n = read(fd, buf, count)) == -1)
    {
        if (errno == EINTR)
        {
            goto again;
        }
        else
        {
            return -1;
        }
    }
    return n;
}
ssize_t Write(int fd, const void *buf, size_t count)
{
    ssize_t n;
again:
    if ((n = write(fd, buf, count)) == -1)
    {
        if (errno == EINTR)
        {
            goto again;
        }
        else
        {
            return -1;
        }
    }
    return n;
}
int Close(int fd)
{
    int n;
    if ((n = close(fd)) == -1)
    {
        perror("close error");
    }

    return n;
}

// 读取N个字节
ssize_t Readn(int fd, void *vptr, size_t n)
{
    size_t nleft;  // unsigned int 剩余未读取的字节数
    ssize_t nread; // int 实际读取的字节数

    char *ptr;

    ptr = vptr;

    nleft = n; // 未读取字节数

    while (nleft > 0)
    {
        if ((nread = read(fd, ptr, nleft)) < 0)
        {
            // 处理系统中断错误
            if (errno == EINTR)
            {
                nread = 0;
            }
            else
            {
                return -1;
            }
        }
        else if (nread == 0)
        {
            break;
        }

        nleft -= nread;
        ptr += nread;
    }
    return n - nleft;
}

// 写N个字节
ssize_t writen(int fd, const void *vptr, size_t n)
{
    // 剩余字节数
    size_t nleft;
    // 已经写出字节数
    ssize_t nwritten;
    // 指向待写数据
    const char *ptr;

    ptr = vptr;
    nleft = n; // 起始，剩余n

    while (nleft > 0)
    {
        // 开始写入
        if ((nwritten = write(fd, ptr, nleft)) < 0)
        {
            if (nwritten < 0 && errno == EINTR)
            {
                nwritten = 0;
            }
            else
            {
                return -1;
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

static ssize_t my_read(int fd, char *ptr)
{
    static int read_cnt;
    static char *read_ptr;
    static char read_buf[100];

    if (read_cnt <= 0)
    {
    again:
        if ((read_cnt = (read(fd, read_buf, sizeof(read_buf)))) < 0)
        {
            if (errno == EINTR)
            {
                goto again;
            }
            return -1;
        }
        else if (read_cnt == 0)
        {
            return 0;
        }

        read_ptr = read_buf;
    }
    read_cnt--;
    *ptr = *read_ptr++;

    return 1;
}

// ssize_t Readline(int fd, void *vptr, size_t maxlen)
// {
//     ssize_t n, rc;

//     char c, *ptr;

//     ptr = vptr;

//     for (n = 1; n < maxlen; n++)
//     {
//         if ((rc = my_read(fd, &c)) == -1)
//         {
//             *ptr++ = c;
//             if (c == '\n')
//             {
//                 break;
//             }
//         }
//         else if (rc == 0)
//         {

//             *ptr = 0;
//             return n - 1;
//         }
//     }
//     *ptr = 0;
//     return n;
// }

// 读取一行
/*readline --- fgets*/
//传出参数 vptr
ssize_t Readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char c, *ptr;
    ptr = vptr;

    for (n = 1; n < maxlen; n++)
    {
        if ((rc = my_read(fd, &c)) == 1)
        { // ptr[] = hello\n
            *ptr++ = c;
            if (c == '\n')
                break;
        }
        else if (rc == 0)
        {
            *ptr = 0;
            return n - 1;
        }
        else
            return -1;
    }
    *ptr = 0;

    return n;
}
