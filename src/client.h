#ifndef _client_H_
#define _client_H_
#include "my_head.h"
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
int sockfd;
int ret;
struct sockaddr_in client_addr;
struct sockaddr_in server_addr;
pthread_t send_thread, receive_thread, send_file_thread,receive_file_thread;
FILE *fp_read, *fp_write;
char file_name[256];
char buffer[4096];
static pthread_mutex_t r_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t r_mutex_cond = PTHREAD_COND_INITIALIZER;

void *client_send_file_func(void *args);
void *client_receive_file_func(void *args);
void *client_receive_func(void *args);
void *client_send_func(void *args);
#endif