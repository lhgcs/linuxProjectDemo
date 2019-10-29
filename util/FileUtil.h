/*
 * @Description: 文件
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:40:44
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class FileUtil {

public:

    /**
    * @description: 是否存在
    * @param {type} 
    * @return: 
    */ 
    static int isExist(const char *fileName) {
        
        return -1;
    }

    /**
    * @description: 创建文件
    * @param {type} 
    * @return: 
    */    

    /**
    * @description: 删除文件
    * @param {type} 
    * @return: 
    */

    /**
    * @description: 获取文件大小（获取光标结束位置）
    * @param {type} 
    * @return: 
    */ 
    static long getSize(const char *fileName) {
        long len = -1;
        int fd = open(fileName, O_RDONLY);
        if (fd > 0) {
            // 光标移动到文件末尾
            len = lseek(fd, 0, SEEK_END);
            close(fd);
        }
        return len;
    }

    /**
    * @brief 读文件
    * @param fileName      文件名
    * @param readBuff      缓冲
    * @param readBuffSize  缓冲大小(如果缓冲是指针，不能用sizeof)
    * @param seek          偏移
    * @return              读取的字节数
    */
    static long readFile(const char *fileName, char *readBuff, unsigned int readBuffSize, long seek) {
        long len = -1;
        long fileSize = 0;
        int fd = open(fileName, O_RDONLY);
        if (fd > 0) {
            // 光标移动到指定位置
            fileSize = lseek(fd, 0L, SEEK_END);
            seek = seek > fileSize ? fileSize : seek;
            len = lseek(fd, seek, SEEK_SET);
            if(len != -1) {
                len = read(fd, readBuff, readBuffSize);
            }
            close(fd);
        }
        return len;
    }

    /**
    * @brief write      写文件（覆盖）
    * @param fileName   文件名
    * @param writeBuff  缓冲
    * @return           写入的字节数
    */
    static long writeFile(const char *fileName, const char *writeBuff) {
        long len = -1;
        int fd = open(fileName, O_WRONLY | O_CREAT, 0777);
        if (fd > 0) {
            len = write(fd, writeBuff, strlen(writeBuff));
            close(fd);
        }
        return len;
    }

    /**
    * @brief append     文件内容追加
    * @param fileName   文件名
    * @param writeBuff  缓冲
    * @return           写入的字节数
    */
    static long append(const char *fileName, const char *writeBuff) {
        long len = -1;
        int fd = open(fileName, O_WRONLY | O_CREAT, 0777);
        if (fd > 0) {
            if(-1 != lseek(fd, 0, SEEK_END)) {
                len = write(fd, writeBuff, strlen(writeBuff));
            }
            close(fd);
        }
        return len;
    }

    /**
    * @description: 获取父目录
    * @param {type}
    * @return:
    */
    int getParentPath(char *path, int num){
        int index = -1;
        int cnt = 0;
        for(int i=strlen(path)-1; i>=0; i--){
            if(path[i] == '/') {
                ++cnt;
                if(cnt == num){
                    index = i;
                    break;
                }
            }
        }
        return index;
    }

};