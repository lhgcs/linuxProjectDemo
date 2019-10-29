/*
 * @Description: 实体
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-24 16:31:56
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:17:29
 */

#include "../helper/Dao.h"
#include "iostream"

class MyDao : public Dao{

public:
    long id;
    long createTime;
    long updateTime;
    std::string name;
    int age;
};