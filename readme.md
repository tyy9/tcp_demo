1.介绍
    一个服务器与客户端的简单双向通信demo，内含双向通信，文件双向通信（可互相上传与下载）
2.文件结构
.
├── bin
│   ├── client_main.c //客户端主函数
│   └── server_main.c   //服务器主函数
├── client //客户端运行程序
├── include //jpeg头文件
│   ├── jconfig.h
│   ├── jerror.h
│   ├── jmorecfg.h
│   └── jpeglib.h
├── makefile //运行脚本
├── readme.md //说明文档
├── resource    //资源文件，下载与上传的主要文件夹
│   ├── 02.bmp
│   ├── 03.bmp
│   ├── 1.jpg
│   ├── speed.jpg
│   └── w1.bmp
├── server  //服务器运行程序
└── src //源文件
    ├── bmp_plus.c //bmp渲染
    ├── bmp_plus.h
    ├── client.c    //客户端，双向通信与双向文件通信
    ├── client.h
    ├── my_head.h   //头文件集合
    ├── server.c    //服务器，双向通信与双向文件通信
    └── server.h
3.使用说明
    编译make即可，需要修改server.c与client.c里的服务器与客户端的ip地址
    传送文件:send_+文件夹/文件名
    下载文件:get_+文件夹/文件名
    显示图片（仅支持bmp）: show_+文件夹/文件名