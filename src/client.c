#include"client.h"
void *client_send_file_func(void *args)
{
    fp_read = fopen(file_name, "rb");
    while (1)
    {
        if (fp_read == NULL)
        {
            printf("read--打开%s失败\n", file_name);
            exit(1);
        }
        bzero(buffer, sizeof(buffer));
        int len = 0;
        while (1)
        {
            len = fread(buffer, 1, sizeof(buffer), fp_read);
            printf("len=%d\n", len);
            if (len > 0)
            {
                send(sockfd, buffer, len, 0);
            }
            else
            {
                sleep(3);
                fclose(fp_read);
                send(sockfd, "up_exit", 7, 0);
                printf("文件上传成功\n");
                pthread_cond_signal(&r_mutex_cond);
                pthread_exit(NULL);
            }
            usleep(10* 1000);
        }
        fclose(fp_read);
    }
}
void *client_receive_file_func(void *args)
{
    printf("创建文件传输线程\n");
    // fp_write=fopen(file_name,"wb");//wb不支持bmp的字节写入，a+可以支持所有文件格式的写入
    int n=0;
    while (1)
    {   
       fp_write=fopen(file_name,"a+");
        bzero(buffer,sizeof(buffer)); 
        n = recv(sockfd, buffer, 4096, 0);
        printf("n=%d\n",n);
        if (n < 0)
        {
            perror("接受客户端信息失败\n");
            exit(0);
        }
        else if (n == 0)
        {
            printf("客户端断开链接\n");
        }
        if(strncmp(buffer,"up_exit",7)==0){
            fclose(fp_write);
            printf("文件下载成功\n");
            pthread_cond_signal(&r_mutex_cond);
            pthread_exit(NULL);
        }
        fwrite(buffer, 1, n, fp_write);
        fseek(fp_write,1,SEEK_CUR);
        fclose(fp_write);
    }
}
void *client_receive_func(void *args)
{
    char receive_msg[1024];
    while (1)
    {
        memset(receive_msg, 0, sizeof(receive_msg));
        ret = recv(sockfd, receive_msg, sizeof(receive_msg), 0);
        if (ret < 0)
        {
            perror("接受服务器信息失败\n");
            exit(0);
        }
        else if (ret == 0)
        {
            printf("服务器断开链接\n");
            exit(0);
        }
         // 判断接受的是何种数据
        if (strstr(receive_msg, "send_") != NULL)
        {
              bzero(file_name,sizeof(file_name));
            strtok(receive_msg, "_");
            char *p=strtok(NULL, "_");
            int i=0;
            while(*p!='\0'){
                file_name[i++]=*p;
                p++;
            }
            printf("file_name:%s\n", file_name);
            // 开启文件传输线程
            if (pthread_create(&receive_file_thread, NULL, client_receive_file_func, NULL) == -1)
            {
                perror("创建文件传输线程失败\n");
                exit(1);
            }
            // 阻塞普通读线程
            printf("阻塞普通读线程\n");
            pthread_cond_wait(&r_mutex_cond, &r_mutex);
            usleep(100*1000);//让下载线程先结束
            printf("唤醒普通读线程\n");
        }
        //上传文件
        if (strstr(receive_msg, "get_") != NULL)
        {
            bzero(file_name,sizeof(file_name));
            char buffer[4096];
            strtok(receive_msg, "_");
            char *p=strtok(NULL, "_");
            int i=0;
            while(*p!='\0'){
                file_name[i++]=*p;
                p++;
            }
            printf("file_name:%s\n", file_name);
            //先往服务器发送上传命令
            char cmd[256];
            bzero(cmd,sizeof(cmd));
            sprintf(cmd,"send_%s",file_name);
            send(sockfd, cmd, strlen(cmd), 0);
            sleep(1);
            // 创建文件上传线程
            if (pthread_create(&send_file_thread, NULL, client_send_file_func, NULL) == -1)
            {
                perror("创建文件上传线程失败\n");
                exit(1);
            }
            // 阻塞普通写线程
            printf("阻塞普通读线程\n");
            pthread_cond_wait(&r_mutex_cond, &r_mutex);
            usleep(100*1000);//让上传线程先结束
            printf("唤醒普通读线程\n");
        }
        printf("客户端---接受到的信息为:%s\n", receive_msg);
    }
}
void *client_send_func(void *args)
{
    char send_msg[1024];
    while (1)
    {
        memset(send_msg, 0, sizeof(send_msg));
        printf("客户端---请输入你所要发送的消息:");
        scanf("%s", send_msg);
        if (strncmp(send_msg, "exit", 4) == 0)
        {
            printf("退出\n");
            exit(0);
        }
        send(sockfd, send_msg, strlen(send_msg), 0);
        // 先发送传送命令，再传送图片数据
        //sleep(1);
        //上传文件
        if (strstr(send_msg, "send_") != NULL)
        {
          bzero(file_name,sizeof(file_name));
            char buffer[4096];
            strtok(send_msg, "_");
            char *p=strtok(NULL, "_");
            int i=0;
            while(*p!='\0'){
                file_name[i++]=*p;
                p++;
            }
            printf("file_name:%s\n", file_name);
            // 创建文件上传线程
            if (pthread_create(&send_file_thread, NULL, client_send_file_func, NULL) == -1)
            {
                perror("创建文件上传线程失败\n");
                exit(1);
            }
            // 阻塞普通写线程
            printf("阻塞普通写线程\n");
            pthread_cond_wait(&r_mutex_cond, &r_mutex);
            usleep(100*1000);//让上传线程先结束
            printf("唤醒普通写线程\n");
        }
        printf("1\n");
    }
}
