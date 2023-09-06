#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include<arpa/inet.h>
#include <pthread.h>
#include<sys/socket.h>
#include<ctype.h>
#include<sys/types.h>
#ifndef __WRAP_H
#define __WRAP_H

void sys_err(const char *str);
int Socket(int domin, int type, int protocol);
int Listen(int fd, int backlog);

#endif