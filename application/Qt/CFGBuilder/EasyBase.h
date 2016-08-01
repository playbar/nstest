/*
 *
 */

#ifndef EASYBASE_H
#define EASYBASE_H

#define SAFE_DEL(p) {if(p) delete p; p=NULL;}

// 光标，用于定位光标位置
struct XFileCursor
{
    XFileCursor()
    {
        Reset();
    }
    void Reset()
    {
        s_XRow = s_YCol = 0;
    }

    int s_XRow;
    int s_YCol;
};

// 元素游标，用于定位该元素在整个树形结构中的位置
struct XEleCursor
{
    XEleCursor()
    {
        Reset();
    }
    void Reset()
    {
        s_Vdepth = s_Hdepth = 0;
    }

    int s_Vdepth;
    int s_Hdepth;
};

// EasyXml中的基本类
class XEasyBase
{
public:
    XEasyBase() {}
    virtual ~XEasyBase() {}

    virtual int Row() const { return iFileCursor.s_XRow; }
    virtual int Col() const { return iFileCursor.s_YCol; }

    virtual void SetRow(int aRow) { iFileCursor.s_XRow = aRow; }
    virtual void SetCol(int aCol) { iFileCursor.s_YCol = aCol; }

protected:
    XFileCursor     iFileCursor;
};

#endif // EASYBASE_H
