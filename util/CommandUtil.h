/*
 * @Description: 执行Linux系统命令
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:40:08
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

class CommandUtil {

public:

    /*
        区别：
        popen相当于是先创建一个管道，fork，关闭管道的一端，执行exec，返回一个标准的io文件指针。
        system相当于是先后调用了fork， exec，waitpid来执行外部命令
        popen本身是不阻塞的，要通过标准io的读取使它阻塞
        system本身就是阻塞的。popen和system都可以执行外部命令。
    */

    /**
    * @description: system
    * @param {type} 
    * @return: 
    */ 
    int cmd_system(const char * command, char *buff) {
        int status = -1;
        if(NULL == command) {
            return -1;
        }

        status = system(command);
        // fork失败
        if(status == -1) {
            printf("char * command: %s\t error: %s", command, strerror(errno));// 这里务必要把errno信息输出或记入Log
        }
        // 执行失败返回127
        else if(WIFEXITED(status)) {
            printf("normal termination, exit status = %d\n", WEXITSTATUS(status));//取得command执行结果
        }
        else if(WIFSIGNALED(status)) {
            printf("abnormal termination,signal number =%d\n", WTERMSIG(status)); //如果command被信号中断，取得信号值
        }
        else if(WIFSTOPPED(status)) {
            printf("process stopped, signal number =%d\n", WSTOPSIG(status));     //如果command被信号暂停执行，取得信号值
        }
    }

    /**
    * @description: fopen
    * @param {type} 
    * @return: 
    */ 
    int cmd_fopen(const char * command, char *buff) {
    int res = -1;
    FILE *fp;

        /*执行预先设定的命令，并读出该命令的标准输出*/
        fp = popen(command, "r");
        if(NULL == fp) {
            perror("popen执行失败！");
            return res;
        }

        memset(buff,0,sizeof(buff));
        while(fgets(buff, sizeof(buff), fp) != NULL) {
            //换行符去掉
            if('\n' == buff[strlen(buff)-1]) {
                buff[strlen(buff)-1] = '\0';
            }
            printf("命令【%s】 输出【%s】\r\n", command, buff);
            memset(buff,0,sizeof(buff));
        }

        /*等待命令执行完毕并关闭管道及文件指针*/
        res = pclose(fp);
        if(-1 == res) {
            perror("关闭文件指针失败");
        }
        else{
            printf("命令【%s】子进程结束状态【%d】命令返回值【%d】\r\n", command, res, WEXITSTATUS(res));
        }
        return res;
    }

    /**
    * @description: 是否包含指定字符串
    * @param {type} 
    * @return: 
    */    

    /**
    * @description: 返回执行结果
    * @param {type} 
    * @return: 
    */    

    /**
    * @description: 返回指定行和列
    * @param {type} 
    * @return: 
    */

};