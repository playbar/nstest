#ifndef XLogEngine_h
#define XLogEngine_h


#include "GGBase.h"

#include "ZKey.h"




    /** 
     *  log写入器
     *  此log用于记录用户行为
     *  由服务器回收
     *  @author zhangchen
     *  @version 1.0
     */
class XLogEngine : public XBase {
    /* {deprecated=false}*/


 public:


    /** 
     *  构造函数
     */
    XLogEngine(char *fname);
	
	/** 
     *  析构函数
     */
    ~XLogEngine();
    
    /**
     *  获得单例
     */
    static XLogEngine* sharedLogEngine()
    {
        static XLogEngine* shared=NULL;
        if(shared==NULL)
            shared = new XLogEngine("C:/ggclient.log");
        return shared;
    }

    /** 
     *  继承自XBace
     *  查看是否需要停止
     */
    virtual XBOOL NeedStop();


    /** 
     *  继承自XBace
     *  线程体
     */
    virtual XUINT CallBack(XU32 nID);


    /** 
     *  写入log
     */
    virtual void WriteLog(ZKey *value);

 private:
    
    /** 
     *  线程句柄
     */
    XThread *m_hThread;
    
    /** 
     *  线程活动控制器
     *  当有log需要写时
     *  此为有事件
     */
    XEvent m_tEvent;
    
    /** 
     *  需要写的log队列
     */
    ZKey *m_rMsg[ 64];
    
    /** 
     *  log队列的读指针
     */
    ZKey **m_plRead;
    
    /** 
     *  log队列的写指针
     */
    ZKey **m_plWrite;
    
    /** 
     *  log文件名
     */
    char *m_hFileName;
    
    /** 
     *  通知停止标记
     */
    XBOOL m_bNeedStop;
};

#endif // XLogEngine_h
