/*
 * @Description: 按钮
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-28 23:38:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:19:03
 */

#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "Device.h"

class ButtonDevice : public Device {

public:

    /*
        关键结构体input_event信息：
        struct input_event {
        struct timeval time;
        __u16 type;
        __u16 code;
        __s32 value;
        };
    */

    /**
     * @description: 读设备
     * @param {type} 
     * @return: 
     */
    int readDevice(int *keyCode, int *keyStatus) {
        int ret = -1;
        struct input_event t;
        if(read(fd,&t,sizeof(t)) == sizeof(t)) {
            // 键盘或者按键，表示一个键码
            if(t.type == EV_KEY) {
                if(t.code == 100) {
                    if((t.value == 0)) {
                    }
                }
                else if(t.code == 200) {
                    if((t.value == 0)) {
                    }
                }
            }
        }
        return ret;
    }

};
