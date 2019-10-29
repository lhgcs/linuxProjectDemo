/*
 * @Description: 2019-10-20 13:14:10
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-29 17:22:02
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:22:02
 */


#include <cstdio>
#include <cstring>
#include <cerrno>
#include <termios.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "Device.h"

/* pin group base number name space,
 * the max pin number : 26*32=832.
 */
#define BANK_SIZE 32

char num;       //GPIO编号
char device[12];//设备节点 


class GpioDevice : public Device {

private:
    char deviceName[64];

public:
    /**
    * @description: 根据引脚名获取编号(引脚编号 = 控制引脚的寄存器基数 + 控制引脚寄存器位数，以 gpioc9 为例：64+9=73)
    * @param {type} 
    * @return: 
    */ 
    int getPinNum(char const *name, char *gpio) {
        char bank;
        int num=0;
        int group=0;

        if (!name || !gpio) {
            return -EINVAL;
        }

        /**  PA0   **/
        //从一个字符串中读进与指定格式相符的数据
    //    sscanf(name,"P%c%d", &bank, &num);
    //    if(num >= 32 || num < 0) {
    //		return -EINVAL;
    //	}

    //	if (bank >= 'a' && bank <= 'z') {
    //        num = num + BANK_SIZE * (bank - 'a');printf("%d",num);
    //	} else if (bank >= 'A' && bank <= 'Z') {
    //        num = num + BANK_SIZE * (bank - 'A');printf("%d",num);
    //	} else {
    //		return -EINVAL;
    //	}

        /**  GPIO0_A0   **/
        sscanf(name,"GPIO%d_%c%d", &group, &bank, &num);
        if(num >= 32 || num < 0) {
            return -EINVAL;
        }

        if (bank >= 'a' && bank <= 'z') {
            num = group * BANK_SIZE + (bank - 'a') + num;
            printf("%d",num);
        } else if (bank >= 'A' && bank <= 'Z') {
            num = group * BANK_SIZE + (bank - 'A') + num;
            printf("%d",num);
        } else {
            return -EINVAL;
        }
        return sprintf(gpio, "%d",  num);
    }

    /**
    * @description: 导出引脚（echo pinnum >> export）
    * @param {type} 引脚数字编号
    * @return: 
    */ 
    int gpioExport(char *pinNum) {
        int ret = writeDevice("/sys/class/gpio/export", pinNum);
        if(ret < 0) {
            printf("Export gpio fail, %s(%d)\n", strerror(errno), errno);
        } else {
            printf("init num:%s\n", pinNum);
        }
        return ret;
    }

    /**
    * @description: 取消导出(echo pinnum >> unexport)
    * @param {type} 
    * @return: 
    */ 
    int gpioUnexport(char *pinNum) {
        int ret = 0;
        // 文件存在
        char path[50] = "/sys/class/gpio/gpio";
        if(0 == isExist(path))   {
            ret = writeDevice("/sys/class/gpio/unexport", pinNum);
            if(ret < 0) {
                printf("Open and try unexport gpio error\n");
            } else {
                printf("try unexport num:%s\n",pinNum);
            }
        }
        return ret;
    }

    /**
    * @description: 初始化
    * @param {type} 
    * @return: 
    */ 
    int init(const char *pinName) {
        int ret;
        char pinNum[5]={0};
        memset(deviceName, 0, sizeof(deviceName));
        strcpy(deviceName, "/sys/class/gpio/gpio");

        if(NULL == pinName) {
            return -EINVAL;
        }

        ret = getPinNum(pinName, pinNum);
        if(ret < 0) {
            printf("get name fail,%d\n",ret);
            return -ret;
        }
        strcat(deviceName,pinNum);

        gpioUnexport(pinNum);

        ret = gpioExport(pinNum);
        if (ret >= 0) {
            strcat(device,"gpio");
            strcat(device,pinNum);
            printf("gpio path is : %s\n", device);
        } else {
            printf("gpio export fail\n");
        }
        return ret;
    }

    /**
    * @description: 设置输出
    * @param {type} 
    * @return: 
    */ 
    int setOutput() {
        int ret = -1;
        char device[64];
        memset(device, 0, sizeof(device));
        strcpy(device, deviceName);
        strcat(device,"/direction");
        printf("gpio direction path is : %s\n", device);
        ///sys/class/gpio/gpio100/direction

        ret = writeDevice(device, (char *)"out");
        if(ret < 0) {
            printf("ERR: gpio direction write error.\n");
        } else {
            ret = 0;
        }

        return ret;
    }

    /**
    * @description: 设置输出电平
    * @param {type} 
    * @return: 
    */ 
    int setOutputValue(int value) {
        int res=-1;

        char device[64];
        memset(device, 0, sizeof(device));
        strcpy(device, deviceName);
        strcat(device,"/value");

        char temp[1];
        sprintf(temp, "%d", value);
        res = writeDevice(device, temp);
        
        return res;
    }

    /**
    * @description: 设置输出电平
    * @param {type} 
    * @return: 
    */ 
    int getInputValue() {
        int res=-1;
        char value[1];

        char device[64];
        memset(device, 0, sizeof(device));
        strcpy(device, deviceName);
        strcat(device,"/value");

        res=readDevice(device, value);
        return res;
    }

};