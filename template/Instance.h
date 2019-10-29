/*
 * @Description: 单例模板
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-24 23:52:08
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:38:52
 */

#include <pthread.h>

template<class T>
class Instance {
    
private:
    static T *mInstance;
    static pthread_mutex_t mutex;

    /**
     * @description: 构造函数（定义为private，防止在其它地方实例化）
     * @param {type} 
     * @return: 
     */
    Instance() {
    }

public:
    /**
     * @description: 获取实例
     * @param {type} 
     * @return: 
     */
    static T *getInstance() {
        // 双检锁机制
        if(NULL == mInstance) {
            pthread_mutex_lock(&mutex);
            if(NULL == mInstance) {
                mInstance = new T();
            }
            pthread_mutex_unlock(&mutex);
        }
        return mInstance;
    }

};

template<class T>
T *Instance<T>::mInstance = NULL;
template<class T>
pthread_mutex_t Instance<T>::mutex;