/*
 * @Description: 字符串
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:44:43
 */

#include <string.h>

class StrUtil {

public:

    /**
     * @description: 字符串连接（剔除不可见字符）
     * @param {type} 
     * @return: 
     */
    static unsigned int strAppend(char *dst, const char *src) {
        unsigned int sum = 0;
        while(*dst != '\0') {
            dst++;
        }
        while(*src != '\0') {
            if((*src == '\r') || (*src == '\n') || (*src == ' ')) {
                src++;
            }else{
                *dst++ = *src++;
                sum ++;
            }
        }
        // 结尾添加回车换行
        *dst++ = '\r';
        *dst = '\n';
        *(dst+1) = '\0';
        sum +=2;
        return sum;
    }

    /**
     * @description: 字符串转long型数据
     * @param {type} 
     * @return: 
     */
    long str2long(char *str) {
        long value = 0;
        char *p=str;
        if(NULL == p) {
            return 0;
        }
        // 跳过开头的非数据字符
        while (*p != '\0') {
            if((*p >= '0') && (*p <= '9')) {
                break;
            } else {
                p++;
            }
        }
        // 遇到\0或非数字字符结束
        while (*p != '\0') {
            if((*p >= '0') && (*p <= '9')) {
                value = value * 10 + (*p - '0');
                p++;
            } else if(*p == ' ' || *p == '\r' || *p == '\n' || *p == '.') {
                break;
            }else {
                return 0;
            }
        }
        printf("value:%ld\n",value);
        return value;
    }

    /**
    * @description: 查找字符串，返回下标
    * @param {type} 
    * @return: 
    */ 
    int findFirstSubstring(const char *dst,const char *sub) {
        unsigned int pLen = 0,qLen=0;
        const char *p=dst;
        const char *q=sub;
        if(NULL == dst || NULL == sub) {
            return -1;
        }

        while (p != NULL) {
            if(*p == *q) {
                q++;
                qLen++;
                if(qLen == strlen(sub)) {
                    break;
                }
            } else {
                q=sub;
                qLen=0;
            }
            p++;
            pLen++;
        }
        return pLen;
    }

    /**
    * @description: 从后往前查找字符，返回下标
    * @param {type} 
    * @return: 
    */ 
    int findLastChar(char *dst,char sub) {
        int index = -1;
        int len = strlen(dst);
        char *p=dst;
        for(int i=len-1; i>=0; i--) {
            if(*(p+i) == sub) {
                index = i;
                break;
            }
        }
        return index;
    }

    /**
    * @description: 提取字符0-9 a-z A-Z
    * @param {type} 
    * @return: 
    */ 
    int filterCharAndNum(char *dst) {
        int i=0,j=0;
        int len=strlen(dst);
        for(;i<len;i++) {
            if((dst[i] >= '0' && dst[i] <= '9') || (dst[i] >= 'A' && dst[i] <= 'Z') || (dst[i] >= 'a' && dst[i] <= 'z')) {
                dst[j++]=dst[i];
            }
        }
        dst[j]='\0';
        return j;
    }

};