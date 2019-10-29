/*
 * @Description: 设备节点
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:20:50
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>
#include <stdio.h>

class DeviceUtil {

public:
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
     * @description: 设备节点是否可读
     * @param {type} 
     * @return: 
     */
    static int isRead(const char *deviceName) {
        if((access(deviceName, R_OK)) != -1) {
            printf("%s can read\n", deviceName);
            return 0;
        } else {
            printf("%s can not read\n", deviceName);
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

        len = read(fd,buff,sizeof(buff));
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
    
};