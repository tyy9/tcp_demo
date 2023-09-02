#include"../src/client.h"
int main(int args, char *argv[])
{
    // 定义客户端自己的数据
    bzero(&client_addr, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client_addr.sin_port = htons(10001);

    // 定义服务器的数据

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.59.170");
    server_addr.sin_port = htons(10000);

    // 获取套接字标识符
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        perror("创建套接字失败\n");
        return -1;
    }
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    ret = bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr));
    if (ret == -1)
    {
        perror("客户端绑定数据失败\n");
        return -1;
    }

    // 链接服务器
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("链接服务端失败\n");
        return -1;
    }

    if (pthread_create(&send_thread, NULL, client_send_func, NULL) == -1)
    {
        perror("创建写线程失败\n");
        return -1;
    }
    if (pthread_create(&receive_thread, NULL, client_receive_func, NULL) == -1)
    {
        perror("创建读线程失败\n");
        return -1;
    }
    while (1)
    {
        sleep(1);
    }
    close(sockfd);
    return 0;
}