// XHTMLFrame.cpp: implementation of the XHTMLFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLFrame.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XHTMLFrameset::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_LAYEROUT_CELLS:
	case XDO_LAYEROUT_CELL:
	case XDO_PAINT:
		 return 0;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLFrame::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_LAYEROUT_CELLS:
	case XDO_LAYEROUT_CELL:
	case XDO_PAINT:
		 return 0;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLIFrame::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_LAYEROUT_CELLS:
	case XDO_LAYEROUT_CELL:
	case XDO_PAINT:
		 return 0;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLNOFrames::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_LAYEROUT_CELLS:
	case XDO_LAYEROUT_CELL:
	case XDO_PAINT:
		 return 0;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}