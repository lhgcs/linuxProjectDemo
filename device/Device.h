/*
 * @Description: 设备节点
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:20:27
 */

#include<stdio.h>      /*标准输入输出定义*/
#include<stdlib.h>     /*标准函数库定义*/
#include<unistd.h>     /*Unix 标准函数定义*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>      /*文件控制定义*/
#include<termios.h>    /*PPSIX 终端控制定义*/
#include<errno.h>      /*错误号定义*/

class Device {

public:
    int fd = -1; // 文件句柄

    /**
     * @description: 设备节点是否存在
     * @param {type} 
     * @return: 
     */
    static int isExist(const char *deviceName) {
        if((access(deviceName, F_OK)) != -1) {
            printf("%s exist\n", deviceName);
            return 0;
        } else {
            printf("%s not exist\n", deviceName);
        }
        return -1;
    }

    /**
     * @description: 读设备
     * @param {type} 
     * @return: 
     */
    static int readDevice(const char *deviceName, char *str) {
        int len = 0;
        char buff[256];
        memset(buff, 0, 256);

        if(NULL == deviceName || NULL == str) {
            return -1;
        }

        int fd = open(deviceName, O_RDONLY);
        if(fd < 0) {
            fprintf(stderr,"open fail\n");
            return -1;
        }

        len = read(fd, buff, sizeof(buff));
        if(len < 0) {
            fprintf(stderr,"read fail\n");
            close(fd);
            return -1;
        }
        close(fd);
        memcpy(str,buff,len);
        return len;
    }

    /**
     * @description: 写设备
     * @param {type} 
     * @return: 
     */
    static int writeDevice(const char *deviceName, const char *str) {
        int len = -1;
        int fd = open(deviceName, O_WRONLY);
        if (fd > 0) {
            len = write(fd, str, strlen(str));
            close(fd);
        }
        return len;
    }

    /**
     * @description: 读写方式打开设备
     * @param {type} 
     * @return: 
     */
    int openDevice(const char *deviceName) {
        if(NULL == deviceName) {
            return -1;
        }

        fd = open(deviceName, O_RDWR);
        if(fd < 0) {
            fprintf(stderr,"open fail\n");
            return -1;
        }
        return fd;
    }

    /**
     * @description: 只读方式打开设备
     * @param {type} 
     * @return: 
     */
    int openDeviceByRead(const char *deviceName) {
        if(NULL == deviceName) {
            return -1;
        }

        fd = open(deviceName, O_RDONLY);
        if(fd < 0) {
            fprintf(stderr,"open fail\n");
            return -1;
        }
        return fd;
    }

    /**
     * @description: 只写方式打开设备
     * @param {type} 
     * @return: 
     */
    int openDeviceByWrite(const char *deviceName) {
        if(NULL == deviceName) {
            return -1;
        }

        fd = open(deviceName, O_WRONLY);
        if(fd < 0) {
            fprintf(stderr,"open fail\n");
            return -1;
        }
        return fd;
    }
    
    /**
     * @description: 关闭设备
     * @param {type} 
     * @return: 
     */
    void closeDevice() {
        if(fd > 0) {
            close(fd);
            fd = -1;
        }
    }
    
    /**
     * @description: 读设备
     * @param {type} 
     * @return: 
     */
    int readDevice(char *buff, int buffSize) {
        if(fd <=0 || NULL == buff) {
            return -1;
        }
        return read(fd, buff, buffSize);
    }

    /**
     * @description: 写设备
     * @param {type} 
     * @return: 
     */
    int writeDevice(const char *buff) {
        if(fd <=0 || NULL == buff) {
            return -1;
        }
        return write(fd, buff, strlen(buff));
    }
    
};