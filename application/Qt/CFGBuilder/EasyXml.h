#ifndef EASYXML_H
#define EASYXML_H

#include "EasyBase.h"
#include <QString>

// XmlBase
class XEasyXmlBase : public XEasyBase
{
public:
    XEasyXmlBase();
    XEasyXmlBase(XEasyXmlBase* aParent);
    virtual ~XEasyXmlBase();

    virtual QString NodeName() { return iXmlNodeName; }
    virtual void SetNodeName(QString aNodeName) { iXmlNodeName = aNodeName; }

    virtual XEasyXmlBase* ParentNode() const { return iParentXmlNode; }
    virtual void SetParentNode(XEasyXmlBase* aParent) { iParentXmlNode = aParent; }

    virtual XEasyXmlBase* PrevBrotherNode() const { return iPrevBrotherXmlNode; }
    virtual void SetPrevBrotherNode(XEasyXmlBase* aPrevNode) { iPrevBrotherXmlNode = aPrevNode; }

    virtual XEasyXmlBase* NextBrotherNode() const { return iNextBrotherXmlNode; }
    virtual void SetNextBrotherNode(XEasyXmlBase* aNextNode) { iNextBrotherXmlNode = aNextNode; }

    enum EXmlNodeType
    {
            NT_XmlBase,         // 未定义节点
            NT_XmlEle,          // 元素节点
            NT_XmlAtt,          // 属性节点
            NT_XmlEleGroup,     // 元素组节点
            NT_XmlAttGroup,     // 属性组节点
            NT_XmlNote,         // 注释节点
            NT_XmlHeader,       // 头节点
            NT_XmlDTD,          // DTD文件节点
            NT_XmlUnknown       // 未知节点
    };

    virtual EXmlNodeType NodeType() const { return iXmlNodeType; }
    virtual void SetNodeType(EXmlNodeType aNodeType) { iXmlNodeType = aNodeType; }

    virtual int VDepth() const { return iEleCursor.s_Vdepth; }
    virtual void SetVDepth(int aVDepth) { iEleCursor.s_Vdepth = aVDepth; }
    virtual int HDepth() const { return iEleCursor.s_Hdepth; }
    virtual void SetHDepth(int aHDepth) { iEleCursor.s_Hdepth = aHDepth; }

protected:
    // 节点名称
    QString             iXmlNodeName;

    // 父节点
    XEasyXmlBase*	iParentXmlNode;

    // 前一个同级节点
    XEasyXmlBase*	iPrevBrotherXmlNode;
    // 后一个同级节点
    XEasyXmlBase*	iNextBrotherXmlNode;

    // 节点类型
    EXmlNodeType	iXmlNodeType;

    // 元素游标
    XEleCursor      iEleCursor;
};

// 元素节点
class XEasyXmlEle : public XEasyXmlBase
{
public:
    XEasyXmlEle();
    XEasyXmlEle(XEasyXmlBase* aParent);
    ~XEasyXmlEle();

    void AddEle(XEasyXmlBase *aEle);
    void AddAtt(XEasyXmlBase *aAtt);

    QString Content() { return iContent; }
    QString Content(QString aEleName);
    bool SetContent(QString aContent) { iContent.clear(); iContent = aContent; return true; }
    bool SetContent(QString aEleName, QString aContent);

    void SetOver(bool aFlag) { iOverFlag = aFlag; }
    bool IfOver() { return iOverFlag; }

    int ChildCount(QString aEleName, QString aChildEleName);
    int ChildCount_this(QString aChildEleName);

    XEasyXmlBase* FindChild(QString aEleName, QString aChildEleName, int aIndex);
    XEasyXmlBase* FindChild_this(QString aChildEleName, int aIndex);

    bool HasEle(QString aParentName, QString aEleName);
    bool HasEle_this(QString aEleName);

    bool AddEle(QString aParentName, QString aEleName);
    bool AddEle_this(QString aEleName);

    bool DelEle(QString aParentName, QString aEleName);
    bool DelEle_this(QString aElename);

    bool HasAtt(QString aEleName, QString aAttName);                    // new
    bool HasAtt_this(QString aAttName);                                 // new
    bool HasAttI(QString aEleName, QString aAttName, int aIndex);       // new

    bool AddAtt(QString aEleName, QString aAttName);                    // new
    bool AddAtt(QString aEleName, QString aAttName, QString aAttValue); // new
    bool AddAtt_this(QString aAttName);                                 // new
    bool AddAtt_this(QString aAttName, QString aAttValue);              // new
    bool AddAttI(QString aEleName, QString aAttName, int aIndex);   // new
    bool AddAttI(QString aEleName, QString aAttName, QString aAttValue, int aIndex);    // new

    bool DelAtt(QString aEleName, QString aAttName);
    bool DelAtt_this(QString aAttName);
    bool DelAttI(QString aEleName, QString aAttName, int aIndex);       // new

    bool ModifyAtt(QString aEleName, QString aAttName, QString aAttNewValue);   // new
    bool ModifyAtt_this(QString aAttName, QString aAttNewValue);        // new
    bool ModifyAttI(QString aEleName, QString aAttName, QString aAttNewValue, int aIndex);  // new

    QString FindAtt(QString aEleName, QString aAttName);
    QString FindAtt_this(QString aAttName);

    QString FindAttI(QString aEleName, QString aAttName, int aIndex);
    QString FindContent(QString aEleName, QString aAttName, QString aAttValue);
    QString FindContent_this(QString aAttName, QString aAttValue);

    void Process(QString aMsg, XEasyXmlBase*& aCursor);
    QString GenerateXml();

//    void Process(QString aMsg, XEasyXmlBase*& aCursor);	// pass
//    string Output();	// pass

//    void AddEle(XEasyXmlBase* aEle);
//    void AddAtt(XEasyXmlBase* aAtt);

//    string Content() { return iContent; }	// added
//    void SetContent(string aContent) { iContent = aContent; }	// added

//    void SetOver(bool aFlag) { iOverFlag = aFlag; }	// added
//    bool IfOver() { return iOverFlag; }	// added

//    int FindChildCount(string aEleName, string aChildEleName);	// added FindChildCount已更改为ChildCount
//    int ChildCount(string aChildEleName);	// added ChildCount已更改为ChildCount_this

//    XEasyXmlBase* FindChild(string aEleName, string aChildEleName, int aIndex);	// added
//    XEasyXmlBase* FindChild_this(string aChildEleName, int aIndex);	// added

//    string FindAtt(string aEleName, string aAttName);	// added
//    string FindAtt_this(string aAttName);	// added

//    string FindAttI(string aEleName, string aAttName, int aIndex);	// added
//    string FindAttV(string aEleName, string aAttName, string aAttValue);	// added FindAttV已更改为FindContent
//    string FindAttV_this(string aAttName, string aAttValue);	// added FindAttV_this已更改为FindContent_this

protected:
    // 子节点
    XEasyXmlBase*	iChildGroup;
    // 属性节点
    XEasyXmlBase*	iAttGroup;
    // 结束标记
    bool                iOverFlag;
    // 内容
    QString             iContent;
};

// 属性节点
class XEasyXmlAtt : public XEasyXmlBase
{
public:
    XEasyXmlAtt();
    XEasyXmlAtt(XEasyXmlBase* aParent);
    ~XEasyXmlAtt();

    QString AttValue() { return iAttValue; }
    void SetAttValue(QString aValue) { iAttValue=aValue; }

    void Process(QString aMsg);
    QString GenerateXml();

//    string AttValue() { return iAttValue; }	// added
//    void SetAttValue(string aValue) { iAttValue=aValue; }	// added

//    void Process(string aMsg);
//    string Output();

protected:
    // 属性值
    QString iAttValue;
//    string iAttValue;
};

// 节点组
class XEasyXmlGroup : public XEasyXmlBase
{
public:
    XEasyXmlGroup();
    XEasyXmlGroup(XEasyXmlBase* aParent);
    ~XEasyXmlGroup();

    void AddChild(XEasyXmlBase *aChild);
    void DelChild(XEasyXmlBase *aChild);
    void Reset();

    QString Content(QString aEleName);
    bool SetContent(QString aEleName, QString aContent);

    int ChildCount(QString aEleName, QString aChildEleName);
    int ChildCount_this(QString aChildEleName);
    int Count() const { return iChildCount; }

    XEasyXmlBase* FindChild(QString aEleName, QString aChildEleName, int aIndex);
    XEasyXmlBase* FindChild_this(QString aChildEleName, int aIndex);

    bool HasEle(QString aParentName, QString aEleName);                 // OK
    bool HasEle_this(QString aEleName);                                 // OK

    bool AddEle(QString aParentName, QString aEleName);                         // OK
    bool AddEle_this(QString aEleName);                 // 必须返回true         OK

    bool DelEle(QString aParentName, QString aEleName);                     // OK
    bool DelEle_this(QString aElename);                 // 必须返回true         OK

    bool HasAtt(QString aEleName, QString aAttName);                    // new          OK
    bool HasAtt_this(QString aAttName);                                 // new          OK
    bool HasAttI(QString aEleName, QString aAttName, int aIndex);       // new          OK

    bool AddAtt(QString aEleName, QString aAttName);                    // new          OK
    bool AddAtt(QString aEleName, QString aAttName, QString aAttValue); // new          OK
    bool AddAtt_this(QString aAttName);                                 // 必须返回true     OK
    bool AddAtt_this(QString aAttName, QString aAttValue);              // 必须返回true      OK
    bool AddAttI(QString aEleName, QString aAttName, int aIndex);   // new                  OK
    bool AddAttI(QString aEleName, QString aAttName, QString aAttValue, int aIndex);    // new  OK

    bool DelAtt(QString aEleName, QString aAttName);                                        // OK
    bool DelAtt_this(QString aAttName);                     // 必须返回true                     OK
    bool DelAttI(QString aEleName, QString aAttName, int aIndex);       // new                  OK

    bool ModifyAtt(QString aEleName, QString aAttName, QString aAttNewValue);   // new          OK
    bool ModifyAtt_this(QString aAttName, QString aAttNewValue);        // 必须返回true         OK
    bool ModifyAttI(QString aEleName, QString aAttName, QString aAttNewValue, int aIndex);  //  OK

    QString FindAtt(QString aEleName, QString aAttName);
    QString FindAtt_this(QString aAttName);

    QString FindAttI(QString aEleName, QString aAttName, int aIndex);
    QString FindContent(QString aEleName, QString aAttName, QString aAttValue);
    QString FindContent_this(QString aAttName, QString aAttValue);

    QString GenerateXml();

//    void AddChild(XEasyXmlBase* aChild);	// add
//    void DelChild(XEasyXmlBase* aChild);	// add
//    void Reset();	// add
//    int FindChildCount(string aEleName, string aChildEleName);	// added FindChildCount改为ChildCount
//    int ChildCount(string aChildEleName);	// added ChildCount改为ChildCount_this
//    XEasyXmlBase* FindChild(string aEleName, string aChildElename, int aIndex);	// added
//    XEasyXmlBase* FindChild_this(string aChildEleName, int aIndex);	// added

//    string FindAtt(string aEleName, string aAttName);	// added
//    string FindAtt_this(string aAttName);	// added

//    string FindAttI(string aEleName, string aAttName, int aIndex);	// added
//    string FindAttV(string aEleName, string aAttName, string aAttValue);	// added FindAttV已更改为FIndContent
//    string FindAttV_this(string aAttName, string aAttValue);	// added FindAttV_this已更改为FindContent_this

//    string Output();

//    void Print();

protected:
    // 指向第一个子节点
    XEasyXmlBase* iFirstChildNode;
    // 指向最后一个子节点
    XEasyXmlBase* iLastChildNode;
    // 节点数
    int iChildCount;
};

// 未知
class XEasyXmlUnknown : public XEasyXmlBase
{
public:
    XEasyXmlUnknown();
    XEasyXmlUnknown(XEasyXmlBase* aParent);
    ~XEasyXmlUnknown();

    void Process(QString aMsg);
    QString GenerateXml();

//    void Process(string aMsg);
//    string Output();

protected:
    // 未知信息
    QString     iMsg;
//    string	iMsg;
};

// 注释
class XEasyXmlNote : public XEasyXmlBase
{
public:
    XEasyXmlNote();
    XEasyXmlNote(XEasyXmlBase* aParent);
    ~XEasyXmlNote();

    QString Note() { return iMsg; }
    void SetNote(QString aMsg) { iMsg = aMsg; }

    void Process(QString aMsg);
    QString GenerateXml();

//    string Note() { return iMsg; }
//    void SetNote(string aMsg) { iMsg = aMsg; }
//    void Process(string aMsg);
//    string Output();

protected:
    // 注释信息
    QString iMsg;

//    string iMsg;
};

// Xml Header
class XEasyXmlHeader
{
public:
    XEasyXmlHeader();
    ~XEasyXmlHeader();

    QString Version() { return iVersion; }
    void SetVersion(QString aVersion) { iVersion = aVersion; }
    QString Encoding() { return iEncoding; }
    void SetEncoding(QString aEncoding) { iEncoding = aEncoding; }

    void Process(QString aMsg);
    QString GenerateXml();

protected:
    // 版本
    QString iVersion;
    // 编码
    QString iEncoding;

//    string iVersion;
//    string iEncoding;
};

// Xml DTD
class XEasyXmlDtd
{
public:
    XEasyXmlDtd();
    ~XEasyXmlDtd();

    QString RootName() { return iRootName; }
    void SetRootName(QString aRootName) { iRootName = aRootName; }
    QString DtdLocation() { return iLocation; }
    void SetDtdLocation(QString aLocation) { iLocation = aLocation; }

    void Process(QString aMsg);
    QString GenerateXml();

//    string RootName() { return iRootName; }
//    void SetRootName(string aRootName) { iRootName = aRootName; }
//    string Dtdlocal() { return iLocal; }
//    void SetDtdLocal(string aLocal) { iLocal = aLocal; }

//    void Process(string aMsg);
//    string Output();

protected:
    // Root Name
    QString iRootName;
    // Location
    QString iLocation;

//    string iRootName;
//    string iLocal;
};

// Xml Reader
class XEasyXml
{
public:
    XEasyXml();
    ~XEasyXml();

    XEasyXmlBase* Root() { return iRoot; }
//    XEasyXmlDtdDocument* DTDReader() { return iDTDReader; }

    void OpenXml(const QString& aPath);
    QString GenerateXml();

//    void Process();

    QString Content(QString aEleName) const;
    bool SetContent(QString aEleName, QString aContent);

    bool HasEle(QString aParentName, QString aEleName);
    bool AddEle(QString aParentName, QString aEleName);
    bool DelEle(QString aParentName, QString aEleName);

    bool HasAtt(QString aEleName, QString aAttName);                    // new
    bool HasAttI(QString aEleName, QString aAttName, int aIndex);       // new

    bool AddAtt(QString aEleName, QString aAttName);                    // new
    bool AddAtt(QString aEleName, QString aAttName, QString aAttValue); // new
    bool AddAttI(QString aEleName, QString aAttName, int aIndex);   // new
    bool AddAttI(QString aEleName, QString aAttName, QString aAttValue, int aIndex);    // new

    bool DelAtt(QString aEleName, QString aAttName);
    bool DelAttI(QString aEleName, QString aAttName, int aIndex);       // new

    bool ModifyAtt(QString aEleName, QString aAttName, QString aAttNewValue);   // new
    bool ModifyAttI(QString aEleName, QString aAttName, QString aAttNewValue, int aIndex);  // new

    QString FindAtt(QString aEleName, QString aAttName);
    QString FindAttI(QString aEleName, QString aAttName, int aIndex);
    QString FindContent(QString aEleName, QString aAttName, QString aAttValue);

    int ChildCount(QString aEleName, QString aChildEleName);
    XEasyXmlBase* FindChild(QString aEleName, QString aChildEleName, int aIndex);

    QString Version() { if(iXmlHeader){return iXmlHeader->Version();} }
    void SetVersion(QString aVersion) { if(iXmlHeader){iXmlHeader->SetVersion(aVersion);} }
    QString Encoding() { if(iXmlHeader){return iXmlHeader->Encoding();} }
    void SetEncoding(QString aEncoding) { if(iXmlHeader){iXmlHeader->SetEncoding(aEncoding);} }

    QString RootName() { if(iDtd){return iDtd->RootName();} }
    void SetRootName(QString aRootName) { if(iDtd){iDtd->SetRootName(aRootName);} }
    QString DtdLocation() { if(iDtd){return iDtd->DtdLocation();} }
    void SetDtdLocation(QString aLocation) { if(iDtd){iDtd->SetDtdLocation(aLocation);} }

protected:
    int SwitchType();
    void CleanString();

private:
    // Xml Header
    XEasyXmlHeader* iXmlHeader;
    // Xml Dtd
    XEasyXmlDtd*    iDtd;

    // Xml Dom
    XEasyXmlBase* iRoot;
    // Dtd Dom
//    XEasyDtdDocument* iDtdReader;

    // 游标指针
    XEasyXmlBase*   iXmlCursor;

    QString iMsg;

    // DTD路径(保留)
    QString iLocation;

    bool iHelpStart;
    QString iHelpMsg;
};


#endif // EASYXML_H
