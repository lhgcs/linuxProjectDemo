/*
 * @Description: cjson生成和解析
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:43:14
 */

class jsonUtil {

public:

  /**
 * @brief cjson_parse_string
 * @param str json字符串
 * @param key 键
 * @return 值(字符串）
 * 解析JSON包
 */

    /**
     * @description: json字符串解析成json对象
     * @param {type} 
     * @return: 
     */
    int jsonStr2JsonObject(const char *str, cJSON *root) {
        int res = -1;
        cJSON *root= cJSON_Parse(str);                  //字符串解析成json结构体
        if(NULL == root) {
            printf("json error\n");
        } else {
            res = 0;
            printf("json: %s\n",cJSON_Print(root));       //json结构体转换成字符串;
        }
        return res;
    }

   /**
     * @description: json字符串解析成json对象
     * @param {type} 
     * @return: 
     */
    int getJsonObject(const char *str, cJSON *root) {
        int res = -1;
        cJSON *root= cJSON_Parse(str); // 字符串解析成json结构体
        if(NULL == root) {
            printf("json error\n");
        } else {
            res = 0;
            printf("json: %s\n",cJSON_Print(root)); // json结构体转换成字符串;
        }
        return res;
    }

   /**
     * @description: 获取键值
     * @param {type} 
     * @return: 
     */
    int getJsonValue(cJSON *root, const char *key, cJSON *temp) {
        int res = -1;
        if (NULL == root || NULL == key) {
            return -1;
        }
        cJSON *temp = cJSON_GetObjectItem(root, key);
        if(NULL == temp) {
            printf("json not key:%s",key);
        }
        res = (NULL == temp) ? -1 : 0;
        return res;
    }

    /**
     * @description: 获取键值String
     * @param {type} 
     * @return: 
     */
    int getJsonValueStr(cJSON *root, const char *key, char *value) {
        int res = -1;
        if (NULL == root || NULL == key) {
            return -1;
        }
        cJSON *temp;
        if(0 == getJsonValue(root, key, temp)) {
            if(temp->type == cJSON_String) {
                // 获取某个元素
                value = temp->valuestring;
                res = 0;
            }
        }
        return res;
    }

    /**
     * @description: 获取键值Int
     * @param {type} 
     * @return: 
     */
    int getJsonValueInt(cJSON *root, const char *key, char *value) {
        int res = -1;
        if (NULL == root || NULL == key) {
            return -1;
        }
        cJSON *temp;
        if(0 == getJsonValue(root, key, temp)) {
            if(temp->type == cJSON_Number) {
                // 获取某个元素
                value = temp->valueint;
                res = 0;
            }
        }
        return res;
    }

   /**
     * @description: json数组大小
     * @param {type} 
     * @return: 
     */
    int getJsonArraySize(cJSON *arrayObject) {
        return cJSON_GetArraySize(arrayObject);
    }

    /**
     * @description: json数组元素
     * @param {type} 
     * @return: 
     */
    int getJsonArrayItem(cJSON *arrayObject, int index) {
        cJSON* temp = cJSON_GetArrayItem(arrayObject, index);
        if (NULL == temp){
            
        }else{
            res = 0;
        }
        return res;
    }

    /**
     * @description: 释放json对象
     * @param {type} 
     * @return: 
     */
    void freeJson(cJSON *root) {
        if (NULL != root) {
            cJSON_Delete(root);
        }
    }

    /**
    * @description: 生成json字符串
    * @param {type} 
    * @return: 
    */
    int setJson(const char *key, const int value, char *str) {
        
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, key, value);
        char *s = cJSON_Print(root);
        // 在运行cJSON_Print(root);后务必要要进行释放操作
        cJSON_Delete(root);

        char *t = s;
        while( *s != '\0' ){
            // 如果当前字符不是要删除的，则保存到目标串中
            if ( *s != '\n' && *s != '\t' ){
                *t++ = *s;
            }
            s++ ;
        }
        *t='\0';
        strcpy(str, t);
        return 0;
    }

};
