/*
 * @Description: 串口
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:27:10
 */

#include "Device.h"

class SerialDevice : public Device {

public:

    /**
     * @description: 打开设备
     * @param {type} 
     * @return: 
     */
    int openDevice(const char *deviceName) {
        fd = open(deviceName, O_RDWR | O_NOCTTY | O_NDELAY);  // O_NOCTTY 表示不能把本串口当成控制终端  O_NONBLOCK非阻塞方式
        if (fd < 0) {
            printf ("Can't open serial\n");
            return fd;
        }

        //恢复串口为阻塞状态
        if(fcntl(fd, F_SETFL, 0) < 0)  {
            printf("fcntl failed!\n");
            return -1;
        } else  {
            printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
        }
        //测试是否为终端设备
        if(0 == isatty(STDIN_FILENO))  {
            printf("standard input is not a terminal device\n");
            return -1;
        } else  {
            printf("isatty success!\n");
        }
        return fd;
    }

    /**
     * @description: 配置
     * @param {type} 
     * @return: 
     */
    int setParams(long BaudRate, char cBits, char cCheck, char cStops, int FlowCtrl) {
        struct termios sp_settings;
        memset (&sp_settings, 0, sizeof (sp_settings));

        if (fd < 0) {
            return -1;
        }

        // 得到与fd指向对象的相关参数，并将它们保存于options, 该函数还可以测试配置是否正确，该串口是否可用等
        if(0 != tcgetattr(fd, &sp_settings)) {
            perror("SetupSerial 1");
            return -1;
        }

        sp_settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // 规范输入|回送输入数据|回送擦除字符|使能SIGINTR，SIGSUSP， SIGDSUSP和 SIGQUIT 信号，这些都关闭
        sp_settings.c_cflag |= (CLOCAL | CREAD);                // 控制模式：忽略modem信号线(保证程序不会占用串口) | 打开接收功能(使得能够从串口中读取输入数据)
        sp_settings.c_oflag &= ~OPOST;                          // 输出模式：原始数据（RAW）输出
        // options.c_cflag &= ~CSIZE;                           // 屏蔽其他标志位
        // options.c_lflag &= ~(ISIG | ICANON);

        // 波特率
        cfsetispeed (&sp_settings, BaudRate); // 设置输入波特率
        cfsetospeed (&sp_settings, BaudRate); // 设置输出波特率
        
        // 设置数据位数
        switch (cBits) {
            case '5':sp_settings.c_cflag |= CS5; break; // 数据位为5bit
            case '6':sp_settings.c_cflag |= CS6; break; // 数据位为6bit
            case '7':sp_settings.c_cflag |= CS7; break; // 数据位为7bit
            case '8':sp_settings.c_cflag |= CS8; break; // 数据位为8bit
            default: sp_settings.c_cflag |= CS8; break; // 数据位为8bit
        }

        // 奇偶校验位
        switch (cCheck) {
            case 'N':
            case 'n':
                    sp_settings.c_cflag &= ~PARENB;
                    break; // 无校验       
            case 'O':
            case 'o':
                    sp_settings.c_cflag |= PARODD; 
                    sp_settings.c_iflag |= (INPCK | ISTRIP);
                    break; // 奇校验
            case 'E':
            case 'e':
                    sp_settings.c_cflag &= ~PARODD;
                    sp_settings.c_iflag |= (INPCK | ISTRIP); 
                    break; // 偶校验
            case 'S':
            case 's': 
                    sp_settings.c_cflag &= ~PARENB;
                    sp_settings.c_cflag &= ~CSTOPB;
                    break; // 设置为空格
            default: sp_settings.c_cflag &= ~PARENB; break; // 无校验
        }

        // 停止位
        switch (cStops) {
            case '1':sp_settings.c_cflag &= ~CSTOPB; break; // 1位停止位
            case '2':sp_settings.c_cflag |= CSTOPB;  break; // 2位停止位
            default: sp_settings.c_cflag &= ~CSTOPB; break; // 1位停止位
        }

        // 数据流控制
        switch (FlowCtrl) {
            case 1:
                sp_settings.c_cflag |= CRTSCTS;
                break; // 硬件流控制
            case 2:
                sp_settings.c_iflag |= (IXON | IXOFF | IXANY);
                break; // 软件流控制，IXON打开输出控制，IXOFF打开输入控制
            default:
                sp_settings.c_iflag &= ~(IXON | IXOFF | IXANY);
                sp_settings.c_cflag &= ~CRTSCTS;
                break; // 不使用流控制
        }

        // 读取字符的最小数量，至少要读到0个字符才会返回
        sp_settings.c_cc[VMIN] = 0;                
        // 读取第一个字符的等待时间*(1/10)s
        sp_settings.c_cc[VTIME] = 0;                
        
        // 将修改后数据设置到串口中
        if(0 != tcsetattr(fd, TCSANOW, &sp_settings)) {
            perror("set uart error!!!");
            return -1;
        }

        // 清空缓冲区
        clearBuff();
        return 0;
    }

    /**
    * @description: 清空缓冲区
    * @param {type} 
    * @return: 
    */
    void clearBuff() {
        if(fd > 0) {
            tcflush(fd, TCIOFLUSH);
        }
    }

    /**
    * @description: 清空输入缓冲区
    * @param {type} 
    * @return: 
    */
    void clearInputBuff() {
        if(fd > 0) {
            tcflush(fd, TCIFLUSH);
        }
    }

    /**
    * @description: 清空输出缓冲区
    * @param {type} 
    * @return: 
    */
    void clearOutputBuff() {
        if(fd > 0) {
            tcflush(fd, TCOFLUSH);
        }
    }

    /**
    * @description: 打印16进制的数据
    * @param {type} 
    * @return: 
    */
    void printfHex(unsigned char *buff, int len) {
        printf("data: ");
        for(int i=0; i<len; i++) {
            printf(" 0x%02x", buff[i]);
        }
        printf("\n");
    }

    /**
     * @description: 读设备
     * @param {type} 
     * @return: 
     */
    int readDevice(char *buff, int buffSize) {

        int len, fs_sel;
        fd_set fs_read;

        if(fd <=0 || NULL == buff) {
            return -1;
        }

        FD_ZERO(&fs_read);
        FD_SET(fd,&fs_read);

        time.tv_sec = 10;
        time.tv_usec = 0;

        // 使用select实现串口的多路通信
        fs_sel = select(fd+1, &fs_read, NULL,NULL, &time);
        printf("fs_sel = %d\n",fs_sel);
        if(fs_sel){
            len = read(fd, buff, buffLen);
            printf("I am right!(version1.2) len = %d fs_sel = %d\n",len,fs_sel);
            return len;
        }else{
            printf("Sorry,I am wrong!");
        }

        return len;
    }

    int uart_read(int fd, char *rcv_buf,int data_len)
{
    
    struct timeval time;

    if(fd < 0 || rcv_buf == NULL){
        return FALSE;
    }

    
}

};