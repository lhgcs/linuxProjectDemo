/*
 * @Description: glog日志
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-29 17:30:00
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:30:27
 */

#ifndef LOGUTIL_H
#define LOGUTIL_H

#include <glog/logging.h>

#define INFO_LEVEL  0
#define WARN_LEVEL  1
#define ERROR_LEVEL 2
#define FATAL_LEVEL 3

class LogUtil
{
public:

    /**
     * @description: 初始化
     * @param {type} argv[0]
     * @return: 
     */
    static int init(char *programeName, char *saveDir) {
        FLAGS_logtostderr = false;               // 设置日志消息是否转到标准输出而不是日志文件
        FLAGS_alsologtostderr = true;            // 设置日志消息除了日志文件之外是否去标准输出
        FLAGS_colorlogtostderr = true;           // 设置记录到标准输出的颜色消息（如果终端支持）
        FLAGS_log_prefix = true;                 // 设置日志前缀是否应该添加到每行输出
        FLAGS_logbufsecs = 0;                    // 设置可以缓冲日志的最大秒数，0指实时输出
        FLAGS_max_log_size = 10;                 // 设置最大日志文件大小（以MB为单位）
        FLAGS_stop_logging_if_full_disk = true;  // 设置是否在磁盘已满时避免日志记录到磁盘

        // 初始化
        google::InitGoogleLogging(programeName);
        // 指定目录下
        strcat(saveDir, "log_");
        google::SetLogDestination(google::INFO, saveDir);   // INFO级别的日志目录
        google::SetLogDestination(google::ERROR, saveDir);  // ERROR级别的日志目录
        google::SetLogDestination(google::WARNING, saveDir);// WARNING级别的日志目录
        google::SetStderrLogging(google::INFO);             // 大于INFO级别的都输出到标准输出的时候        
    }

    /**
     * @description: 关闭
     * @param {type} 
     * @return: 
     */
    static void close() {
        google::ShutdownGoogleLogging();
    }

    /**
     * @description: 输出日志
     * @param {type} 
     * @return: 
     */
    static void log(int &level, std::string &info) {
        switch(level) {
            case INFO_LEVEL:  LOG(INFO) << info;    break; // 输出一个Info日志
            case WARN_LEVEL:  LOG(WARNING) << info; break; // 输出一个Warning日志
            case ERROR_LEVEL: LOG(ERROR) << info;   break; // 输出一个Error日志
            case FATAL_LEVEL: LOG(FATAL) << info;   break; // 输出一个Fatal日志，这是最严重的日志并且输出之后会中止程序
        }
    }
    
// for(int i=0; i< 10;i++){
//     // 当条件满足时输出日志
//     LOG_IF(INFO, 1 > 0) << "LOG_IF";

//     // 每2次输出当前日志（输出1,3,5……的日志）
//     LOG_EVERY_N(INFO, 2) << google::COUNTER << " LOG_EVERY_N";

//     // 条件满足并且每2次输出当前日志
//     LOG_IF_EVERY_N(INFO, (1 > 0), 2) << google::COUNTER << " LOG_IF_EVERY_N";

//     // 只输出前两条日志，后面的都不输出
//     LOG_FIRST_N(INFO, 2) << google::COUNTER << " LOG_FIRST_N";
// }

};

#endif //  LOGUTIL_H
