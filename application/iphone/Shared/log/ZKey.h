#ifndef ZKey_h
#define ZKey_h

#include "ZKeyArray.h"
#include "ELogType.h"
#include "GGBase.h"



    /** 
     *  log键值
     *  @author zhangchen
     *  @version 1.0
     */
class ZKey{

 public:
 
	ZKey();
	
    ZKey(ELogType name, int value);

    ZKey(ELogType name, char *value);

    ZKey(ELogType name, ZKeyArray *value);
	
	~ZKey();


    /** 
     *  整理为字符串
     */
    char* getString();

    ZKey operator=(ZKey value);

 private:
    enum
    {
        EInt,
        EString,
        ETeam
    };
    
    /** 
     *  值类型
     */
    int m_nValueType;
    
	union {
    /** 
     *  整形数值
     */
    XU32 m_nNValue;
    
    /** 
     *  字符串数值
     */
    char *m_hSValue;
    
    /** 
     *  键值组
     */
    ZKeyArray *m_hAValue;
	
	};

 private:


    /**
     * @element-type ELogType
     */
    ELogType myELogType;

};

ZKey* getTimeKey();

#endif // ZKey_h
