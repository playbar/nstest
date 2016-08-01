/*
 *
 */

#include "EasyXml.h"
#include "CustomDefine.h"
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QDebug>

// XEasyXmlBase
XEasyXmlBase::XEasyXmlBase()
{
    iParentXmlNode = NULL;
    iPrevBrotherXmlNode = NULL;
    iNextBrotherXmlNode = NULL;

    iXmlNodeType = NT_XmlBase;
}

XEasyXmlBase::XEasyXmlBase(XEasyXmlBase* aParent)
{
    iParentXmlNode = aParent;
    iPrevBrotherXmlNode = NULL;
    iNextBrotherXmlNode = NULL;

    iXmlNodeType = NT_XmlBase;
}

XEasyXmlBase::~XEasyXmlBase()
{
}

// XEasyXmlEle
XEasyXmlEle::XEasyXmlEle()
{
    iChildGroup = NULL;
    iAttGroup = NULL;
    iOverFlag = false;

    iXmlNodeType = NT_XmlEle;
}

XEasyXmlEle::XEasyXmlEle(XEasyXmlBase* aParent)
{
    iChildGroup = NULL;
    iAttGroup = NULL;
    iOverFlag = false;

    iXmlNodeType = NT_XmlEle;

    iParentXmlNode = aParent;
    if(iParentXmlNode!=NULL)
        iEleCursor.s_Vdepth = iParentXmlNode->VDepth()+1;
}

XEasyXmlEle::~XEasyXmlEle()
{
    SAFE_DEL(iChildGroup);
    SAFE_DEL(iAttGroup);
}

void XEasyXmlEle::AddEle(XEasyXmlBase *aEle)
{
    if(iChildGroup==NULL)
    {
        iChildGroup = new XEasyXmlGroup(this);
        iChildGroup->SetNodeType(XEasyXmlBase::NT_XmlEleGroup);
    }
    ((XEasyXmlGroup*)iChildGroup)->AddChild(aEle);
}

void XEasyXmlEle::AddAtt(XEasyXmlBase *aAtt)
{
    if(iAttGroup==NULL)
    {
        iAttGroup = new XEasyXmlGroup(this);
        iAttGroup->SetNodeType(XEasyXmlBase::NT_XmlAttGroup);
    }
    ((XEasyXmlGroup*)iAttGroup)->AddChild(aAtt);
}

int XEasyXmlEle::ChildCount(QString aEleName, QString aChildEleName)
{
    if(this->NodeName()==aEleName)
    {
        return ChildCount_this(aChildEleName);
    }
    else
    {
        if(iChildGroup!=NULL)
        {
            return ((XEasyXmlGroup*)iChildGroup)->ChildCount(aEleName, aChildEleName);
        }
        else
        {
            return 0;
        }
    }
}

int XEasyXmlEle::ChildCount_this(QString aChildEleName)
{
    int count = 0;
    if(iChildGroup!=NULL)
    {
        count = ((XEasyXmlGroup*)iChildGroup)->ChildCount_this(aChildEleName);
    }

    return count;
}

XEasyXmlBase* XEasyXmlEle::FindChild(QString aEleName, QString aChildEleName, int aIndex)
{
    XEasyXmlBase* value = NULL;
    if(this->NodeName()==aEleName)
    {
        if(iChildGroup!=NULL)
        {
            value = ((XEasyXmlGroup*)iChildGroup)->FindChild_this(aChildEleName, aIndex);
        }
    }
    else
    {
        if(iChildGroup!=NULL)
        {
            value = ((XEasyXmlGroup*)iChildGroup)->FindChild(aEleName, aChildEleName, aIndex);
        }
    }

    return value;
}

XEasyXmlBase* XEasyXmlEle::FindChild_this(QString aChildEleName, int aIndex)
{
    XEasyXmlBase* value = NULL;
    if(iChildGroup!=NULL)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->FindChild_this(aChildEleName, aIndex);
    }

    return value;
}

bool XEasyXmlEle::HasEle(QString aParentName, QString aEleName)
{
    bool value = false;

    if(this->NodeName()==aParentName)
    {
        value = this->HasEle_this(aEleName);
    }
    else if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->HasEle(aParentName, aEleName);
    }

    return value;
}

bool XEasyXmlEle::HasEle_this(QString aEleName)
{
    bool value = false;

    if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->HasEle_this(aEleName);
    }

    return value;
}

bool XEasyXmlEle::AddEle(QString aParentName, QString aEleName)
{
    bool value = false;

    if(this->NodeName()==aParentName)
    {
        value = this->AddEle_this(aEleName);
    }
    else
    {
        if(iChildGroup==NULL)
        {
            iChildGroup = new XEasyXmlGroup(this);
            iChildGroup->SetNodeType(XEasyXmlBase::NT_XmlEleGroup);

            if(iChildGroup)
            {
                value = ((XEasyXmlGroup*)iChildGroup)->AddEle(aParentName, aEleName);
            }
            if(!value)
                SAFE_DEL(iChildGroup);
        }
        else if(iChildGroup)
        {
            value = ((XEasyXmlGroup*)iChildGroup)->AddEle(aParentName, aEleName);
        }
    }

    return value;
}

bool XEasyXmlEle::AddEle_this(QString aEleName)
{
    bool value = false;

    if(iChildGroup==NULL)
    {
        iChildGroup = new XEasyXmlGroup(this);
        iChildGroup->SetNodeType(XEasyXmlBase::NT_XmlEleGroup);
    }

    if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->AddEle_this(aEleName);
    }

    return value;
}

bool XEasyXmlEle::DelEle(QString aParentName, QString aEleName)
{
    bool value = false;

    if(this->NodeName()==aParentName)
    {
        value = this->DelEle_this(aEleName);
    }
    else if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->DelEle(aParentName, aEleName);
    }

    return value;
}

bool XEasyXmlEle::DelEle_this(QString aEleName)
{
    bool value = false;

    if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->DelEle_this(aEleName);
        if(((XEasyXmlGroup*)iChildGroup)->Count()==0)
            SAFE_DEL(iChildGroup);
    }

    return value;
}

bool XEasyXmlEle::HasAtt(QString aEleName, QString aAttName)
{
    bool value = false;

    if(this->NodeName()==aEleName)
    {
        value = this->HasAtt_this(aAttName);
    }
    else if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->HasAtt(aEleName, aAttName);
    }

    return value;
}

bool XEasyXmlEle::HasAtt_this(QString aAttName)
{
    bool value = false;

    if(iAttGroup)
    {
        value = ((XEasyXmlGroup*)iAttGroup)->HasAtt_this(aAttName);
    }

    return value;
}

bool XEasyXmlEle::HasAttI(QString aEleName, QString aAttName, int aIndex)
{
    bool value = false;

    if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->HasAttI(aEleName, aAttName, aIndex);
    }

    return value;
}

bool XEasyXmlEle::AddAtt(QString aEleName, QString aAttName)
{
    bool value = false;

    if(this->NodeName()==aEleName)
        value = this->AddAtt_this(aEleName);
    else
    {
        if(iChildGroup)
        {
            value = ((XEasyXmlGroup*)iChildGroup)->AddAtt(aEleName, aAttName);
        }
//        if(iChildGroup==NULL)
//        {
//            iChildGroup = new XEasyXmlGroup(this);
//            iChildGroup->SetNodeType(XEasyXmlBase::NT_XmlEleGroup);
//        }
//        if(iChildGroup)
//        {
//            value = ((XEasyXmlGroup*)iChildGroup)->AddAtt(aEleName, aAttName);
//        }
    }

    return value;
}

bool XEasyXmlEle::AddAtt(QString aEleName, QString aAttName, QString aAttValue)
{
    bool value = false;

    if(this->NodeName()==aEleName)
        value = this->AddAtt_this(aAttName, aAttValue);
    else
    {
        if(iChildGroup==NULL)
        {
            iChildGroup = new XEasyXmlGroup(this);
            iChildGroup->SetNodeType(XEasyXmlBase::NT_XmlEleGroup);

            if(iChildGroup)
            {
                value = ((XEasyXmlGroup*)iChildGroup)->AddAtt(aEleName, aAttName, aAttValue);
            }
            if(!value)
                SAFE_DEL(iChildGroup);
        }
        else if(iChildGroup)
        {
            value = ((XEasyXmlGroup*)iChildGroup)->AddAtt(aEleName, aAttName, aAttValue);
        }
    }

    return value;
}

QString XEasyXmlEle::Content(QString aEleName)
{
    QString value;

    if(this->NodeType()==XEasyXmlBase::NT_XmlEle)
    {
        if(this->NodeName()==aEleName)
        {
            value = this->Content();
            return value;
        }
        else if(iChildGroup)
        {
            value = ((XEasyXmlGroup*)iChildGroup)->Content(aEleName);
        }
    }

    return value;
}

bool XEasyXmlEle::SetContent(QString aEleName, QString aContent)
{
    bool value = false;

    if(this->NodeName()==aEleName)
    {
        value = this->SetContent(aContent);
    }
    else if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->SetContent(aEleName, aContent);
    }

    return value;
}

bool XEasyXmlEle::AddAtt_this(QString aAttName)
{
    bool value = false;
    if(iAttGroup==NULL)
    {
        iAttGroup = new XEasyXmlGroup(this);
        iAttGroup->SetNodeType(XEasyXmlBase::NT_XmlAttGroup);
    }
    if(iAttGroup)
    {
        value = ((XEasyXmlGroup*)iAttGroup)->AddAtt_this(aAttName);
    }

    return value;
}

bool XEasyXmlEle::AddAtt_this(QString aAttName, QString aAttValue)
{
    bool value = false;
    if(iAttGroup==NULL)
    {
        iAttGroup = new XEasyXmlGroup(this);
        iAttGroup->SetNodeType(XEasyXmlBase::NT_XmlAttGroup);
    }
    if(iAttGroup)
    {
        value = ((XEasyXmlGroup*)iAttGroup)->AddAtt_this(aAttName, aAttValue);
    }

    return value;
}

bool XEasyXmlEle::AddAttI(QString aEleName, QString aAttName, int aIndex)
{
    bool value = false;
    if(iChildGroup==NULL)
    {
        iChildGroup = new XEasyXmlGroup(this);
        iChildGroup->SetNodeType(XEasyXmlBase::NT_XmlEleGroup);

        if(iChildGroup)
        {
            value = ((XEasyXmlGroup*)iChildGroup)->AddAttI(aEleName, aAttName, aIndex);
        }
        if(!value)
            SAFE_DEL(iChildGroup);
    }
    else if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->AddAttI(aEleName, aAttName, aIndex);
    }

    return value;
}

bool XEasyXmlEle::AddAttI(QString aEleName, QString aAttName, QString aAttValue, int aIndex)
{
    bool value = false;
    if(iChildGroup==NULL)
    {
        iChildGroup = new XEasyXmlGroup(this);
        iChildGroup->SetNodeType(XEasyXmlBase::NT_XmlEleGroup);

        if(iChildGroup)
        {
            value = ((XEasyXmlGroup*)iChildGroup)->AddAttI(aEleName, aAttName, aAttValue, aIndex);
        }
        if(!value)
            SAFE_DEL(iChildGroup);
    }
    else if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->AddAttI(aEleName, aAttName, aAttValue, aIndex);
    }

    return value;
}

bool XEasyXmlEle::DelAtt(QString aEleName, QString aAttName)
{
    bool value = false;
    if(this->NodeName()==aEleName)
        value = this->DelAtt_this(aAttName);
    else if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->DelAtt(aEleName, aAttName);
    }

    return value;
}

bool XEasyXmlEle::DelAtt_this(QString aAttName)
{
    bool value = false;
    if(iAttGroup)
    {
        value = ((XEasyXmlGroup*)iAttGroup)->DelAtt_this(aAttName);
        if(((XEasyXmlGroup*)iAttGroup)->Count()==0)
            SAFE_DEL(iAttGroup);
    }

    return value;
}

bool XEasyXmlEle::DelAttI(QString aEleName, QString aAttName, int aIndex)
{
    bool value = false;
    if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->DelAttI(aEleName, aAttName, aIndex);
    }

    return value;
}

bool XEasyXmlEle::ModifyAtt(QString aEleName, QString aAttName, QString aAttNewValue)
{
    bool value = false;
    if(this->NodeName()==aEleName)
        value = this->ModifyAtt_this(aAttName, aAttNewValue);
    else if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->ModifyAtt(aEleName, aAttName, aAttNewValue);
    }

    return value;
}

bool XEasyXmlEle::ModifyAtt_this(QString aAttName, QString aAttNewValue)
{
    bool value = false;
    if(iAttGroup)
    {
        value = ((XEasyXmlGroup*)iAttGroup)->ModifyAtt_this(aAttName, aAttNewValue);
    }

    return value;
}

bool XEasyXmlEle::ModifyAttI(QString aEleName, QString aAttName, QString aAttNewValue, int aIndex)
{
    bool value = false;
    if(iChildGroup)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->ModifyAttI(aEleName, aAttName, aAttNewValue, aIndex);
    }

    return value;
}

QString XEasyXmlEle::FindAtt(QString aEleName, QString aAttName)
{
    QString value;
    if(this->NodeName()==aEleName)
    {
        value = this->FindAtt_this(aAttName);
    }
    else if(iChildGroup!=NULL)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->FindAtt(aEleName, aAttName);
    }

    return value;
}

QString XEasyXmlEle::FindAtt_this(QString aAttName)
{
    QString value;
    if(iAttGroup!=NULL)
    {
        value = ((XEasyXmlGroup*)iAttGroup)->FindAtt_this(aAttName);
    }

    return value;
}

QString XEasyXmlEle::FindAttI(QString aEleName, QString aAttName, int aIndex)
{
    QString value;
    if(iChildGroup!=NULL)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->FindAttI(aEleName, aAttName, aIndex);
    }

    return value;
}

QString XEasyXmlEle::FindContent(QString aEleName, QString aAttName, QString aAttValue)
{
    QString value;
    if(this->NodeName()==aEleName)
    {
        value = FindContent_this(aAttName, aAttValue);
    }
    else if(iChildGroup!=NULL)
    {
        value = ((XEasyXmlGroup*)iChildGroup)->FindContent(aEleName, aAttName, aAttValue);
    }

    return value;
}

QString XEasyXmlEle::FindContent_this(QString aAttName, QString aAttValue)
{
    QString value;
    if(iAttGroup!=NULL)
    {
        value = ((XEasyXmlGroup*)iAttGroup)->FindContent_this(aAttName, aAttValue);
    }

    return value;
}

void XEasyXmlEle::Process(QString aMsg, XEasyXmlBase*& aCursor)
{
    // 判断是否是</XXX>结束元素符
    QString flag = aMsg.mid(1, 1);
    if(flag==GlobalNAskew)
    {
        // 如果当前游标指针指向的节点名等于结束元素名，直接标记结束元素
        // 之后游标上移，否则直接PASS
        QString OverNodeName;
        int start = 0;
        int end = 0;

        start = aMsg.indexOf(GlobalNAskew);
        end = aMsg.indexOf(GlobalBigFlag, start+1);
        if(start==-1 || end==-1)
            return;

        OverNodeName = aMsg.mid(start+1, end-start-1);

        if(aCursor->NodeName()==OverNodeName)
        {
            ((XEasyXmlEle*)aCursor)->SetOver(true);
            aCursor = aCursor->ParentNode();
        }
    }
    else
    {
        // 获取节点名称以及属性群
        int flagstart = 0;
        int flagend = 0;
        flagstart = aMsg.indexOf(GlobalSmallFlag);
        flagend = aMsg.indexOf(GlobalBlank, flagstart+1);

        if(flagstart==-1)
            return;
        if(flagend==-1)
        {
            flagend = aMsg.indexOf(GlobalNAskew);
            if(flagend==-1)
                flagend = aMsg.length()-1;
        }

        this->SetNodeName(aMsg.mid(flagstart+1, flagend-flagstart-1));

        while(aMsg.indexOf(GlobalEqualTo, flagend)!=-1)
        {
            flagstart = aMsg.indexOf(GlobalBlank, flagend);
            flagend = aMsg.indexOf(GlobalBlank, flagstart+1);

            if(flagend==-1)
            {
                flagend = aMsg.indexOf(GlobalBigFlag, flagstart+1);
            }

            if(flagstart!=-1 && flagend!=-1)
            {
                XEasyXmlBase* att = new XEasyXmlAtt;
                ((XEasyXmlAtt*)att)->Process(aMsg.mid(flagstart+1, flagend-flagstart-1));
                ((XEasyXmlEle*)aCursor)->AddAtt(att);
            }
        }

        // 判断是否是<../>结束元素
        flagend = aMsg.indexOf(GlobalBigFlag);
        if(flagend==-1)
            return;                     // 应该标记出错，因为未找到与“<”标记相对应的“>”标记

        if(aMsg.mid(flagend-1, 1)==GlobalNAskew)
        {
            ((XEasyXmlEle*)aCursor)->SetOver(true);
            aCursor = aCursor->ParentNode();
        }
//        else
//        {
//            iContent = aMsg.mid(flagend+1);
//        }
    }
}

QString XEasyXmlEle::GenerateXml()
{
    QString output;

    // 因为设定Root的vdepth为0，所以要从1开始生成信息
    if(iEleCursor.s_Vdepth!=0)
    {
        // 节点名为空，直接return
        if(iXmlNodeName.length()==0)
            return output;
        // 没有属性，没有子元素，且没有内容，直接返回
        if(iAttGroup==NULL && iChildGroup==NULL && iContent.length()==0)
            return output;
        
        // 缓存下数据，如果没属性
        QString tmpatt; // att的缓存
        QString tmpele; // ele的缓存
        QString tmpcot; // context的缓存
        
//        output.append(GlobalSmallFlag); // 
//        output.append(iXmlNodeName);    // iXmlNodeName.length()
        tmpele.append(GlobalSmallFlag); // tmpele + <
        tmpele.append(iXmlNodeName);    // tmpele + iXmlNodeName的length

        if(iAttGroup!=NULL)
        {
//            output.append(GlobalBlank); // +1
//            output += ((XEasyXmlGroup*)iAttGroup)->GenerateXml();
            
            tmpatt.append(GlobalBlank); // tmpatt + 空格
            tmpatt += ((XEasyXmlGroup*)iAttGroup)->GenerateXml();
            
            // 如果没有属性，则清空tmpatt
            if(tmpatt.length()==1)
                tmpatt.clear();
            
            tmpele.append(tmpatt);
        }
        
        // 属性已经处理完毕，可以再后面直接加上tmpatt

        if(iChildGroup!=NULL)
        {
//            output.append(GlobalBigFlag);   // +1

//            output += ((XEasyXmlGroup*)iChildGroup)->GenerateXml();

//            output.append(GlobalNewLine);
//            for(int i=0; i<iEleCursor.s_Vdepth-1; i++)
//                output.append(GlobalTab);
//            output.append(GlobalSmallFlag);
//            output.append(GlobalNAskew);
//            output.append(iXmlNodeName);
//            output.append(GlobalBigFlag);
            
            tmpele.append(GlobalBigFlag);   // tmpele + >
            tmpele += ((XEasyXmlGroup*)iChildGroup)->GenerateXml();
            tmpele.append(GlobalNewLine);   // tmpele + 换行
            int i = 0;
            for(i=0; i<iEleCursor.s_Vdepth-1; i++)
                tmpele.append(GlobalTab);   // tmpele + i*tab
            
            tmpele.append(GlobalSmallFlag); // tmpele + <
            tmpele.append(GlobalNAskew);    // tmpele + /
            tmpele.append(iXmlNodeName);    // tmpele + iXmlNodeName的length
            tmpele.append(GlobalBigFlag);   // tmpele + >
            
            int length = 2;
            length += iXmlNodeName.length();
            length += 2;
            length += i;
            length += 3;
            length += iXmlNodeName.length();
            if(tmpele.length()==length)
                tmpele.clear();
            
            output += tmpele;
        }
        else
        {
            if(iContent.length()==0)
            {
//                output.append(GlobalNAskew);
//                output.append(GlobalBigFlag);
                tmpele.append(GlobalNAskew);    // tmpcot + /
                tmpele.append(GlobalBigFlag);   // tmpcot + >
                
                int length = 3;
                length += iXmlNodeName.length();
                if(tmpele.length()==length)
                    tmpele.clear();
                
                output.append(tmpele);
            }
            else
            {
//                output.append(GlobalBigFlag);
//                output.append(GlobalNewLine);
//                output.append(iContent);
//                output.append(GlobalNewLine);
//                for(int i=0; i<iEleCursor.s_Vdepth-1; i++)
//                    output.append(GlobalTab);
//                output.append(GlobalSmallFlag);
//                output.append(GlobalNAskew);
//                output.append(iXmlNodeName);
//                output.append(GlobalBigFlag);
                
                tmpele.append(GlobalBigFlag);
                tmpele.append(GlobalNewLine);
                tmpele.append(iContent);
                tmpele.append(GlobalNewLine);
                for(int i=0; i<iEleCursor.s_Vdepth-1; i++)
                    tmpele.append(GlobalTab);
                tmpele.append(GlobalSmallFlag);
                tmpele.append(GlobalNAskew);
                tmpele.append(iXmlNodeName);
                tmpele.append(GlobalBigFlag);
                
                int length = 10;
                length += iEleCursor.s_Vdepth-1;
                length += 2*iXmlNodeName.length();
                if(tmpele.length()==length)
                    tmpele.clear();
                
                output += tmpele;
            }
        }
    }
    else
    {
        if(iChildGroup!=NULL)
            output += ((XEasyXmlGroup*)iChildGroup)->GenerateXml();
    }

    return output;
}

// XEasyXmlAtt

XEasyXmlAtt::XEasyXmlAtt()
{
    iXmlNodeType = NT_XmlAtt;
}

XEasyXmlAtt::XEasyXmlAtt(XEasyXmlBase* aParent)
{
    iXmlNodeType = NT_XmlAtt;

    iParentXmlNode = aParent;

    if(iParentXmlNode)
    {
        iEleCursor.s_Vdepth = iParentXmlNode->VDepth()+1;
    }
}

XEasyXmlAtt::~XEasyXmlAtt()
{
}

void XEasyXmlAtt::Process(QString aMsg)
{
    // AttName 位置 和 AttValue 位置
    int EqualPos = 0;
    int flagstart = 0;
    int flagend  = 0;

    EqualPos = aMsg.indexOf(GlobalEqualTo);
    flagstart = aMsg.indexOf(GlobalDoubleQuotationMarks, EqualPos+1);
    flagend = aMsg.indexOf(GlobalDoubleQuotationMarks, flagstart+1);

    if(EqualPos==-1 || flagstart==-1 || flagend==-1)
        return;

    iXmlNodeName = aMsg.mid(0, EqualPos);
    iAttValue = aMsg.mid(flagstart+1, flagend-flagstart-1);
}

QString XEasyXmlAtt::GenerateXml()
{
    QString output;

    if(iXmlNodeName.length()==0 || iAttValue.length()==0)
    {
        return output;
    }
    else
    {
        output.append(iXmlNodeName);
        output.append(GlobalEqualTo);
        output.append(GlobalDoubleQuotationMarks);
        output.append(iAttValue);
        output.append(GlobalDoubleQuotationMarks);
        output.append(GlobalBlank);
    }
    
    return output;
}

// XEasyXmlGroup
XEasyXmlGroup::XEasyXmlGroup()
{
    iXmlNodeType = NT_XmlEleGroup;

    iFirstChildNode = NULL;
    iLastChildNode = NULL;
    iChildCount = 0;
}

XEasyXmlGroup::XEasyXmlGroup(XEasyXmlBase* aParent)
{
    iXmlNodeType = NT_XmlEleGroup;

    iFirstChildNode = NULL;
    iLastChildNode = NULL;
    iChildCount = 0;

    iParentXmlNode = aParent;
    if(aParent!=NULL)
    {
        iEleCursor.s_Vdepth = aParent->VDepth();
    }
}

XEasyXmlGroup::~XEasyXmlGroup()
{
    Reset();
}

void XEasyXmlGroup::AddChild(XEasyXmlBase *aChild)
{
    // 子节点为空，啥也不做
    if(aChild==NULL)
        return;

    // 如果是第一个子节点，FirstChildNode指向此节点
    aChild->SetParentNode(this->ParentNode());
    aChild->SetVDepth(iEleCursor.s_Vdepth+1);
    aChild->SetHDepth(iChildCount);
    if(iChildCount==0)
    {
        iFirstChildNode = aChild;
        iLastChildNode = aChild;
    }
    else
    {
        iLastChildNode->SetNextBrotherNode(aChild);
        aChild->SetPrevBrotherNode(iLastChildNode);
        iLastChildNode = aChild;
    }
    iChildCount++;
}

void XEasyXmlGroup::DelChild(XEasyXmlBase *aChild)
{
    // aChild是否有效
    if(!aChild)
        return;

    // 查找aChild
    XEasyXmlBase* tmp = iFirstChildNode;
    for(int i=0; i<iChildCount; i++)
    {
        if(tmp==aChild)
        {
            if(tmp->NextBrotherNode()!=NULL)
                tmp->NextBrotherNode()->SetPrevBrotherNode(tmp->PrevBrotherNode());
            if(tmp->PrevBrotherNode()!=NULL)
                tmp->PrevBrotherNode()->SetNextBrotherNode(tmp->NextBrotherNode());
            if(tmp==iFirstChildNode)
                iFirstChildNode = tmp->NextBrotherNode();
            if(tmp==iLastChildNode)
                iLastChildNode = tmp->PrevBrotherNode();

            SAFE_DEL(tmp);
            iChildCount--;

            return;
        }
        else
        {
            tmp = tmp->NextBrotherNode();
        }
    }
}

void XEasyXmlGroup::Reset()
{
    int count = iChildCount;
    for(int i=0; i<count; i++)
        DelChild(iFirstChildNode);
}

QString XEasyXmlGroup::Content(QString aEleName)
{
    QString value;

    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp!=NULL)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
            value = ((XEasyXmlEle*)tmp)->Content(aEleName);
        if(value.length()!=0)
            break;

        tmp = tmp->NextBrotherNode();
    }

    return value;

}

bool XEasyXmlGroup::SetContent(QString aEleName, QString aContent)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp!=NULL)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
            value = ((XEasyXmlEle*)tmp)->SetContent(aEleName, aContent);
        if(value)
            break;

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

int XEasyXmlGroup::ChildCount(QString aEleName, QString aChildEleName)
{
    XEasyXmlBase* tmp = iFirstChildNode;
    int count = 0;

    while(tmp!=NULL)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            count = ((XEasyXmlEle*)tmp)->ChildCount(aEleName, aChildEleName);
        }
        tmp = tmp->NextBrotherNode();

        if(count!=0)
            break;
    }

    return count;
}

int XEasyXmlGroup::ChildCount_this(QString aChildEleName)
{
    int count = 0;

    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp!=NULL)
    {
        if(tmp->NodeName()==aChildEleName)
            count++;
        tmp = tmp->NextBrotherNode();
    }

    return count;
}

XEasyXmlBase* XEasyXmlGroup::FindChild(QString aEleName, QString aChildEleName, int aIndex)
{
    XEasyXmlBase* value = NULL;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp!=NULL)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            value = ((XEasyXmlEle*)tmp)->FindChild(aEleName, aChildEleName, aIndex);
        }
        if(value!=NULL)
            break;
        else
            tmp = tmp->NextBrotherNode();
    }

    return value;
}

XEasyXmlBase* XEasyXmlGroup::FindChild_this(QString aChildEleName, int aIndex)
{
    XEasyXmlBase* value = NULL;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp!=NULL)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aChildEleName)
            {
                if(aIndex>0)
                    aIndex--;
                else if(aIndex==0)
                {
                    value = tmp;
                    break;
                }
            }
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::HasEle(QString aParentName, QString aEleName)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aParentName)
            {
                value = ((XEasyXmlEle*)tmp)->HasEle_this(aEleName);
            }
            else
            {
                value = ((XEasyXmlEle*)tmp)->HasEle(aParentName, aEleName);
            }

            if(value)
                break;
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::HasEle_this(QString aEleName)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aEleName)
            {
                value = true;
                break;
            }
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::AddEle(QString aParentName, QString aEleName)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aParentName)
            {
                value = ((XEasyXmlEle*)tmp)->AddEle_this(aEleName);
            }
            else
            {
                value = ((XEasyXmlEle*)tmp)->AddEle(aParentName, aEleName);
            }

            if(value)
                break;
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::AddEle_this(QString aEleName)
{
    if(this->NodeType()==XEasyXmlBase::NT_XmlEleGroup)
    {
        XEasyXmlBase* tmp = new XEasyXmlEle(this);
        tmp->SetNodeName(aEleName);
        this->AddChild(tmp);
    }

    return true;
    // aEleName可能未添加成功，但事件已成功消耗，故返回true
}

bool XEasyXmlGroup::DelEle(QString aParentName, QString aEleName)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aParentName)
            {
                value = ((XEasyXmlEle*)tmp)->DelEle_this(aEleName);
            }
            else
            {
                value = ((XEasyXmlEle*)tmp)->DelEle(aParentName, aEleName);
            }
            if(value)
                break;
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::DelEle_this(QString aEleName)
{
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aEleName)
            {
                this->DelChild(tmp);
                break;
            }
        }

        tmp = tmp->NextBrotherNode();
    }

    return true;
    // aEleName可能未删除成功，但事件成功消耗，返回true
}

bool XEasyXmlGroup::HasAtt(QString aEleName, QString aAttName)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            value = ((XEasyXmlEle*)tmp)->HasAtt(aEleName, aAttName);
        }
        if(value)
            break;

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::HasAtt_this(QString aAttName)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlAtt)
        {
            if(tmp->NodeName()==aAttName)
            {
                value = true;
                break;
            }
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::HasAttI(QString aEleName, QString aAttName, int aIndex)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aEleName)
            {
                bool value_1 = ((XEasyXmlEle*)tmp)->HasAtt_this(aAttName);
                if(aIndex>0)
                    aIndex--;
                else if(aIndex==0)
                {
                    value = value_1;
                    break;
                }
            }
            else
            {
                value = ((XEasyXmlEle*)tmp)->HasAttI(aEleName, aAttName, aIndex);
            }
            if(value)
                break;
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::AddAtt(QString aEleName, QString aAttName)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aEleName)
            {
                value = ((XEasyXmlEle*)tmp)->AddAtt_this(aAttName);
                break;
            }
            else
            {
                value = ((XEasyXmlEle*)tmp)->AddAtt(aEleName, aAttName);
            }
            if(value)
                break;
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::AddAtt(QString aEleName, QString aAttName, QString aAttValue)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aEleName)
            {
                value = ((XEasyXmlEle*)tmp)->AddAtt_this(aAttName, aAttValue);
            }
            else
            {
                value = ((XEasyXmlEle*)tmp)->AddAtt(aEleName, aAttName, aAttValue);
            }
            if(value)
                break;
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::AddAtt_this(QString aAttName)
{
    if(this->NodeType()==XEasyXmlBase::NT_XmlAttGroup)
    {
        XEasyXmlBase* tmp = new XEasyXmlAtt(this);
        tmp->SetNodeName(aAttName);
        this->AddChild(tmp);
    }

    return true;
}

bool XEasyXmlGroup::AddAtt_this(QString aAttName, QString aAttValue)
{
    if(this->NodeType()==XEasyXmlBase::NT_XmlAttGroup)
    {
         XEasyXmlBase* tmp = new XEasyXmlAtt(this);
         tmp->SetNodeName(aAttName);
         ((XEasyXmlAtt*)tmp)->SetAttValue(aAttValue);
         this->AddChild(tmp);
    }

    return true;
}

bool XEasyXmlGroup::AddAttI(QString aEleName, QString aAttName, int aIndex)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aEleName)
            {
                if(aIndex>0)
                    aIndex--;
                else if(aIndex==0)
                {
                    value = ((XEasyXmlEle*)tmp)->AddAtt_this(aAttName);
                    break;
                }
            }
            else
            {
                value = ((XEasyXmlEle*)tmp)->AddAttI(aEleName, aAttName, aIndex);
            }
            if(value)
                break;
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::AddAttI(QString aEleName, QString aAttName, QString aAttValue, int aIndex)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aEleName)
            {
                if(aIndex>0)
                    aIndex--;
                else if(aIndex==0)
                {
                    value = ((XEasyXmlEle*)tmp)->AddAtt_this(aAttName, aAttValue);
                    break;
                }
            }
            else
            {
                value = ((XEasyXmlEle*)tmp)->AddAttI(aEleName, aAttName, aAttValue, aIndex);
            }
            if(value)
                break;
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::DelAtt(QString aEleName, QString aAttName)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aEleName)
            {
                value = ((XEasyXmlEle*)tmp)->DelAtt_this(aAttName);
                break;
            }
            else
            {
                value = ((XEasyXmlEle*)tmp)->DelAtt(aEleName, aAttName);
            }
            if(value)
                break;
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::DelAtt_this(QString aAttName)
{
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlAtt)
        {
            if(tmp->NodeName()==aAttName)
            {
                this->DelChild(tmp);
                break;
            }
        }

        tmp = tmp->NextBrotherNode();
    }

    return true;
}

bool XEasyXmlGroup::DelAttI(QString aEleName, QString aAttName, int aIndex)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aEleName)
            {
                if(aIndex>0)
                    aIndex--;
                else if(aIndex==0)
                {
                    value = ((XEasyXmlEle*)tmp)->DelAtt_this(aAttName);
                    break;
                }
            }
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::ModifyAtt(QString aEleName, QString aAttName, QString aAttNewValue)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aEleName)
            {
                value = ((XEasyXmlEle*)tmp)->ModifyAtt_this(aAttName, aAttNewValue);
                break;
            }
            else
            {
                value = ((XEasyXmlEle*)tmp)->ModifyAtt(aEleName, aAttName, aAttNewValue);
            }
            if(value)
                break;
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

bool XEasyXmlGroup::ModifyAtt_this(QString aAttName, QString aAttNewValue)
{
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlAtt)
        {
            if(tmp->NodeName()==aAttName)
            {
                ((XEasyXmlAtt*)tmp)->SetAttValue(aAttNewValue);
                break;
            }
        }

        tmp = tmp->NextBrotherNode();
    }

    return true;
}

bool XEasyXmlGroup::ModifyAttI(QString aEleName, QString aAttName, QString aAttNewValue, int aIndex)
{
    bool value = false;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aEleName)
            {
                if(aIndex>0)
                    aIndex--;
                else if(aIndex==0)
                {
                    value = ((XEasyXmlEle*)tmp)->ModifyAtt_this(aAttName, aAttNewValue);
                    break;
                }
            }
            else
            {
                value = ((XEasyXmlEle*)tmp)->ModifyAttI(aEleName, aAttName, aAttNewValue, aIndex);
            }
            if(value)
                break;
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

QString XEasyXmlGroup::FindAtt(QString aEleName, QString aAttName)
{
    QString value;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp!=NULL)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            value = ((XEasyXmlEle*)tmp)->FindAtt(aEleName, aAttName);
        }
        if(value.length()!=0)
            break;

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

QString XEasyXmlGroup::FindAtt_this(QString aAttName)
{
    QString value;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp!=NULL)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlAtt)
        {
            if(tmp->NodeName()==aAttName)
            {
                value = ((XEasyXmlAtt*)tmp)->AttValue();
            }
        }
        if(value.length()!=0)
            break;
        tmp = tmp->NextBrotherNode();
    }

    return value;
}

QString XEasyXmlGroup::FindAttI(QString aEleName, QString aAttName, int aIndex)
{
    QString value;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp!=NULL)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            if(tmp->NodeName()==aEleName)
            {
                if(aIndex>0)
                {
                    aIndex--;
                }
                else if(aIndex==0)
                {
                    value = ((XEasyXmlEle*)tmp)->FindAtt_this(aAttName);
                    break;
                }
            }
            else
                    value = ((XEasyXmlEle*)tmp)->FindAttI(aEleName, aAttName, aIndex);
        }

        if(value.length()!=0)
            break;

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

QString XEasyXmlGroup::FindContent(QString aEleName, QString aAttName, QString aAttValue)
{
    QString value;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp!=NULL)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlEle)
        {
            value = ((XEasyXmlEle*)tmp)->FindContent(aEleName, aAttName, aAttValue);
        }
        if(value.length()!=0)
            break;
        tmp = tmp->NextBrotherNode();
    }

    return value;
}

QString XEasyXmlGroup::FindContent_this(QString aAttName, QString aAttValue)
{
    QString value;
    XEasyXmlBase* tmp = iFirstChildNode;

    while(tmp!=NULL)
    {
        if(tmp->NodeType()==XEasyXmlBase::NT_XmlAtt)
        {
            if(tmp->NodeName()==aAttName && ((XEasyXmlAtt*)tmp)->AttValue()==aAttValue)
            {
                value = ((XEasyXmlEle*)(tmp->ParentNode()))->Content();
                break;
            }
        }

        tmp = tmp->NextBrotherNode();
    }

    return value;
}

QString XEasyXmlGroup::GenerateXml()
{
    QString output;
    if(this->NodeType()==NT_XmlAttGroup)
    {
        XEasyXmlBase* p = iFirstChildNode;
        for(int i=0; i<iChildCount; i++)
        {
            output += ((XEasyXmlAtt*)p)->GenerateXml();
            p = p->NextBrotherNode();
        }
    }
    else if(this->NodeType()==NT_XmlEleGroup)
    {
        XEasyXmlBase* p = iFirstChildNode;
        for(int i=0; i<iChildCount; i++)
        {
            QString tmp;
            QString tmp1;
            if(p->NodeType()==XEasyXmlBase::NT_XmlEle)
            {
//                output.append(GlobalNewLine);
                
                tmp.append(GlobalNewLine);
                
                for(int j=0; j<p->VDepth()-1; j++)
                    tmp.append(GlobalTab);
//                    output.append(GlobalTab);

                
                
//                output+=((XEasyXmlEle*)p)->GenerateXml();
                tmp+=((XEasyXmlEle*)p)->GenerateXml();
                if(tmp.length()==(2+(p->VDepth()-1)))
                    tmp.clear();
                
                output+=tmp;
            }

            p = p->NextBrotherNode();
        }
    }

    return output;
}

// 未知
XEasyXmlUnknown::XEasyXmlUnknown()
{
    iXmlNodeType = NT_XmlUnknown;
}

XEasyXmlUnknown::XEasyXmlUnknown(XEasyXmlBase *aParent)
{
    iXmlNodeType = NT_XmlUnknown;

    iParentXmlNode = aParent;
    if(iParentXmlNode!=NULL)
    {
        iEleCursor.s_Vdepth = iParentXmlNode->VDepth()+1;
    }
}

XEasyXmlUnknown::~XEasyXmlUnknown()
{}

void XEasyXmlUnknown::Process(QString aMsg)
{
    iMsg = aMsg;
}

QString XEasyXmlUnknown::GenerateXml()
{
    QString output;

    if(iMsg.length()==0)
        return output;

    output.append(iMsg);

    return output;
}

// 注释
XEasyXmlNote::XEasyXmlNote()
{
    iXmlNodeType = NT_XmlNote;
}

XEasyXmlNote::XEasyXmlNote(XEasyXmlBase *aParent)
{
    iXmlNodeType = NT_XmlNote;

    iParentXmlNode = aParent;
    if(aParent!=NULL)
    {
        iEleCursor.s_Vdepth = aParent->VDepth()+1;
    }
}

XEasyXmlNote::~XEasyXmlNote()
{}

void XEasyXmlNote::Process(QString aMsg)
{
    // 直接查找<!--跟-->位置
    int flagstart = 0;
    int flagend = 0;
    flagstart = aMsg.indexOf(XmlNoteMarkStart);
    flagend = aMsg.indexOf(XmlNoteMarkEnd, flagstart+1);
    if(flagstart==-1 || flagend==-1)
        return;

    iMsg = aMsg.mid(flagstart+4, flagend-flagstart-4);
}

QString XEasyXmlNote::GenerateXml()
{
    QString output;

    if(iMsg.length()==0)
        return output;

    output.append(XmlNoteMarkStart);
    output.append(iMsg);
    output.append(XmlNoteMarkEnd);

    return output;
}

// Xml Header
XEasyXmlHeader::XEasyXmlHeader()
{}

XEasyXmlHeader::~XEasyXmlHeader()
{}

void XEasyXmlHeader::Process(QString aMsg)
{
    int flagstart = 0;
    int flagend = 0;
    flagstart = aMsg.indexOf(GlobalDoubleQuotationMarks);
    flagend = aMsg.indexOf(GlobalDoubleQuotationMarks, flagstart+1);
    if(flagstart==-1 || flagend==-1)
        return;

    iVersion = aMsg.mid(flagstart+1, flagend-flagstart-1);

    flagstart = aMsg.indexOf(GlobalDoubleQuotationMarks, flagend+1);
    flagend = aMsg.indexOf(GlobalDoubleQuotationMarks, flagstart+1);
    if(flagstart==-1 || flagend==-1)
        return;

    iEncoding = aMsg.mid(flagstart+1, flagend-flagstart-1);
}

QString XEasyXmlHeader::GenerateXml()
{
    QString output;

    if(iVersion.length()==0 || iEncoding.length()==0)
        return output;

    output.append(XmlHeaderMarkStart);
    output.append(XmlHeaderVersion);
    output.append(GlobalEqualTo);
    output.append(GlobalDoubleQuotationMarks);
    output.append(iVersion);
    output.append(GlobalDoubleQuotationMarks);

    output.append(GlobalBlank);

    output.append(XmlHeaderEncoding);
    output.append(GlobalEqualTo);
    output.append(GlobalDoubleQuotationMarks);
    output.append(iEncoding);
    output.append(GlobalDoubleQuotationMarks);

    output.append(GlobalBlank);
    output.append(XmlHeaderMarkEnd);

    return output;
}

// Xml Dtd
XEasyXmlDtd::XEasyXmlDtd()
{}

XEasyXmlDtd::~XEasyXmlDtd()
{}

void XEasyXmlDtd::Process(QString aMsg)
{
    int flagstart = 0;
    int flagend = 0;

    flagstart = aMsg.indexOf(GlobalBlank);
    flagend = aMsg.indexOf(GlobalBlank, flagstart+1);
    if(flagstart==-1 || flagend==-1)
        return;

    iRootName = aMsg.mid(flagstart+1, flagend-flagstart-1);

    flagstart = aMsg.indexOf(GlobalDoubleQuotationMarks, flagend+1);
    flagend = aMsg.indexOf(GlobalDoubleQuotationMarks, flagstart+1);
    if(flagstart==-1 || flagend==-1)
        return;

    iLocation = aMsg.mid(flagstart+1, flagend-flagstart-1);
}

QString XEasyXmlDtd::GenerateXml()
{
    QString output;
    if(iRootName.length()==0 || iLocation.length()==0)
        return output;

    output.append(GlobalNewLine);
    output.append(XmlDtdMarkStart);
    output.append(XmlDtdDocType);
    output.append(GlobalBlank);
    output.append(iRootName);
    output.append(GlobalBlank);
    output.append(XmlDtdSystem);
    output.append(GlobalBlank);
    output.append(GlobalDoubleQuotationMarks);
    output.append(iLocation);
    output.append(GlobalDoubleQuotationMarks);
    output.append(XmlDtdMarkEnd);

    return output;
}

// Xml Reader
XEasyXml::XEasyXml()
{
    iXmlHeader = new XEasyXmlHeader;
    iDtd = new XEasyXmlDtd;
    iRoot = new XEasyXmlEle;
    iXmlCursor = iRoot;
    iHelpStart = false;
//    iDTDReader = NULL;
}

XEasyXml::~XEasyXml()
{
    SAFE_DEL(iXmlHeader);
    SAFE_DEL(iDtd);
    SAFE_DEL(iRoot);
//    SAFE_DEL(iDTDReader);
}

void XEasyXml::OpenXml(const QString &aPath)
{
    if(aPath.length()!=0)
    {
        QFile file(aPath);

        QString encode;
        
        if(file.open(QIODevice::ReadOnly))        
        {
            QByteArray by = file.read(3);
            QByteArray by1;
            by1.resize(3);
            by1[0] = 0xEF;
            by1[1] = 0xBB;
            by1[2] = 0xBF;
            if(by.length()<3)
            {
                if(by[0]==by1[0] && by[1]==by1[1])
                    encode = "UTF-8";
            }
            else
            {
                if(by[0]==by1[0] && by[1]==by1[1] && by[2]==by1[2])
                    encode = "UTF-8";
            }
            
            file.close();
        }
        
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            if(encode=="UTF-8")
                stream.setCodec("UTF-8");
            

            while(!stream.atEnd())
            {
                // readline
                iMsg = stream.readLine();

                // clean
                CleanString();

                // switch type
                switch(SwitchType())
                {
                case 1: // Xml Header
                    {
                        if(iXmlHeader)
                            iXmlHeader->Process(iMsg);
                    }
                    break;
                case 2: // DTD
                    {
                        if(iXmlHeader)
                            iDtd->Process(iMsg);
                    }
                    break;
                case 3: // Ele
                    {
                        if(iMsg.mid(1,1)!=GlobalNAskew)
                        {
                            XEasyXmlBase* ele = new XEasyXmlEle(iXmlCursor);
                            ((XEasyXmlEle*)iXmlCursor)->AddEle(ele);
                            iXmlCursor = ele;
                            ((XEasyXmlEle*)ele)->Process(iMsg, iXmlCursor);
                        }
                        else
                        {
                            QString OverNodeName;
                            int flagstart = 0;
                            int flagend = 0;

                            flagstart = iMsg.indexOf(GlobalNAskew);
                            flagend = iMsg.indexOf(GlobalBigFlag);
                            if(flagstart==-1 || flagend==-1)
                                break;

                            OverNodeName = iMsg.mid(flagstart+1, flagend-flagstart-1);

                            if(iXmlCursor->NodeName()==OverNodeName)
                            {
                                ((XEasyXmlEle*)iXmlCursor)->SetOver(true);
                                iXmlCursor = iXmlCursor->ParentNode();
                            }
                        }
                    }
                    break;
                case 4: // Note
                    {
                        XEasyXmlBase* note = new XEasyXmlNote(iXmlCursor);
                        ((XEasyXmlEle*)iXmlCursor)->AddEle(note);
                        ((XEasyXmlNote*)note)->Process(iMsg);
                    }
                    break;
                case 5: // Unknown
                    {
                        XEasyXmlBase* unknown = new XEasyXmlUnknown(iXmlCursor);
                        ((XEasyXmlEle*)iXmlCursor)->AddEle(unknown);
                        ((XEasyXmlUnknown*)unknown)->Process(iMsg);
                    }
                    break;
                case 6:
                    {
                        QString value;
                        value = this->Content("txt_define");
                        if(value.length()!=0)
                            value += GlobalNewLine;
                        value += iHelpMsg;
                        this->SetContent("txt_define", value);
                    }
                    break;
                default:
                    break;
                }
            }

            file.close();
        }
    }
}

QString XEasyXml::GenerateXml()
{
    QString output;

    if(iXmlHeader)
        output += iXmlHeader->GenerateXml();
    if(iDtd)
        output += iDtd->GenerateXml();
    if(iRoot)
        output += ((XEasyXmlEle*)iRoot)->GenerateXml();

    return output;
}

QString XEasyXml::Content(QString aEleName) const
{
    QString value;
    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->Content(aEleName);
    }

    return value;
}

bool XEasyXml::SetContent(QString aEleName, QString aContent)
{
    bool value = false;

    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->SetContent(aEleName, aContent);
    }

    return value;
}

bool XEasyXml::HasEle(QString aParentName, QString aEleName)
{
    bool value = false;

    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->HasEle(aParentName, aEleName);
    }

    return value;
}

bool XEasyXml::AddEle(QString aParentName, QString aEleName)
{
    bool value = false;
    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->AddEle(aParentName, aEleName);
    }

    return value;
}

bool XEasyXml::DelEle(QString aParentName, QString aEleName)
{
    bool value = false;
    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->DelEle(aParentName, aEleName);
    }

    return value;
}

bool XEasyXml::HasAtt(QString aEleName, QString aAttName)
{
    bool value = false;

    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->HasAtt(aEleName, aAttName);
    }

    return value;
}

bool XEasyXml::HasAttI(QString aEleName, QString aAttName, int aIndex)
{
    bool value = false;

    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->HasAttI(aEleName, aAttName, aIndex);
    }

    return value;
}

bool XEasyXml::AddAtt(QString aEleName, QString aAttName)
{
    bool value = false;

    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->AddAtt(aEleName, aAttName);
    }

    return value;
}

bool XEasyXml::AddAtt(QString aEleName, QString aAttName, QString aAttValue)
{
    bool value = false;

    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->AddAtt(aEleName, aAttName, aAttValue);
    }

    return value;
}

bool XEasyXml::AddAttI(QString aEleName, QString aAttName, int aIndex)
{
    bool value = false;

    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->AddAttI(aEleName, aAttName, aIndex);
    }

    return value;
}

bool XEasyXml::AddAttI(QString aEleName, QString aAttName, QString aAttValue, int aIndex)
{
    bool value = false;

    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->AddAttI(aEleName, aAttName, aAttValue, aIndex);
    }

    return value;
}

bool XEasyXml::DelAtt(QString aEleName, QString aAttName)
{
    bool value = false;

    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->DelAtt(aEleName, aAttName);
    }

    return value;
}

bool XEasyXml::DelAttI(QString aEleName, QString aAttName, int aIndex)
{
    bool value = false;

    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->DelAttI(aEleName, aAttName, aIndex);
    }

    return value;
}

bool XEasyXml::ModifyAtt(QString aEleName, QString aAttName, QString aAttNewValue)
{
    bool value = false;

    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->ModifyAtt(aEleName, aAttName, aAttNewValue);
    }

    return value;
}

bool XEasyXml::ModifyAttI(QString aEleName, QString aAttName, QString aAttNewValue, int aIndex)
{
    bool value = false;

    if(iRoot)
    {
        value = ((XEasyXmlEle*)iRoot)->ModifyAttI(aEleName, aAttName, aAttNewValue, aIndex);
    }

    return value;
}

//void XEasyXml::Process()
//{}

QString XEasyXml::FindAtt(QString aEleName, QString aAttName)
{
    QString value;

    if(iRoot!=NULL)
    {
        value = ((XEasyXmlEle*)iRoot)->FindAtt(aEleName, aAttName);
    }

    return value;
}

QString XEasyXml::FindAttI(QString aEleName, QString aAttName, int aIndex)
{
    QString value;

    if(iRoot!=NULL)
    {
        value = ((XEasyXmlEle*)iRoot)->FindAttI(aEleName, aAttName, aIndex);
    }

    return value;
}

QString XEasyXml::FindContent(QString aEleName, QString aAttName, QString aAttValue)
{
    QString value;

    if(iRoot!=NULL)
    {
        value = ((XEasyXmlEle*)iRoot)->FindContent(aEleName, aAttName, aAttValue);
    }

    return value;
}

int XEasyXml::ChildCount(QString aEleName, QString aChildEleName)
{
    int count = 0;

    if(iRoot!=NULL)
    {
        count = ((XEasyXmlEle*)iRoot)->ChildCount(aEleName, aChildEleName);
    }

    return count;
}

XEasyXmlBase* XEasyXml::FindChild(QString aEleName, QString aChildEleName, int aIndex)
{
    XEasyXmlBase* value = NULL;

    if(iRoot!=NULL)
    {
        value = ((XEasyXmlEle*)iRoot)->FindChild(aEleName, aChildEleName, aIndex);
    }

    return value;
}

int XEasyXml::SwitchType()
{
    if(iMsg.length()!=0)
    {
        if(iMsg.mid(0, 1)==GlobalSmallFlag)
        {
            if(iMsg.mid(0, 2)==XmlHeaderMarkStart)
                return 1;
            else if(iMsg.mid(0, 2)==XmlDtdMarkStart)
            {
                if(iMsg.mid(2, 2)==XmlNoteFlag)
                    return 4;
                else
                    return 2;
            }
            else
            {
                if(iMsg==QString("<txt_define>"))
                {
                    iHelpStart = true;
                    return 3;
                }
                else if(iMsg==QString("</txt_define>"))
                {
                    iHelpStart = false;
                    return 3;
                }
                else
                {
                    if(iHelpStart)
                        return 6;
                    else
                        return 3;
                }
            }
        }
        else
        {
            if(iHelpStart)
                return 6;
            else
                return 5;
        }
    }
    else
    {
        if(iHelpStart)
            return 6;
        else
            return -1;
    }
}

void XEasyXml::CleanString()
{
    int bufstart = 0;
    if(iHelpStart)
        iHelpMsg = iMsg;

    while(iMsg.mid(0, 1)==GlobalTab || iMsg.mid(0, 1)==GlobalTabReal)
    {
        iMsg = iMsg.mid(bufstart+1);
    }
}
