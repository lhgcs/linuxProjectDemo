/*
 * @Description: 数据表接口类
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-24 16:36:28
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:18:20
 */

#include "Dao.h"

class TableHelper {

public:

    /**
     * @description: 增
     * @param {type} 
     * @return: 
     */
    virtual int insert(Dao *dao) = 0;

    /**
     * @description: 删
     * @param {type} 
     * @return: 
     */
    virtual int remove(Dao *dao) = 0;

    /**
     * @description: 改
     * @param {type} 
     * @return: 
     */
    virtual int update(Dao *dao) = 0;

    /**
     * @description: 查（所有）
     * @param {type} 
     * @return: 
     */
    virtual int select() = 0;

    /**
     * @description: 查（ID）
     * @param {type} 
     * @return: 
     */
    virtual int selectById() = 0;
};