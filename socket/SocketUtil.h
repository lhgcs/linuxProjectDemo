/*
 * @Description: socket
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:47:16
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

class SocketUtil {

public:

    int fd = -1;

    /**
     * @description: 创建socket
     * @param {type} 
     * @return: 
     */
    int createSocket() {
        // IPv4协议,字节流套接字
        fd = socket(AF_INET, SOCK_STREAM, 0);
        return fd;
    }
    
    /**
     * @description: 关闭socket
     * @param {type} 
     * @return: 
     */
    int closeSocket() {
        if(fd > 0) {
            shutdown(fd,SHUT_RDWR);
            close(fd);
            fd = -1;
        }
    }

    /**
     * @description: 连接
     * @param {type} 
     * @return: 
     */
    int connect2Server(int port) {
        struct hostent *host;
        struct sockaddr_in serv_addr;

        // 设置 sockaddr_in 结构体中相关参数
        serv_addr.sin_family = AF_INET;                          // 地址族
        serv_addr.sin_port = htons(port);                        // 端口号
        serv_addr.sin_addr = *((struct in_addr *)host->h_addr);  // IP地址
        bzero(&(serv_addr.sin_zero),8);                          // 填充0以保持与 struct sockaddr 同样大小

        // 调用 connect 函数主动发起对服务器端的连接
        int res = connect(fd,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr));// 服务器端地址,地址长度
        if(res == 0) { // 立即创建了socket链接
        } else {
            switch(errno) {
                case EISCONN:
                    res = 0;
                    break;
                case EINPROGRESS:// errno == EINPROGRESS表示正在建立链接
                case EALREADY:
                    // res = socket_wait_writable(fd, 30000);
                    res=-1;
                    break;
                default:
                    res = -EFAULT;
                    break;
            }

        }
    }

    /**
     * @description: 写
     * @param {type} 
     * @return: 
     */
    int writeSocket(const char *buff) {
        int len = send(fd, buff, strlen(buff), 0);// 设置为异步方式，就可以一边读，一边写
        if(len<0) {
            printf("消息发送失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));
        }else if(len == strlen(buff)) {
            printf("write ok\n");
        } else {
            
        }
    }

    /**
     * @description: 读
     * @param {type} 
     * @return: 
     */
    int readSocket(char *buff, int buffSize) {
        memset(buff, 0, sizeof(buff));
        fd_set readfd;               // 定义要监视的集合,监听文件描述符是否可读，不监听可以传入 NULL
        fd_set *writefds=NULL;       // 监听文件描述符是否可写 ，不监听可以传入 NULL
        fd_set *exceptfds=NULL;      // 监听文件描述符是否有异常，不监听可以传入 NULL
        struct timeval time;         // 设置监视的时间  0：从不等待  NULL：永远等待

        int heartCnt=0;
        int timeoutCnt=0;

        while(1) {
            // 由于select()会修改字符集，因此如果select()调用是在一个循环中，则描述符集必须被重新赋值
            FD_ZERO(&readfd);     // 清除一个文件描述符集
            FD_SET(fd, &readfd);  // 将一个文件描述符加入文件描述符集中

            // timeout的值必须在每次select()之前重新赋值，因为操作系统会修改此值
            // 在非必须的情况下，尽量使用不超时的select()，即将utimeout参数设置为NULL
            time.tv_sec = 0;
            time.tv_usec = 1000000;

            /*调用 select 函数*/
            // 最高的文件描述符加1,读文件描述符集合,写文件描述符集合,异常处理文件描述符集合
            int res = select(fd+1, &readfd,writefds, exceptfds, &time);
            if(res > 0) {                                  // 准备好的文件描述符
                if(FD_ISSET(fd,&readfd)>0) {           // 用于测试指定的文件描述符是否在该集合中
                    // 将检测到_socket_fd读事件或写时间，并不能说明connect成功
                    int error = 0;
                    socklen_t len = sizeof(error);
                    if(getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {// 建立简介失败close(_socket_fd)
                        continue;
                    }
                    if(error != 0) {// 建立链接失败close(_socket_fd)
                        continue;
                    } else {          // 建立链接成功
                    }

                    int len = recv(fd,buff,sizeof(buff),0);
                    if(len == -1) {
                        printf("recv error");
                    }else if(len == 0) {// 当返回值为0时，表示对端已经关闭了这个链接，我们应该自己关闭这个链接
                        FD_CLR(fd,&readfd);
                        break;
                    } else {
                       
                    }
                }
            }
        }
    } 

};
