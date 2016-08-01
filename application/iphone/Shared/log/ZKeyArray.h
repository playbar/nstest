#ifndef ZKeyArray_h
#define ZKeyArray_h


#include "GGBase.h"

class ZKey;


    /** 
     *  log键值序列
     *  @author zhangchen
     *  @version 1.0
     */
class ZKeyArray {

 public:

    ZKeyArray();
    
    ZKeyArray(const ZKeyArray& array);
	
	~ZKeyArray();


    /** 
     *  添加log键值
     */
    virtual bool addKey(ZKey *element);

    virtual char* getString();

 private:

    /**
     * @element-type ZKey
     */
    XVector<ZKey*>  m_tZKey;
};

#endif // ZKeyArray_h
