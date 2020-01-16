/*
 * @Description: 线程
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-23 00:06:29
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:45:16
 */

typedef void *(*Funn)(void *);//定义Funn类型是一个指向函数的指针，该函数参数为void*，返回值为void*

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include <signal.h>
#include <unistd.h>

class ThreadUtil {

public:

    /**
    * @description: 创建线程 
    * @param {type} 是否阻塞 子线程ID
    * @return: 
    */ 
    void create_thread(Funn fun, int isWait, pthread_t *_tid) {
        pid_t pid;
        pthread_t tid;

        int err = pthread_create(&tid, NULL,fun, NULL); // 参数为空
        *_tid = tid;
        pid = getpid();
        printf("process id:%d,thread id:%d\n",(uint)pid,(uint)tid);

        //获取本线程id
        printf("thread ID is %lu\n",pthread_self());

        if(err != 0) {
            printf("%s错误出现在第%d行",__FUNCTION__,__LINE__);
            printf("%s:%s\n",__func__,strerror(err));
        }

        if(isWait) {
            //主线程等待子线程完成
            err = pthread_join(tid, NULL);
            // pthread_timedjoin_np(tid, &rvall, 1000);//等待线程结束，设置超时
            if(err != 0) {
                printf("%s:%s\n",__func__,strerror(err));
            }
        }
    }

    /**
    * @description: 检测线程是否启动
    * @param {type} 子线程ID
    * @return: 
    */ 
    int test_thread(pthread_t tid) {
        int pthread_kill_err;
        if(tid <= 0) {
            return 1;
        }
        pthread_kill_err = pthread_kill(tid,0);
        //int ret = pthread_tryjoin_np(tid, NULL);//前提是线程不能被设置成detached，否则函数一直返回22

        if(0 == pthread_kill_err) {
            printf("ID为0x%x的线程目前仍然存活。/n",(unsigned int)tid);
        } else if(3 == pthread_kill_err) {//ESRCH
            printf("ID为0x%x的线程不存在或者已经退出。/n",(unsigned int)tid);
        } else if(22 == pthread_kill_err) {//EINVAL
            printf("发送信号非法。/n");
        } else {

        }
        return pthread_kill_err;
    }

    /**
    * @description: 启动线程
    * @param {type} 子线程ID
    * @return: 
    */ 
    void start_thread(Funn fun, int isWait, pthread_t *_tid) {
        if (*_tid > 0) {
            int ret = test_thread(*_tid);//xiaoyu0paocuo
            //线程在运行
            if (0 == ret) {
                return;
            }
        }
        create_thread(fun, isWait, _tid);
    }

    /**
    * @description: 关闭线程
    * @param {type} 子线程ID 是否阻塞等待资源释放
    * @return: 
    */ 
    void close_thread(pthread_t *tid, int isWait) {
        if (*tid > 0) {
            int ret = test_thread(*tid);
            if (0 == ret) {
                //取消线程
                pthread_cancel(*tid);
                pthread_testcancel();
                if(isWait) {
                    pthread_join(*tid, NULL);//子线程合入主线程，主线程阻塞等待子线程结束，然后回收子线程资源
                } else {
                    pthread_detach(*tid);    //主线程与子线程分离，子线程结束后，资源自动回收
                }
            }
        }
    }

    /**
    * @description: 线程任务
    * @param {type} 通过结构体指针或数组指针传入多个参数
    * @return: 
    */ 
    void *thread_task(void *arg) {
        printf("start thread\n"); 

        while (1) {
            
            usleep(1000000);
        }

        printf("end thread\n");
        pthread_exit(0);
    }

};
