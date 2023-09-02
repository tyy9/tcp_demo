#include "bmp_plus.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
int lcd_open()
{
    fd_lcd = open("/dev/fb0", O_RDWR);
    if (fd_lcd == -1)
    {
        perror("打开lcd失败\n");
        return -1;
    }
    pb = mmap(NULL,
              800 * 480 * 4,
              PROT_READ | PROT_WRITE,
              MAP_SHARED,
              fd_lcd,
              0);
    if (pb == NULL)
    {
        perror("mmap error\n");
        return -1;
    }
    return 0;
}

void lcd_draw_point(int x, int y, unsigned int color)
{
    *(pb + y * 800 + x) = color;
}
void lcd_draw_point_mydraw(int x, int y, char *color)
{
    for(int i=y;i<y+20;i++){
        int color_buf=color[i*800*3+x*3+2]<<16|color[i*800*3+x*3+1]<<8|color[i*800*3+x*3];
        *(pb+i*800+x)=color_buf;
    }
}

void lcd_clear(unsigned int color)
{
    int x, y;
    for (int y = 479; y >= 0; y--)
    {
        for (int x = 0; x < 800; x++)
        {
            lcd_draw_point(x, y, color);
        }
    }
}

int lcd_draw_bmp(const char *pathname, int x, int y)
{
    int fd_bmp = open(pathname, O_RDWR);
    if (fd_bmp < 0)
    {
        perror("open file error\n");
        return -1;
    }
    // 位图文件头(54字节)
    BITMAPFILEHEADER file_head;
    read(fd_bmp, &file_head, sizeof(file_head));
    printf("%s图片大小:%d bytes\n", pathname, file_head.bfSize);
    // 位图信息段
    BITMAPINFOHEADER info_head;
    read(fd_bmp, &info_head, sizeof(info_head));
    printf("%s图片尺寸:宽%d 高%d\n", pathname, info_head.biWidth, info_head.biHeight);
    printf("%s图片颜色深度:%d\n", pathname, info_head.biBitCount);
    int bmp_rgb_size = info_head.biWidth * info_head.biHeight * info_head.biBitCount / 8;

    // 定义一个变长数组
    char bmp_buf[bmp_rgb_size];

    read(fd_bmp, bmp_buf, bmp_rgb_size);

    close(fd_bmp);

    // 显示图片
    // 起始坐标与结束坐标
    int x_s = x;
    int y_s = y;
    int x_e = x_s + info_head.biWidth;
    int y_e = y_s + info_head.biHeight;

    int x_pos, y_pos;

    unsigned int color;
    int i = 0;
    // 防止倒置输出
    for (y_pos = y_e - 1; y_pos >= y_s; y_pos--)
    {
        for (x_pos = x_s; x_pos < x_e; x_pos++, i += 3)
        {

            color = (bmp_buf[i + 2] << 16) | (bmp_buf[i + 1] << 8) | bmp_buf[i];
            lcd_draw_point(x_pos, y_pos, color);
        }
    }
    return 0;
}
int lcd_draw_bmp_lock(const char *pathname, int x, int y, int end)
{
    int fd_bmp = open(pathname, O_RDWR);
    if (fd_bmp < 0)
    {
        perror("open file error\n");
        return -1;
    }
    // 位图文件头(54字节)
    BITMAPFILEHEADER file_head;
    read(fd_bmp, &file_head, sizeof(file_head));
    printf("%s图片大小:%d bytes\n", pathname, file_head.bfSize);
    // 位图信息段
    BITMAPINFOHEADER info_head;
    read(fd_bmp, &info_head, sizeof(info_head));
    printf("%s图片尺寸:宽%d 高%d\n", pathname, info_head.biWidth, info_head.biHeight);
    printf("%s图片颜色深度:%d\n", pathname, info_head.biBitCount);
    int bmp_rgb_size = info_head.biWidth * info_head.biHeight * info_head.biBitCount / 8;

    // 定义一个变长数组
    char bmp_buf[bmp_rgb_size];

    read(fd_bmp, bmp_buf, bmp_rgb_size);

    close(fd_bmp);

    // 显示图片
    // 起始坐标与结束坐标
    int x_s = x;
    int y_s = y;
    int x_e = x_s + info_head.biWidth;
    int y_e = y_s + end;

    int x_pos, y_pos;

    unsigned int color;
    int i = 0;
    // 防止倒置输出
    for (y_pos = y_e - 1; y_pos >= y_s; y_pos--)
    {
        for (x_pos = x_s; x_pos < x_e; x_pos++, i += 3)
        {

            color = (bmp_buf[i + 2] << 16) | (bmp_buf[i + 1] << 8) | bmp_buf[i];
            lcd_draw_point(x_pos, y_pos, color);
        }
    }
    return 0;
}
int lcd_draw_bmp_swiper(const char *pathname, int x, int y)
{
    int fd_bmp = open(pathname, O_RDWR);
    if (fd_bmp < 0)
    {
        perror("open file error\n");
        return -1;
    }
    // 位图文件头(54字节)
    BITMAPFILEHEADER file_head;
    read(fd_bmp, &file_head, sizeof(file_head));
    printf("%s图片大小:%d bytes\n", pathname, file_head.bfSize);
    // 位图信息段
    BITMAPINFOHEADER info_head;
    read(fd_bmp, &info_head, sizeof(info_head));
    printf("%s图片尺寸:宽%d 高%d\n", pathname, info_head.biWidth, info_head.biHeight);
    printf("%s图片颜色深度:%d\n", pathname, info_head.biBitCount);
    int bmp_rgb_size = info_head.biWidth * info_head.biHeight * info_head.biBitCount / 8;

    // 定义一个变长数组
    char bmp_buf[bmp_rgb_size];

    read(fd_bmp, bmp_buf, bmp_rgb_size);

    close(fd_bmp);

    // 显示图片
    // 起始坐标与结束坐标
    int x_s = x;
    int y_s = y;
    int x_e = x_s + info_head.biWidth;
    int y_e = y_s + info_head.biHeight;

    int x_pos, y_pos;

    unsigned int color;
    int i = 0;
    // 防止倒置输出
    for (y_pos = y_e - 1; y_pos >= y_s; y_pos--)
    {
        if (y_pos <= 30)
        {
            continue;
        }
        for (x_pos = x_s; x_pos < x_e; x_pos++, i += 3)
        {

            color = (bmp_buf[i + 2] << 16) | (bmp_buf[i + 1] << 8) | bmp_buf[i];
            lcd_draw_point(x_pos, y_pos, color);
        }
    }
    return 0;
}
int lcd_close(void)
{
    if (fd_lcd > 0)
    {
        close(fd_lcd);
    }
    if (pb)
    {
        munmap(pb, 800 * 480 * 4);
    }
    return 0;
}
