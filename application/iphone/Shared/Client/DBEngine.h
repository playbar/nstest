//
//  DBEngine.h
//  testsplite
//
//  Created by yock on 12-4-13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "sqlite3.h"     //libsqlite3

@interface DBEngine : NSObject
{
    sqlite3         *m_hDb;                 //数据库拒柄
    sqlite3_stmt    *m_hStatement;          //视表拒柄
}

//打开或创建数据库
- (bool) OpenOrCreate:(NSString*) dbpath;

//加入条目
- (bool) Add:(NSString*) data;

//逐条获得查询值
- (bool) Get:(int*) index
         UseCount:(int*) count
         LastTime:(NSDate**) date
         Data:(NSString**) data;

//删除条目
- (bool) Remove:(int) itemid;

//查找内容,使用like匹配
- (bool) Select:(NSString*) data 
         UseCount:(int) count
         LastTime:(NSDate*) date;

//得到表中总条目数
- (int) GetCount;

//查找内容是否有，朝找是否有固定内容使用＝匹配
- (bool) Find:(NSString*) data;

//关闭数据库
- (void) Close;

@end
