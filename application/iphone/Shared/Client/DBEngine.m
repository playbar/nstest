//
//  DBEngine.m
//  testsplite
//
//  Created by yock on 12-4-13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "DBEngine.h"

//TableName "chat"
//chat表包含字段 data usecount lasttime weight
//weight为权重，lasttime*100+usecount;
//lasttime,uscount降序排列建立索引 id
#define ZRecordCount 100

@implementation DBEngine

- (id) init
{
    self = [super init];
    if(self)
    {
        m_hDb = NULL;
        m_hStatement = NULL;
    }
    return self;
}

- (void) dealloc
{
    [self Close];
    [super dealloc];
}

//打开或创建数据库
- (bool) OpenOrCreate:(NSString*) dbpath
{
    if(m_hDb != NULL)
        [self Close];
    if(sqlite3_open([dbpath UTF8String], &m_hDb)==SQLITE_OK)
        BWLOG(@"open db is ok\n");
    else
    {
        BWLOG(@"open db have an error");
        return false;
    }
    char* errorMsg=NULL;
    const char* createTable = "CREATE TABLE IF NOT EXISTS chat (data TEXT, usecount INTEGER, lasttime INTEGER, weight INTEGER)";
    const char* createIndex = "CREATE UNIQUE INDEX id ON chat (data DESC)"; //小规模数据，插入速度更优先
    if(sqlite3_exec(m_hDb, createTable, NULL, NULL, &errorMsg) == SQLITE_OK)
    {
        sqlite3_exec(m_hDb, createIndex, NULL, NULL, &errorMsg);
        BWLOG(@"create ok");
    }
    if(errorMsg!=NULL)
    {
        BWLOG(@"err = %s",errorMsg);
        free(errorMsg);
    }
    return true;
    
}
//查找内容
- (bool) Select:(NSString*) data 
       UseCount:(int) count
       LastTime:(NSDate*) date
{
    unsigned char type = 0;
    int dotime = 0;
    if(data) type |= 0x04;
    if(count>0) type |= 0x02;
    if(date)
    {
        type |= 0x01;
        NSCalendar* cal = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar]; 
        NSInteger unitflegs = NSDayCalendarUnit | NSMonthCalendarUnit | NSYearCalendarUnit;
        NSDateComponents* comp;
        comp = [cal components:unitflegs fromDate:date];
        dotime = [comp year]*10000;
        dotime += [comp month]*100;
        dotime += [comp day];
        [comp release];
        [cal release];
    }
    if(type == 0) return false;
    if(m_hStatement)
        sqlite3_finalize(m_hStatement);
    m_hStatement = NULL;
    int err;
    char* errmsg=NULL;
    switch(type)
    {
        case 0x04:      //data
        {
            /*
            const char* src = "SELECT weight,lasttime,usecount,data FROM chat WHERE data LIKE ? ORDER BY weight DESC";
//            const char* src = "select weight,lasttime,usecount,data from chat";
            err=sqlite3_prepare_v2(m_hDb, src,-1, &m_hStatement, &errmsg);
            err=sqlite3_bind_text(m_hStatement, 1, [data UTF8String], [data length], SQLITE_TRANSIENT);
             */
            NSString *str = [NSString stringWithFormat:@"SELECT weight,lasttime,usecount,data FROM chat WHERE data LIKE '%@' ORDER BY weight DESC",data]; 
            err=sqlite3_prepare_v2(m_hDb, [str UTF8String], -1, &m_hStatement, NULL);
        }
            break;
        case 0x02:      //count
        {
            /*
            const char* src = "SELECT weight,lasttime,usecount,data FROM chat WHERE usecount=? ORDER BY weight DESC";
            err=sqlite3_prepare_v2(m_hDb, src, -1, &m_hStatement, &errmsg);
            err=sqlite3_bind_int(m_hStatement, 1, count);
             */
            NSString *str = [NSString stringWithFormat:@"SELECT weight,lasttime,usecount,data FROM chat WHERE usecount=%d ORDER BY weight DESC",count];
            err = sqlite3_prepare_v2(m_hDb, [str UTF8String], -1, &m_hStatement, NULL); 
        }
            break;
        case 0x01:      //date
        {
            /*
            const char* src = "SELECT weight,lasttime,usecount,data FROM chat WHERE lasttime=? ORDER BY weight DESC";
            err=sqlite3_prepare_v2(m_hDb, src, -1, &m_hStatement, &errmsg);
            err=sqlite3_bind_int(m_hStatement, 1, dotime);
             */
            NSString *str = [NSString stringWithFormat:@"SELECT weight,lasttime,usecount,data FROM chat WHERE lasttime=%d ORDER BY weight DESC",dotime];
            err = sqlite3_prepare_v2(m_hDb, [str UTF8String], -1, &m_hStatement, NULL);
        }
            break;
        case 0x06:      //data && count
        {
            /*
            const char* src = "SELECT weight,lasttime,usecount,data FROM chat WHERE data LIKE ? AND usecount=? ORDER BY weight DESC";
            sqlite3_prepare_v2(m_hDb, src, -1, &m_hStatement, &errmsg);
            sqlite3_bind_text(m_hStatement, 1, [data UTF8String], -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(m_hStatement, 2, count);
             */
            NSString *str = [NSString stringWithFormat:@"SELECT weight,lasttime,usecount,data FROM chat WHERE data LIKE '%@' AND usecount=%d ORDER BY weight DESC",data,count];
            err = sqlite3_prepare_v2(m_hDb, [str UTF8String], -1, &m_hStatement, NULL);
        }
            break;
        case 0x05:      //data && date
        {
            /*
            const char* src = "SELECT weight,lasttime,usecount,data FROM chat WHERE data LIKE ? AND lasttime=? ORDER BY weight DESC";
            err=sqlite3_prepare_v2(m_hDb, src, -1, &m_hStatement, &errmsg);
            err=sqlite3_bind_text(m_hStatement, 1, [data UTF8String], -1, SQLITE_TRANSIENT);
            err=sqlite3_bind_int(m_hStatement, 2, dotime);
             */
            NSString *str = [NSString stringWithFormat:@"SELECT weight,lasttime,usecount,data FROM chat WHERE data LIKE '%@' AND lasttime=%d ORDER BY weight DESC",data,dotime];
            err = sqlite3_prepare_v2(m_hDb, [str UTF8String], -1, &m_hStatement, NULL);
        }
            break;
        case 0x03:      //count && date
        {
            /*
            const char* src = "SELECT weight,lasttime,usecount,data FROM chat WHERE usecount=? AND lasttime=?";
            err=sqlite3_prepare_v2(m_hDb, src, -1, &m_hStatement, &errmsg);
            err=sqlite3_bind_int(m_hStatement, 1, count);
            err=sqlite3_bind_int(m_hStatement, 2, dotime);
             */
            NSString *str = [NSString stringWithFormat:@"SELECT weight,lasttime,usecount,data FROM chat WHERE usecount=%d AND lasttime=%d",count,dotime];
            err = sqlite3_prepare_v2(m_hDb, [str UTF8String], -1, &m_hStatement, NULL);
        }
            break;
        case 0x07:      //data && count && date
        {
            /*
            const char* src = "SELECT weight,lasttime,usecount,data FROM chat WHERE data LIKE ? AND usecount=? AND lasttime=?";
            err=sqlite3_prepare_v2(m_hDb, src, -1, &m_hStatement, &errmsg);
            err=sqlite3_bind_text(m_hStatement, 1, [data UTF8String], -1, SQLITE_TRANSIENT);
            err=sqlite3_bind_int(m_hStatement, 2, count);
            err=sqlite3_bind_int(m_hStatement, 3, dotime);
             */
            NSString *str = [NSString stringWithFormat:@"SELECT weight,lasttime,usecount,data FROM chat WHERE data LIKE '%@' AND usecount=%d AND lasttime=%d",data,count,dotime];
            err = sqlite3_prepare_v2(m_hDb, [str UTF8String], -1, &m_hStatement, NULL);
        }
            break;
    }
    if(errmsg!=NULL)
    {
        BWLOG(@"err=%s",errmsg);
        free(errmsg);
    }
    if(err==0)
        return true;
    else
        return false;
}

//加入条目
- (bool) Add:(NSString*) data
{

    if(data==NULL) return false;
    if(m_hDb == NULL) return false;
    
    //获取当前时间
    NSDate *date = [[NSDate alloc] init];
    [date dateByAddingTimeInterval:-5*3600*24];
    //调制时间
    int dotime;
    NSCalendar* cal = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar]; 
    NSInteger unitflegs = NSDayCalendarUnit | NSMonthCalendarUnit | NSYearCalendarUnit;
    NSDateComponents* comp;
    comp = [cal components:unitflegs fromDate:date];
    dotime = [comp year]*10000;
    dotime += [comp month]*100;
    dotime += [comp day];
    [cal release];
    [date release];

    
    /*
     char **result;
     int row,column;
     char *err;
     sqlite3_get_table(m_hDb, "SELECT COUNT(weight) FROM chat", &result, &row, &column, &err);
     */

    /*
    NSString *strslc = [NSString stringWithFormat:@"SELECT weight,lasttime,usecount FROM chat WHERE data LIKE '%s'",[data UTF8String]];
    sqlite3_stmt *tempStmt;
    sqlite3_prepare_v2(m_hDb, [strslc UTF8String], -1, &tempStmt, NULL);
    if(sqlite3_step(tempStmt)==SQLITE_ROW)
    {
        int index,count,lasttime;
        index = sqlite3_column_int(tempStmt, 0);
        count = sqlite3_column_int(tempStmt, 2);
        lasttime = sqlite3_column_int(tempStmt, 1);
        sqlite3_finalize(tempStmt);
    }*/
    
    //判断是否有重复数据
    if([self Find:data])
    {
        NSString *strup = [NSString stringWithFormat:@"UPDATE chat SET usecount=usecount+1,lasttime=%d,weight=%d*100+usecount+1 WHERE data='%@'",dotime,dotime,data];
        char *test = NULL;
        int err = sqlite3_exec(m_hDb, [strup UTF8String], NULL, NULL, &test);
        if(err == SQLITE_OK)
        {
        return true;
        }
        else
        {
            if(test)
            {
                BWLOG(@"%s",test);
                free(test);
            }
            return false;
        }
    }
    
    //判断数据是否太多
    if([self GetCount] >= ZRecordCount)
    {

        sqlite3_stmt *tempStmt;
        sqlite3_prepare_v2(m_hDb, "SELECT TOP 1 weight FROM chat ORDER BY weight ASC", -1, &tempStmt, nil);
        sqlite3_step(tempStmt);
        int weight;
        weight = sqlite3_column_int(tempStmt, 0);
        [self Remove:weight];
        sqlite3_finalize(tempStmt);
    }
    //插入数据
    NSString *str = [NSString stringWithFormat:@"INSERT INTO chat(data,usecount,lasttime,weight) VALUES('%@',%d,%d,%d)",data,1,dotime,dotime*100+1];
    char* errMsg=NULL;
    bool b;
    b = sqlite3_exec(m_hDb, [str UTF8String], NULL, NULL, &errMsg)==SQLITE_OK ;
    if(errMsg)
    {
        BWLOG(@"add have error %s",errMsg);
        free(errMsg);
    }
    /*
    const char* str = "INSERT INTO chat(data,usecount,lasttime,weight) VALUES(?,?,?,?)";
    sqlite3_stmt* tempStmt;
    int err = sqlite3_prepare_v2(m_hDb, str, -1, &tempStmt, NULL);
    sqlite3_bind_text(tempStmt, 0, [data UTF8String], -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(tempStmt, 1, count);
    sqlite3_bind_int(tempStmt, 2, dotime);
    sqlite3_bind_int(tempStmt, 3, dotime*100+count);
    err = sqlite3_step(tempStmt);
    sqlite3_finalize(tempStmt);
     */
    return b;
}

/*
//改变指定条目中的项
- (bool) Change:(int) itemid
         UseCount:(int) count
         LastTime:(NSDate*) date
{
    if(date==NULL) return false;
    
    int dotime = 0;
    NSCalendar* cal = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar]; 
    NSInteger unitflegs = NSDayCalendarUnit | NSMonthCalendarUnit | NSYearCalendarUnit;
    NSDateComponents* comp;
    comp = [cal components:unitflegs fromDate:date];
    dotime = [comp year]*10000;
    dotime += [comp month]*100;
    dotime += [comp day];
    [comp release];
    [cal release];
    
    const char* str = "UPDATE chat SET usecount=? AND lasttime=? WHERE weight=?";
    sqlite3_stmt* tempStmt;
    sqlite3_prepare_v2(m_hDb, str, -1, &tempStmt, NULL);
    sqlite3_bind_int(tempStmt, 0, count);
    sqlite3_bind_int(tempStmt, 1, dotime);
    sqlite3_bind_int(tempStmt, 2, itemid);
    if(sqlite3_step(tempStmt)==SQLITE_ROW)
    {
        sqlite3_finalize(tempStmt);
        return true;
    }
    sqlite3_finalize(tempStmt);
    return false;
}
 */

//获得查询值
- (bool) Get:(int*) index
         UseCount:(int*) count
         LastTime:(NSDate**) date       //不需要release
         Data:(NSString**) data         //需要release
{
    if(m_hDb==NULL) return false;
    if(m_hStatement == NULL) return false;
    int err = sqlite3_step(m_hStatement);
    if(err == SQLITE_ROW)
    {
        if(index!=NULL)
            *index = sqlite3_column_int(m_hStatement, 0);       //get weight
        if(count!=NULL)
            *count = sqlite3_column_int(m_hStatement, 2);       //get usecount
        if(date!=NULL)
        {
            int time = sqlite3_column_int(m_hStatement, 1);
            NSString* timeS = [NSString stringWithFormat:@"%d/%d/%d",time/10000,time%10000/100,time%100];
            NSDateFormatter* format = [[NSDateFormatter alloc] init];
            [format setDateFormat:@"yyyy/MM/dd"];
            *date = [format dateFromString:timeS];
            [format release];
        }
//        *data = [[NSString alloc] initWithCString:(char*)sqlite3_column_text(m_hStatement, 3) encoding:NSUTF8StringEncoding];
//        NSString *str = [[NSString alloc]initWithUTF8String:(char*)sqlite3_column_text(m_hStatement, 3)];
        if(data!=NULL)
        {
            NSString *str = [[NSString alloc] initWithCString:(const char*)sqlite3_column_text(m_hStatement, 3) encoding:NSUTF8StringEncoding];
            *data = str;
        }
        return true;
    }
    else
    {
        sqlite3_finalize(m_hStatement);
        m_hStatement = NULL;
        return false;
    }
}

//删除条目
- (bool) Remove:(int) itemid
{
    if(itemid<0) return false;
    const char* str = "DELETE FROM chat WHERE weight=?";
    const char* selectstr = "SELECT top 1 * FROM chat WHERE weight=?";
    sqlite3_stmt *tempStmt;
    
    sqlite3_prepare_v2(m_hDb, selectstr, -1, &tempStmt, NULL);
    sqlite3_bind_int(tempStmt, 0, itemid);
    if(sqlite3_step(tempStmt)!=SQLITE_ROW)
    {
        sqlite3_finalize(tempStmt);
        return false;
    }
    sqlite3_finalize(tempStmt);
    
    sqlite3_prepare_v2(m_hDb, str, -1, &tempStmt, NULL);
    sqlite3_bind_int(tempStmt, 0, itemid);
    if(sqlite3_step(tempStmt)==SQLITE_ROW)
    {
        sqlite3_finalize(tempStmt);
        return true;
    }
    sqlite3_finalize(tempStmt);
    return false;
}

//关闭数据库
- (void) Close
{
    if(m_hStatement!=NULL)
        sqlite3_finalize(m_hStatement);
    m_hStatement=NULL;
    if(m_hDb != NULL)
        sqlite3_close(m_hDb);
    m_hDb = NULL;
}

//得到表中总条目数
- (int) GetCount
{
    if(m_hDb == NULL) return -1;
    sqlite3_stmt *tempStmt;
    if(sqlite3_prepare_v2(m_hDb, "SELECT COUNT(*) FROM chat", -1, &tempStmt, NULL)!=SQLITE_OK)
    {
        BWLOG(@"get count have err");
        return -1;
    }
    sqlite3_step(tempStmt);
    int count;
    count = sqlite3_column_int(tempStmt, 0);
    sqlite3_finalize(tempStmt);
    return count;
}

//查找内容是否有
- (bool) Find:(NSString*) data
{
    if(m_hDb == NULL) return false;
    NSString *tempStr = [NSString stringWithFormat:@"SELECT COUNT(data) FROM chat WHERE data='%@'",data];
    sqlite3_stmt *tempStmt;
    int err = sqlite3_prepare_v2(m_hDb, [tempStr UTF8String], -1, &tempStmt, NULL);
    if( err != SQLITE_OK)
    {
        BWLOG(@"find has err");
        return false;
    }
    sqlite3_step(tempStmt);
    int count;
    count = sqlite3_column_int(tempStmt, 0);
    sqlite3_finalize(tempStmt);
    return count>0;
}

@end
