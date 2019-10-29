/*
 * @Description: jsoncpp
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-22 00:18:59
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:48:03
 */

#include "jsoncpp/json/json.h"
#include "jsoncpp/json/reader.h"
#include "jsoncpp/json/value.h"
#include <iostream>

class JsoncppUtil {

public:

    /**
    * @description: 解析json字符串
    * @param {type} 
    * @return: 
    */ 
    static int parseJsonStr(const char *jsonStr) {

        double temp = 0;
        Json::Reader reader;
        Json::Value value;

        printf("jsonStr: %s\n", jsonStr);
        
        if (true == reader.parse(jsonStr, value)) {
            // 包含字段
            if (value.isMember("time") && value.isMember("people")) {
                unsigned long time = value["time"].asUInt64();
                
                // 解析对象数组对象
                const Json::Value arrayObj = value["people"];
                for (int i = 0; i < arrayObj.size(); i++) {
                    // 识别当前状态
                    unsigned int post = arrayObj[i]["pose"].asUInt();
                    // 指标统计
                    temp = arrayObj[i]["heartRate"].asDouble();
                    temp = arrayObj[i]["breathRate"].asDouble();
                }

                return 0;
            }
        }
        return -1;
    }

    /**
    * @description: 生成json字符串
    * @param {type} 
    * @return: 
    */ 
    static std::string generateJsonStr() {

        Json::Value root;
        root["num"] = 1;

        return root.toStyledString();
    }

};
