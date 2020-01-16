/*
 * @Description: 获取命令行参数
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2020-01-06 17:06:24
 * @LastEditors  : lhgcs
 * @LastEditTime : 2020-01-06 17:36:34
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

/*
	命令行参数
	0 可带可不带
	1 必须带参数
	2 不带参数   
*/
static struct option opts[] = {
	{"aa", 2, 0, 'a'},
	{"bb", 1, 0, 'b'},
	{"help", 0, 0, 'h'},
	{0, 0, 0, 0}
};


/**
 * @description: 获取命令行参数
 * @param {type} 
 * @return: 
 */
int getArg() {
	int argc = 2;
	char *argv[10] = {
		"-a 1",
		"--bb=\"2\""
	};
	char c;
	while ((c = getopt_long(argc, argv, "c::d:f:hi:ln:s:Srae", opts, NULL)) != -1) {
		//optarg 选项参数指针，optind 下一个选项指针
		printf("optind:%d  optopt:%d\n", optind, optopt);

		switch (c) {
			case 'a':
				printf("%d\n", atoi(optarg));
				break;
			case 'b'://格式
				if (0 == strcmp(optarg, "2")) {

				}
				break;
			case 'h'://帮助
				// usage(argv[0]);
				return 0;
			default:break;
		}

	}
	if (optind >= argc) {
		// usage(argv[0]);
		return 1;
	}
}
