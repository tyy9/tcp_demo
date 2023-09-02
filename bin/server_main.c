#include"../src/server.h"
int main(int args, char *argv[])
{
    lcd_open();
    // 定义服务器自己的数据
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.59.170");
    server_addr.sin_port = htons(10000);

    // 定义客户端自己的数据
    bzero(&client_addr, sizeof(client_addr));
    int client_addr_size = sizeof(client_addr);

    // 获取套接字标识符
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        perror("创建套接字失败\n");
        return -1;
    }
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    ret = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret == -1)
    {
        perror("服务器绑定数据失败\n");
        return -1;
    }
    // 监听
    ret = listen(sockfd, 10); // 最多允许10个客户端同时连接同一个服务器
    if (ret == -1)
    {
        perror("监听失败了!\n");
        return -1;
    }
    printf("服务器正在尝试接受信息\n");
    client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_fd == -1)
    {
        perror("接受信息失败\n");
        return -1;
    }
    printf("客户端的信息--ip:%s,port:%hu\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    if (pthread_create(&send_thread, NULL, server_send_func, NULL) == -1)
    {
        perror("创建写线程失败\n");
        return -1;
    }
    if (pthread_create(&receive_thread, NULL, server_receive_func, NULL) == -1)
    {
        perror("创建读线程失败\n");
        return -1;
    }
    while (1)
    {
        sleep(1);
    }

    close(sockfd);
    close(client_fd);
    lcd_close();
    return 0;
}