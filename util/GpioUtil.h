/*
 * @Description: GPIO
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:42:55
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>

typedef struct {
	unsigned int groupNum;
	unsigned int bitNum;
	unsigned int value;
} GpioInfo;

#define GPIO_FILE      "/dev/gpio"
#define GPIO_SET_DIR   0x1
#define GPIO_GET_DIR   0x2
#define GPIO_READ_BIT  0x3
#define GPIO_WRITE_BIT 0x4
#define IO_CONFIG      0x5

class GpioUtil {

public:

    /**
    * @description: 设置
    * @param {type} 
    * @return: 
    */
    int ctrlGpio(const char *device, GpioInfo *gpio, int ctrl) {
        int fd;
        fd = open(device, O_RDWR);
        if(fd < 0) {
            printf("open %s fail\n", GPIO_FILE);
            return -1;
        }
        ioctl(fd, ctrl, gpio);
        close(fd);
        return 0;
    }

    /**
     * @description: 设置GPIO
     * @param {type} 
     * @return: 
     */
    int setGpio(int group, int bitNum, unsigned int configuration) {
	    GpioInfo gpio;
        gpio.groupNum = group;
        gpio.bitNum = bitNum;
        gpio.value = 1;
        return ctrlGpio(GPIO_FILE, &gpio, IO_CONFIG);
    }

    /**
     * @description: 设置为输出
     * @param {type} 
     * @return: 
     */
    int setGpioOutput(int group, int bitNum) {
        GpioInfo gpio;
        gpio.groupNum = group;
        gpio.bitNum = bitNum;
        gpio.value = 1;
        return ctrlGpio(GPIO_FILE, &gpio, GPIO_SET_DIR);
    }

    /**
     * @description: 设置为输入
     * @param {type} 
     * @return: 
     */
    int setGpioIutput(int group, int bitNum) {
        GpioInfo gpio;
        gpio.groupNum = group;
        gpio.bitNum = bitNum;
        gpio.value = 0;
        return ctrlGpio(GPIO_FILE, &gpio, GPIO_SET_DIR);
    }

    /**
     * @description: 设置gpio值
     * @param {type} 
     * @return: 
     */
    int setGpioValue(int group, int bitNum, int value) {
        GpioInfo gpio;
        gpio.groupNum = group;
        gpio.bitNum = bitNum;
        gpio.value = value;
        return ctrlGpio(GPIO_FILE, &gpio, GPIO_WRITE_BIT);
    }

    /**
     * @description: 获取gpio值
     * @param {type} 
     * @return: 
     */
    int getGpioValue(int group, int bitNum, int *value) {
        GpioInfo gpio;
        gpio.groupNum = group;
        gpio.bitNum = bitNum;
        int res = ctrlGpio(GPIO_FILE, &gpio, GPIO_READ_BIT);
        if(0 == res) {
            *value = gpio.value;
            return 0;
        }
        return -1;
    }

};