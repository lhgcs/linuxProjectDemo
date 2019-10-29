/*
 * @Description: CPU信息
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:28:25
 */

#include "DeviceUtil.h"

#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>               // struct timespec, clock_gettime
#include <stdio.h>              // FILE, fscanf, stdout, printf
//#define _POSIX_C_SOURCE 199309L // enable interface for struct timespec ...
const unsigned int N = 1000;

typedef struct{
    struct timespec t;
    unsigned temp;
}
RecT;

class CpuDevice {

public:

    /**
     * @description: 获取CPU温度
     * @param {type} 
     * @return: 
     */
    static float getCpuTemp() {

        float temp = 0;
        char buff[64];
        const char *device = "/sys/class/thermal/thermal_zone0/temp";
        if(DeviceUtil::readDevice(device, buff) > 0) {
            //temp = str2long(buff)/1000.0;
            printf("temp=%.02f\n",temp);
        } else {
            RecT r [N];
            for (unsigned i = 0; i < N; ++i) {
                (void) clock_gettime (CLOCK_REALTIME, &r [i].t);
                FILE *fp = fopen ("/sys/devices/virtual/thermal/thermal_zone0/temp", "r");
                (void) fscanf (fp, "%u", &r[i].temp);
                (void) fclose (fp);
            }
            for (unsigned i = 0; i < N; ++i) {
                printf ("%lu.%06llu %u\n", r [i].t.tv_sec, (r [i].t.tv_nsec + 500ull) / 1000ull, r [i].temp);
            }
        }

        ///proc/acpi/thermal_zone/THRM/temperature

        return temp;  
    }

    /**
     * @description: 获取CPU ID
     * @param {type} 
     * @return: 
     */
    static int getCpuSeiral(char *cpuId) {
        int len = 0;
        char buff[2048];
        char path[64] = "/sys/firmware/devicetree/base/serial-number";
        memset(buff, 0, sizeof(buff));

        if(0 == DeviceUtil::isExist(path)) {
            len = DeviceUtil::readDevice(path, buff);
            if(len > 0) {
                //去掉结尾的\r\n
                //get_char(buff);
                cpuId = buff;
            } else {
                fprintf(stderr,"read fail1\n");
            }
        }
   
        // cat /proc/cpuinfo
        memset(path, 0, sizeof(path));
        memcpy(path, "/proc/cpuinfo", 14);

        if(0 == DeviceUtil::isExist(path)) {
            len = DeviceUtil::readDevice(path, buff);
            if(len > 0) {
                char id[32];
                    memset(id, 0, sizeof(id));

                    //查找Serial字符串
                    const char *str="Serial";
                    int strLen = strlen(str);

                    int i = 0;
                    int j = 0;
                    for(;i<len;i++) {
                        if (buff[i] == str[j]) {
                            j++;

                            if (j == strLen - 1) {
                                i -= strLen -1;
                                break;
                            }
                        } else {
                            j = 0;
                        }
                    }
                    printf("%s\n",buff+i);

                    //字符串匹配
                    int res = sscanf(buff + i, "Serial%*[^:]:%16s", id);
                    //Serial\s{0,5}:\s{0,2}[0-9a-zA-z]{0,16}
                    if(res > 0) {
                        printf("%d:%s\n",res,id);
                        //去掉结尾的\r\n
                        //get_char(id);
                        cpuId = id;
                    } else {
                        printf("not find\n");
                    }
            } else {
                fprintf(stderr,"read fail1\n");
            }
        }
    }

    /**
    * @description: 获取应用程序当前的路径
    * @param {type} 
    * @return: 
    */ 
    static int getCurrentProgramPath(char *programPath) {
        int len;
        char buff[1024];
        char* p = NULL;

        if (NULL == programPath){
            return -1;
        }

        //通过访问/proc/self/目录来获取自己的系统信息，相当于/proc/本进程pid
        len = readlink("/proc/self/exe", buff, sizeof(buff));
        if(len < 0 || len >= sizeof(buff)){
            return -1;
        }
        buff[len] ='\0';

        //查找最后一次出现的位置
    //    p = strrchr(buff,'/');
    //    if(NULL == p || p == buff){
    //        return -1;
    //    }

        //截取路径
        memset(programPath, 0, sizeof(programPath));
    //    strncpy(programPath, buff, p - buff);
        printf("path:%s\n", programPath);
        return 0;
    }

    /**
    * @description: 获取硬盘ID
    * @param {type} 
    * @return: 
    */
    static void getDiskSerial(char *diskname, char *serial) {
        int len = 0;
        char buff[128];
        char diskDevice[128];
        char realDevice[128];
        char *temp = NULL;
        int i = 0;

        if((NULL == diskname) || (NULL == serial)){
            return;
        }

        //连接实际指向了../devices/platform/fe330000.sdhci/mmc_host/mmc1/mmc1:0001/block/mmcblk1
        sprintf(diskDevice, "/sys/block/%s", diskname);
        len = readlink(diskDevice, buff, 256);
        if(-1 == len){
            return;
        }
        buff[len] = 0;
        sprintf(realDevice, "%s/%s", "/sys/block", buff);
        for (i=0;i<6;i++){
            temp = strrchr(realDevice, '/');
            if(temp){
                *temp = 0;
                }
        }
        strcat(realDevice, "/serial");
        memset(buff, '\0', sizeof(buff));
        printf("[len=%ld,serial: %s]\n", strlen(buff), buff);
    }


    /**
    获取CPU ID
    相当于：hexdump /sys/bus/nvmem/devices/rockchip-efuse0/nvmem
    */
    int getCpuSerialHex(){

        int len = 0;
        char buff[64];
        char path[64] = "/sys/bus/nvmem/devices/rockchip-efuse0/nvmem";
        memset(buff, 0, sizeof(buff));

        if(0 == DeviceUtil::isExist(path)) {
            len = DeviceUtil::readDevice(path, buff, sizeof(buff));
            if(len > 0) {
                printf("cpu id:%x\n",buff);
                cpuId = buff;
            } else {
                fprintf(stderr,"read fail1\n");
            }
        }
        return 0;
    }
};
