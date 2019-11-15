/*
 * @Description: 播放音频
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-11-08 11:30:24
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-11-11 14:48:47
 */

#ifndef __UDPLAYSIGNALVALUE_H__
#define __UDPLAYSIGNALVALUE_H__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

#define UD_PLAY_DEBUG 1

/**
 * @description: 执行命令
 * @param {type} 
 * @return: 
 */
int system_shell(const char *cmdString) {
    int ret = -1;
    if(NULL == cmdString) {
        return -1;
    }
    printf("%s\n", cmdString);
    ret = system(cmdString);

    return ret == 0 ? 0 : -1;
}

/**
 * @description: 执行命令
 * @param {type} 
 * @return: 
 */
int execl_shell(const char *cmdString) {

	pid_t pid;
	int status;

	if ((pid = fork()) < 0) {
		status = -1;
	} else if (pid == 0) {  // 子进程

        printf("%s\n", cmdString);
        // 正确执行无返回
        execl("/bin/sh", "sh", "-c", cmdString, (char *)0);
        perror(cmdString);   
		_exit(127);

	} else {   // 父进程
		while (waitpid(pid, &status, 0) < 0) {
			if (errno != EINTR) {
				status = -1;
				break;
			}
		}
	}
	return status;
 }

/**
 * @description: 顺序执行多条语句
 * @param {type} 
 * @return: 
 */
int popen_shell_sync(char cmdString[][64], unsigned int len) {
    int status = -1;

    if(cmdString == NULL || len <= 0) {
        return status;
    }

	pid_t pid;
	
	if ((pid = fork()) < 0) {
		return status;
	} else if (pid == 0) {  

        FILE *fp = NULL;
		char data[10];
		
        for(int i=0; i<len; i++) {
            fp = popen(cmdString[i], "r");
            if(NULL != fp) {
                while(fgets(data, sizeof(data), fp) != NULL) {
                    printf("%s", data);
                }
                pclose(fp);
            }
        }

        status = 0;
	}
	
	return status;
 }

/**
 * @description: 命令播放音频文件
 * @param {type} 
 * @return: 
 */
int command_play(char fileName[][64], unsigned int len) {
    int status = -1;
    int cnt = 0;

    if(fileName == NULL || len <= 0) {
        return status;
    }

    char cmdString[512];
    memset(cmdString, 0, sizeof(cmdString));

    // 播放器是否存在
    if(0 == access("/usr/bin/sox", X_OK)) {            // sox
        strcpy(cmdString, "/usr/bin/sox");
    } else if(0 == access("/usr/bin/play", X_OK)) {    // sox
        strcpy(cmdString, "/usr/bin/play");
    } else if(0 == access("/usr/bin/aplay", X_OK)) {   // alsa
        strcpy(cmdString, "/usr/bin/aplay");
    } else if(0 == access("/usr/bin/mplayer", X_OK)) { // mplayer
        strcpy(cmdString, "/usr/bin/mplayer");
    } else {
        printf("music player is not exist\n");
        return status;
    }

    strcat(cmdString, " ");
    for(int i=0; i<len; i++) {
        // 音频文件存在
        if(0 == access(fileName[i], F_OK)) {
            strcat(cmdString, fileName[i]);
            strcat(cmdString, " ");
            cnt++;
        }
    }

    // 音频文件都不存在
    if(0 == cnt) {
        return status;
    }

    if(0 == access("/usr/bin/sox", X_OK)) {
        strcat(cmdString, "-t alsa hw:0,0,0");
    } else if(0 == access("/usr/bin/play", X_OK)) {

    } else if(0 == access("/usr/bin/aplay", X_OK)) {
        strcat(cmdString, "-D plughw:0");
    } else if(0 == access("/usr/bin/mplayer", X_OK)) {
        
    } else {
        printf("music player is not exist\n");
        return status;
    }
    strcat(cmdString, " > /dev/null");

    #if UD_PLAY_DEBUG
        system_shell(cmdString);
    #endif

    return 0;
}

/**
 * @description: 数字分解
 * @param {type} 
 * @return: 
 */
static int numberFliter(unsigned int num, char *buff) {
    int i = 0;
    if (0 == num) {
        buff[i] = 0;
        return 1;
    }
    
    while (num > 0) {
        buff[i++] = num % 10;
        num /= 10;
    }

    // 倒序
    for(int j=0; j<i/2; j++) {
        char temp = buff[j];
        buff[j] = buff[i-j-1];
        buff[i-j-1] = temp;
    }
    return i;
}

/**
 * @description: 播放列表
 * @param {type} 
 * @return: 
 */
static int play_list(const char *soundDir, const char *prefix, char fileName[][64], unsigned int len, unsigned int num, char playList[][64]) {
    
    if (NULL == soundDir || NULL == prefix || NULL == fileName || len <= 0) {
        return -1;
    }
    
    char buff[20];
    memset(buff, 0, sizeof(buff));
    int buffSize = numberFliter(num, buff);

    char temp[64];
    int index = 0;

    for(int i=0; i<len; i++) {
        memset(temp, 0, sizeof(temp));
        sprintf(temp, "%s/%s%s.wav", soundDir, prefix, fileName[i]);
        strcpy(playList[index++], temp);
    }
    
    for(int i=0; i<buffSize; i++) {
        memset(temp, 0, sizeof(temp));
        sprintf(temp, "%s/%s%d.wav", soundDir, prefix, buff[i]);
        strcpy(playList[index++], temp);
    }
    return index;
}

/**
 * @description: 播放心率值
 * @param {type} 
 * @return: 
 */
void play_heart(unsigned int num) {
    char playList[20][64];
    char soundDir[20] = "./sound";
    char prefix[10] = "test-";
    char fileName[1][64] = {"heart"};
    int len = play_list(soundDir, prefix, fileName, 1, num, playList);
    command_play(playList, len);
}

/**
 * @description: 播放呼吸值
 * @param {type} 
 * @return: 
 */
void play_breath(unsigned int num) {
    char playList[20][64];
    char soundDir[20] = "./sound";
    char prefix[10] = "test-";
    char fileName[1][64] = {"breath"};
    int len = play_list(soundDir, prefix, fileName, 1, num, playList);
    command_play(playList, len);
}

#endif