#ifndef _server_H_
#define _server_H_
#include "my_head.h"
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include"bmp_plus.h"
int sockfd, client_fd;
int ret;
struct sockaddr_in client_addr;
struct sockaddr_in server_addr;
pthread_t send_thread, receive_thread;
FILE *fp_read, *fp_write;
int download_flag; // 图片下载标志
char file_name[256];
char buffer[4096];
static pthread_mutex_t r_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t r_mutex_cond = PTHREAD_COND_INITIALIZER;

void *server_send_file_func(void *args);
void *server_receive_file_func(void *args);
void *server_receive_func(void *args);
void *server_send_func(void *args);
#endif