/*
 * @Description: I2C
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-28 23:38:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:24:28
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <string.h>
#include <malloc.h>

#include "Device.h"

/*
    开启：
    直接调用控制器收发i2c设备数据的接口(dev-interface)
    内核配置里的dev-interface选项:
    make menuconfig ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
    Device Drivers  --->
        <*> I2C support  --->
            <*>   I2C device interface

    参数：
    struct i2c_msg {
        __u16 addr;  //  slave address
        __u16 flags; //  写为0, 读为I2C_M_RD
        __u16 len;   //  msg length
        __u8 *buf;   //  pointer to msg data
    };

    struct i2c_rdwr_ioctl_data {
            struct i2c_msg *msgs;  //  ptr to array of simple messages/
            int nmsgs;             //  number of messages to exchange/
    };
*/

class I2cDevice : public Device {

public:

    /**
     * @description: 打开设备
     * @param {type} "/dev/i2c-0"
     * @return: 
     */
    int openDevice(const char *device){
        int fd = open(device, O_RDWR);
        if (fd < 0){
            return 1;
        }
        // 设置超时和传输失败时的重试次数(非必须作)
        ioctl(fd, I2C_TIMEOUT, 10);
        ioctl(fd, I2C_RETRIES, 2);
        return fd;
    }

    /**
     * @description: 读设备
     * @param {type} 
     * @return: 
     */
    int readDevice(unsigned short deviceAddr, unsigned char *data, unsigned short dataLen){
        unsigned char addr = 0;
        int ret =0;
        // 需要传输的数据
        struct i2c_msg msgs[2] = {
            {deviceAddr, (unsigned short)0,              1, &addr},// write
            {deviceAddr, (unsigned short)I2C_M_RD, dataLen, data}, // read
        };
        struct i2c_rdwr_ioctl_data idata = {
            .msgs = msgs,
            .nmsgs = 2,
        };
        // 调用此ioctl函数类似设备驱动里调用i2c_transfer函数。调用一次，只会在完成时发出一个停止信号，不管发出了多少个i2c_msg。　而且每一个i2c_msg都会有一个开始信号
        ret = ioctl(fd, I2C_RDWR, &idata);
        if (ret < 0){
            perror("ioctl");
        }
        return ret;
    }

    /**
     * @description: 写设备
     * @param {type} 
     * @return: 
     */
    int writeDevice(unsigned short deviceAddr, unsigned char *data, unsigned short dataLen){
        unsigned char addr = 0;
        int ret =0;
        // 需要传输的数据
        struct i2c_msg msgs[2] = {
            {deviceAddr, 0,      1, &addr},// write
            {deviceAddr, 0, dataLen, data},// write
        };
        struct i2c_rdwr_ioctl_data idata = {
            .msgs = msgs,
            .nmsgs = 2,
        };
        // 调用此ioctl函数类似设备驱动里调用i2c_transfer函数。调用一次，只会在完成时发出一个停止信号，不管发出了多少个i2c_msg。　而且每一个i2c_msg都会有一个开始信号
        ret = ioctl(fd, I2C_RDWR, &idata);
        if (ret < 0){
            perror("ioctl");
        }
        return ret;
    }

};
