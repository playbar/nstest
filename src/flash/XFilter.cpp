// _XFilter.cpp: implementation of the _XFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XFilter.h"
#include "splayer.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XFilter::_XFilter()
{
   m_pNext=XNULL;
   nColor=0;
   //m_pBlur=XNULL;
}

_XFilter::~_XFilter()
{

}

_XFilter* _XFilter::CreateFilter(SParser *pStream,XSWFPlayer*player)
{
	XU8 id=pStream->GetByte();
	//if(!pStream->ReadByte(id)) return XNULL;
	if(id>XFilterGradientBevel) return XNULL;
	_XFilter*pFilter=player->CreateFilter();//new _XFilter;
	pFilter->m_nFilterID=id;
//#if (__CORE_VERSION__>=0x02076000)
	pFilter->nData=0;
//#endif
#ifdef _WINEMU
	/*const char* _strFilterName[]=
	{
		"DropShadow",
		"Blur",
		"Bevel",
		"GradientGrow",
		"Convolution",
		"ColorMatrix",
		"GradientBevel",
	};
	XString16 s("Waring:Filter ");
	s+=XString16(_strFilterName[id]);
	s+=XString16(" exist but not support!");
	player->Trace(s);*/
#endif
	pFilter->ReadFromStream(pStream);
	return pFilter;
}


#define CASE(VV)\
	case XFilter##VV:\
		p##VV=(_XFilter##VV*)(pStream->ReadAddr());\
		pStream->SkipBytes(sizeof(_XFilter##VV));break;

XU32 _XFilter::GetColor()
{
	/*if(!pGlow)
		return nColor?nColor:1;*/
	switch(m_nFilterID)
	{
		case XFilterBevel: 
			 return 1;//this->pBevel->shadowColor;
		case XFilterDropShadow:		
			 return 1;//this->pDropShadow->color;
		case XFilterGlow:
			 return 1;//this->pGlow->color;
		case XFilterGradientGlow:
			 //return this->pGradientGlow
		case XFilterGradientBevel:
			 return 1;
	}
	return 0;
}

XBOOL _XFilter::ReadFromStream(SParser*pStream)
{
	int i=0;
	XU32 temptt=0;
	float tempf=0.0;
	switch(m_nFilterID)
	{
	default:return XFALSE;
	case XFilterDropShadow:
		 /*pGlow->color=pStream->GetDWord();
		 pGlow->blurX=pStream->GetDWord();
		 pGlow->blurY=pStream->GetDWord();
		 pGlow->strength=pStream->GetWord();
		 pGlow->flags=pStream->GetByte();*/
		 //pDropShadow=(_XFilterDropShadow*)pStream->ReadAddr();
		 //pGlow->color=pStream->GetDWord();
		 pDropShadow.color[0]=pStream->GetByte()/255.0;//b
		 pDropShadow.color[1]=pStream->GetByte()/255.0;//g
		 pDropShadow.color[2]=pStream->GetByte()/255.0;//r
		 pDropShadow.color[3]=pStream->GetByte()/255.0;//a
		 pDropShadow.blurX=pStream->GetFixedtoFloat();
		 pDropShadow.blurY=pStream->GetFixedtoFloat();
		 pDropShadow.angle=pStream->GetDWord()/1143.81;
		 pDropShadow.distance=pStream->GetFixedtoFloat();
		 pDropShadow.strength=pStream->GetFixedtoFloat8();//
		 temptt=pStream->GetByte();
		 //pDropShadow.konckout=(temptt&64)==64?1:0;
		 filter_konckout=(temptt&64)==64?1:0;
		 //pStream->SkipBytes(3);//total_size 23
		 break;
	case XFilterBlur:
		 //pBlur=(_XFilterBlur*)pStream->ReadAddr();
		// pBlur->
		 pBlur.blurX=pStream->GetFixedtoFloat();
		 pBlur.blurY=pStream->GetFixedtoFloat();		 
		 pStream->SkipBytes(1);//total_size 9
		 //pBlur->color=pStream-
		 break;
	case XFilterGlow:
		 //pGlow=(_XFilterGlow*)pStream->ReadAddr();
		 //pGlow->color=pStream->GetDWord();
		 pGlow.color[0]=pStream->GetByte()/255.0;//b
		 pGlow.color[1]=pStream->GetByte()/255.0;//g
		 pGlow.color[2]=pStream->GetByte()/255.0;//r
		 pGlow.color[3]=pStream->GetByte()/255.0;//a
		 pGlow.blurX=pStream->GetFixedtoFloat();
		 pGlow.blurY=pStream->GetFixedtoFloat();
		 pGlow.strength=pStream->GetFixedtoFloat8();//
		 temptt=pStream->GetByte();
		 //pGlow.konckout=(temptt&64==64?1:0);
		 filter_konckout=(temptt&64)==64?1:0;
		 //pStream->SkipBytes(1);//total_size 15
		 break;
	case XFilterBevel:
		 //pBevel=(_XFilterBevel*)pStream->ReadAddr();
		//pBevel->shadowColor=pStream->GetDWord();
		 pBevel.shadowColor[0]=pStream->GetByte()/255.0;//b
		 pBevel.shadowColor[1]=pStream->GetByte()/255.0;//g
		 pBevel.shadowColor[2]=pStream->GetByte()/255.0;//r
		 pBevel.shadowColor[3]=pStream->GetByte()/255.0;//a
		 pBevel.hiColor[0]=pStream->GetByte()/255.0;//b
		 pBevel.hiColor[1]=pStream->GetByte()/255.0;//g
		 pBevel.hiColor[2]=pStream->GetByte()/255.0;//r
		 pBevel.hiColor[3]=pStream->GetByte()/255.0;//a
		 pBevel.blurX=pStream->GetFixedtoFloat();
		 pBevel.blurY=pStream->GetFixedtoFloat();
		 pBevel.angle=pStream->GetDWord()/1143.81;
		 pBevel.distance=pStream->GetFixedtoFloat();
		 pBevel.strength=pStream->GetFixedtoFloat8();//
		 temptt=pStream->GetByte();
		 //pBevel.konckout=(temptt&64==64?1:0);
		 filter_konckout=(temptt&64)==64?1:0;
		 //pStream->SkipBytes(3);//total_size 27
		 break;
	case XFilterGradientBevel:
		 i=pStream->GetByte();
		 pGradientBevel.hiColor[0]=pStream->GetByte()/255.0;//b
		 pGradientBevel.hiColor[1]=pStream->GetByte()/255.0;//g
		 pGradientBevel.hiColor[2]=pStream->GetByte()/255.0;//r
		 pGradientBevel.hiColor[3]=pStream->GetByte()/255.0;//a
		 pStream->SkipBytes((i-2)*(sizeof(XU32)));
		 pGradientBevel.shadowColor[0]=pStream->GetByte()/255.0;//b
		 pGradientBevel.shadowColor[1]=pStream->GetByte()/255.0;//g
		 pGradientBevel.shadowColor[2]=pStream->GetByte()/255.0;//r
		 pGradientBevel.shadowColor[3]=pStream->GetByte()/255.0;//a
		 pStream->SkipBytes(i);
		 pGradientBevel.blurX=pStream->GetFixedtoFloat();
		 pGradientBevel.blurY=pStream->GetFixedtoFloat();
		 pGradientBevel.angle=pStream->GetDWord()/1143.81;
		 pGradientBevel.distance=pStream->GetFixedtoFloat();
		 pGradientBevel.strength=pStream->GetFixedtoFloat8();//
		 temptt=pStream->GetByte();
		 //pGradientBevel.konckout=(temptt&64==64?1:0);
		 filter_konckout=(temptt&64)==64?1:0;
		 //pStream->SkipBytes(3);//total_size i*5+20
		 break;
	case XFilterGradientGlow:
		 i=pStream->GetByte();
		 pStream->SkipBytes((i-1)*(sizeof(XU32)));
		 pGradientGlow.color[0]=pStream->GetByte()/255.0;//b
		 pGradientGlow.color[1]=pStream->GetByte()/255.0;//g
		 pGradientGlow.color[2]=pStream->GetByte()/255.0;//r
		 pGradientGlow.color[3]=pStream->GetByte()/255.0;//a
		 pStream->SkipBytes(i);
		 pGradientGlow.blurX=pStream->GetFixedtoFloat();
		 pGradientGlow.blurY=pStream->GetFixedtoFloat();
		 pGradientGlow.angle=pStream->GetDWord()/1143.81;
		 pGradientGlow.distance=pStream->GetFixedtoFloat();
		 pGradientGlow.strength=pStream->GetFixedtoFloat8();//
		 temptt=pStream->GetByte();
		 //pGradientGlow.konckout=(temptt&64==64?1:0);
		 filter_konckout=(temptt&64)==64?1:0;
		 //pStream->SkipBytes(1);//total_size i*5+20
		 /*pGradientGlow->colorNums=pStream->GetByte();
		 pGradientGlow->pGradColors=(XU32*)pStream->ReadAddr();
		 pStream->SkipBytes(pGradientGlow->colorNums*sizeof(XU32));
		 pGradientGlow->pGradRatios=pStream->ReadAddr();
		 pStream->SkipBytes(pGradientGlow->colorNums);
		 pGradientGlow->blurX=pStream->GetDWord();
		 pGradientGlow->blurY=pStream->GetDWord();
		 pGradientGlow->angle=pStream->GetDWord();
		 pGradientGlow->distance=pStream->GetDWord();
		 pGradientGlow->strength=pStream->GetWord();
		 pGradientGlow->flags=pStream->GetByte();*/
		 break;
	case XFilterConvolution:
		 //pConvolution->matrixX=pStream->GetByte();
		 //pConvolution->matrixY=pStream->GetByte();
		 //pConvolution->divisor=pStream->GetDWord();
		 //pConvolution->bias=pStream->GetDWord();
		 /*pConvolution=(_XFilterConvolution*)pStream->ReadAddr();
		 pStream->SkipBytes(10);
		 pConvolution->pMatrix=(float*)pStream->ReadAddr();
		 pStream->SkipBytes(pConvolution->matrixX*pConvolution->matrixY*sizeof(float));
		 pConvolution->defaultColor=pStream->GetDWord();
		 pConvolution->flag=pStream->GetByte();*/
		 break;
	case XFilterColorMatrix:
		 //pColorMatrix=(_XFilterColorMatrix*)pStream->ReadAddr();
		 for(i=0;i<20;i++)
		 {
			 if(i==4||i==9||i==14||i==19)
				pColorMatrix.fData[i]=(pStream->GetFloat32()/256);
			 else
				pColorMatrix.fData[i]=(pStream->GetFloat32());
		 }
		 //pStream->SkipBytes(sizeof(_XFilterColorMatrix));//total_size 80
		 break;
	/*CASE(DropShadow);
	CASE(Blur);
	CASE(Glow);
	CASE(Bevel);
	CASE(GradientGlow);
	CASE(Convolution);
	CASE(ColorMatrix);
	CASE(GradientBevel);	*/
	}
	return XTRUE;
}