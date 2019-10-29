/*
 * @Description: 目录
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:40:29
 */

#include <dirent.h>

class DirUtil {

public:

    /**
    * @brief isExist 目录是否存在
    * @param dirPath 目录
    * @return        
    */
    static int isExist(const char *dirPath) {
        DIR *dir = NULL;
        if(NULL == dirPath) {
            return -1;
        }
        // 能否打开目录
        if(NULL != (dir= opendir(dirPath))) {
            closedir(dir);
            return 0;
        }
        return -1;
    }

    /**
    * @brief exist   目录是否存在
    * @param dirPath 目录
    * @return        
    */
    static int exist(const char *dirPath) {
        struct stat buff;
        // 获取信息
        if (0 == stat(dirPath, &buff)) {
            // 是否是文件夹
            if ((buff.st_mode & S_IFDIR) != 0) {
                    return 0;
            }
        }
        return -1;
    }

    /**
    * @brief create  创建目录
    * @param dirPath 目录
    * @return        
    */
    static int create(const char *dirPath) {
        if(0 != isExist(dirPath)) {
            if(0 != mkdir(dirPath, 0777)) {
                return -1;
            }
        }
        return 0;
    }

    /**
    * @brief rmmove  删除目录(缺陷：只能删除空目录)
    * @param dirPath 目录
    * @return        0：成功
    */
    static int remove(const char *dirPath) {
        if(0 != isExist(dirPath)) {
            //删除目录
            if(0 != rmdir(dirPath)) {
                return -1;
            }
        }
        return 0;
    }

    /**
     * @description: 创建多级目录（绝对路径）
     * @param {type} 目录
     * @return: 
     */
    static int createDeep(const char *dirPath) {

        if(NULL == dirPath || ('/' != dirPath[0])) {
            return -1;
        }

        char path[64];
        int lastIndex = strlen(dirPath) - 1;
        // 是否以/结尾
        lastIndex = dirPath[lastIndex] == '/' ? lastIndex -1 : lastIndex;

        for(int i=1; i<=lastIndex; i++) {
            // 最后一个目录
            if(i == lastIndex) {
                if(0!= create(dirPath)) {
                    return -1;
                }
            } else if('/' == dirPath[i]) {
                memset(path, 0, sizeof(path));
                strncpy(path, dirPath, i);
                if(0 != create(path)) {
                    return -1;
                }
            }
        }
        return 0;
    }

    /**
    * @description: 递归删除目录（如果是根目录/不处理）
    * @param {type} 目录
    * @return: 
    */
    static int removeAll(const char *dirPath) {
        DIR *dir;
        dirent *dirInfo;
        char filePath[1024];

        //文件类型
        struct stat buff;
        if(stat(dirPath, &buff) != 0) {
            return -1;
        }

        // 是文件夹，并且不是根目录
        if(1 == S_ISDIR(buff.st_mode) && (0 != strcmp(dirPath, "/"))) {
            // 打开目录
            if((dir = opendir(dirPath)) != NULL) {
                // 读取
                while((dirInfo = readdir(dir)) != NULL) {
                    // 跳过. ..文件
                    if((0 == strcmp(dirInfo->d_name, ".")) || (0 == strcmp(dirInfo->d_name, ".."))) {
                        continue;
                    }

                    // 绝对路径
                    memset(filePath, 0, sizeof(filePath));
                    strcpy(filePath, dirPath);
                    if(filePath[strlen(dirPath) - 1] != '/') {
                        strcat(filePath, "/");
                    }
                    strcat(filePath, dirInfo->d_name);

                    // 文件类型
                    if(stat(filePath, &buff) != 0) {
                        continue;
                    }

                    if(1 == S_ISREG(buff.st_mode) || 1 == S_ISLNK(buff.st_mode)) { // 常规文件或连接
                        remove(filePath);
                    }else if(1 == S_ISDIR(buff.st_mode)) {                         // 是文件夹，并且不是根目录
                        removeAll(filePath);                                       // 递归
                    }
                }

                closedir(dir);
                // 删除空目录
                rmdir(dirPath);
            }
        }
        return 0;
    }

};