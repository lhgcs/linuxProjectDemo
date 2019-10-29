/*
 * @Description: sqlite3
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-22 23:26:39
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:46:32
 */

#include "sqlite3.h"
#include <stdio.h>

#define _DEBUG_

/*
    源码：wget http://www.sqlite.org/sqlite-3.6.16.tar.gz
    安装工具：sudo apt-get install sqlitebrowser
    http://www.runoob.com/sqlite/sqlite-drop-table.html
*/

typedef int (*callback)(void*,int,char**,char**);

class SqliteUtil {

public:

    /**
    * @description: 执行SQL命令
    * @param {type} 
    * @return: 
    */ 
    static int sqlExecute(sqlite3 *db, const char *sql, callback fun) {
        char *errMsg = NULL;
        int ret = -1;
        if (NULL == db || NULL == sql) {
            fprintf(stdout, "db null\n");
            return -1;
        }

        fprintf(stdout, sql);
        ret = sqlite3_exec(db, sql, fun, NULL, &errMsg);
        #ifdef _DEBUG_
            if (ret != SQLITE_OK) {
                printf("errMsg = %s \n", errMsg);
                sqlite3_free(errMsg);
            } else {
                fprintf(stdout, "Table created successfully\n");
            }
        #endif
        return ret;
    }

    /**
    * @description: 打开数据库（不存在数据库则创建）  
    * @param {type} 数据库对象
    * @return: 
    */
    static int sqliteOpen(char *dbName, sqlite3 **db) {
        int ret = sqlite3_open(dbName, db);
        if(ret != 0) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));//sqlite3_errmsg(db)用以获得数据库打开错误码的英文描述
            sqlite3_close(*db);
        } else {
            fprintf(stderr, "Opened database successfully\n");
        }
        return ret;
    }

    /**
    * @description: 关闭数据库
    * @param {type} 
    * @return: 
    */
    static void sqliteClose(sqlite3 *db) {
        if(db != NULL) {
            sqlite3_close(db);
            db = NULL;
        }
    }

    /**
    * @description: 创建表
    * @param {type} 
    * @return: 
    */ 
    static int sqliteCreateTable(sqlite3 *db) {
        const char *sql = "CREATE TABLE IF NOT EXISTS COMPANY("  \
                    "ID INT PRIMARY KEY     NOT NULL DEFAULT 0," \
                    "NAME           TEXT    NOT NULL DEFAULT 0," \
                    "AGE            INT     NOT NULL DEFAULT 0," \
                    "ADDRESS        CHAR(50)," \
                    "SALARY         REAL );";
        return sqlExecute(db, sql, NULL);
    }

    /**
    * @description: 插入
    * @param {type} 
    * @return: 
    */ 
    static int insert(sqlite3 *db) {
        const char*sql ="INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) VALUES(1, 'Paul', 32, 'California', 20000.00);";
        return sqlExecute(db, sql, NULL);
    }

    /**
    * @description: 删除某个特定的数据
    * @param {type} 
    * @return: 
    */ 
    static int remove(sqlite3 *db) {
        const char *sql="DELETE from COMPANY where ID=2;";
        return sqlExecute(db, sql, NULL);
    }

    /**
    * @description: 更新
    * @param {type} 
    * @return: 
    */ 
    static int update(sqlite3 *db) {
        const char *sql = "UPDATE COMPANY SET SALARY = 25000.00 , AGE = 33 where ID=1; " \
                "SELECT * from COMPANY";
        return sqlExecute(db, sql, NULL);
    }

    /**
    * @description: 查询数据
    * @param {type} 限制SELECT语句返回的数据数量
    * @return: 
    */
    static int query(sqlite3 *db, char *tableName, unsigned int limit) {
        int ret = -1;

        int i = 0;
        int row = 0;//查出多少行，不包括字段名那行
        int col = 0;//查出多少列

        char **azResult = NULL; //二维数组存放结果
        char *errMsg = NULL;

        char temp[128];

        if(NULL == db || tableName == (char *)"") {
            return ret;
        }

        memset(temp, 0, sizeof(temp));
        sprintf(temp,"SELECT * FROM %s LIMIT %d;",tableName, limit);

        ret = sqlite3_get_table(db, temp, &azResult, &row, &col, &errMsg);

        printf("row=%d col=%d\n",row,col);
        printf("the result is:\n");

        //查找字段
        for(i=0;i<(row+1)*col;i++) {
            printf("azResult[%d]=%s\n",i,azResult[i]);
        }

        sqlite3_free_table(azResult);
        return ret;
    }

    /**
    * @description: 
    * @param {type} 
    * @return: 
    */ 
    int query_order(sqlite3 *db, char *tableName, char *field, int sort) {
        char temp[128];
        memset(temp, 0, sizeof(temp));
        sprintf(temp,"SELECT * FROM %s ORDER BY %s ",tableName,field);
        if (sort) {
            strcat(temp," ASC;");//升序排序
        }else {
            strcat(temp," DESC;");//降序排序
        }
        return sqlExecute(db, temp, NULL);
    }

    /**
    * @description: 增加字段
    * @param {type} 字段类型
    * @return: 
    */ 
    int addField(sqlite3 *db, char *tableName, char *field, char *fieldType) {
        char temp[128];

        if(0 == isExistField(db, tableName, field)) {
            printf("field %s exist\n",field);
            return 0;
        }
        memset(temp, 0, sizeof(temp));
        sprintf(temp,"ALTER TABLE %s ADD %s %s;",tableName,field,fieldType);
        return sqlExecute(db, temp, NULL);
    }

    /**
    * @description: 删除表中的列
    * @param {type} 
    * @return: 
    */ 
    int removeField(sqlite3 *db, char *tableName, char *field) {
        char temp[128];
        memset(temp, 0, sizeof(temp));
        sprintf(temp,"ALTER TABLE %s DROP COLUMN %s;",tableName,field);
        return sqlExecute(db, temp, NULL);
    }

    /**
    * @description: 表重命名
    * @param {type} 
    * @return: 
    */ 
    int renameTable(sqlite3 *db, char *tableName, char *tableNameNew) {
        char temp[128];
        memset(temp, 0, sizeof(temp));
        sprintf(temp,"ALTER TABLE %s RENAME TO %s;",tableName,tableNameNew);
        return sqlExecute(db, temp, NULL);
    }

    /**
    * @description: 判断字段是否存在
    * @param {type} 
    * @return: 
    */ 
    int isExistField(sqlite3 *db, char *tableName, char *field) {
        int ret = -1;

        int i = 0;
        int row = 0;//查出多少行，不包括字段名那行
        int col = 0;//查出多少列

        char **azResult = NULL; //二维数组存放结果
        char *errMsg = NULL;

        char temp[128];
        if(NULL == db || tableName == (char *)"") {
            return ret;
        }

        if(NULL == field || field == (char *)"") {
            return ret;
        }

        memset(temp, 0, sizeof(temp));
        sprintf(temp,"PRAGMA  table_info([%s]);",tableName);

        ret = sqlite3_get_table(db, temp, &azResult, &row, &col, &errMsg);

        printf("row=%d col=%d\n",row,col);
        printf("the result is:\n");

        //查找字段
        for(i=1+col; i<(row+1)*col; i+=col) {
            printf("azResult[%d]=%s\n",i,azResult[i]);
            if(NULL == azResult[i] || azResult[i] == (char *)"") {
                continue;
            }
            if(0 == strcmp(azResult[i],field)) {
                printf("find field:%s\n",field);
                ret = 0;
                break;
            }
        }

        sqlite3_free_table(azResult);
        return ret;
    }

    /**
    * @description: 获取记录数量
    * @param {type} 
    * @return: 
    */ 
    int getRecordNum(sqlite3 *db, char *tableName) {
        int ret = -1;
        int i = 0;
        int row = 0;//查出多少行，不包括字段名那行
        int col = 0;//查出多少列

        char **azResult = NULL; //二维数组存放结果
        char *errMsg = NULL;

        char temp[128];

        if(NULL == db || tableName == (char *)"") {
            return ret;
        }

        memset(temp, 0, sizeof(temp));
        sprintf(temp,"SELECT COUNT(*) FROM %s;",tableName);

        ret = sqlite3_get_table(db, temp, &azResult, &row, &col, &errMsg);

        printf("row=%d col=%d\n",row,col);
        printf("the result is:\n");

        //查找字段
        for(i=0;i<(row+1)*col;i++) {
            printf("azResult[%d]=%s\n",i,azResult[i]);
        }
        if(azResult[1] != NULL && azResult[1] != (char *)"") {
            ret = atoi(azResult[1]);
            printf("num:%d\n",ret);
        }

        sqlite3_free_table(azResult);
        return ret;
    }

};
