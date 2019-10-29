/*
 * @Description: 宏定义实现日志输出
 * @Author: your name
 * @Date: 2019-10-28 23:29:34
 * @LastEditTime: 2019-10-29 17:43:27
 * @LastEditors: lhgcs
 */

#define DBG_EN 	1
#if (1 == DBG_EN)
#define printf_dbg(x,arg...) printf("[gpio_debug]" x,##arg)
#else
#define printf_dbg(x,arg...)
#endif

#define printf_info(x,arg...) printf("[gpio_info]" x,##arg)
#define printf_warn(x,arg...) printf("[gpio_warnning]" x,##arg)
#define printf_err(x,arg...)  printf("[gpio_error]" x,##arg)

#define LOGD printf
#define LOGI printf
#define LOGE printf
