#include "StdAfxflash.h"
#include "avmplus.h"
#include "stags.h"

using namespace avmplus;
#include "AS3BlendMode.h"
namespace avmshell{
BlendModeClass::BlendModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BlendModeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* BlendModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BlendModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
BlendModeObject::BlendModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}

Stringp BlendModeClass::getModeNameOf(int i)
{
	Stringp strMode=core()->kEmptyString;
	switch(i)
		{//more Safely
		case drawBlendAdd:
			 strMode=this->getSlotADD();break;
		case drawBlendAlpha://BlendModeClass::ALPHA:
			 strMode=this->getSlotALPHA();break;
		case drawBlendDarken://BlendModeClass::DARKEN:
			 strMode=this->getSlotDARKEN();break;
		case drawBlendDefrence://BlendModeClass::DIFFERENCE_FORSPACE:
			 strMode=this->getSlotDIFFERENCE_FORSPACE();break;
		case drawBlendErease://BlendModeClass::ERASE:
			 strMode=this->getSlotERASE();break;
		case drawBlendHardLight://BlendModeClass::HARDLIGHT:
			 strMode=this->getSlotHARDLIGHT();break;
		case drawBlendInvert://BlendModeClass::INVERT:
			 strMode=this->getSlotINVERT();break;
		case drawBlendLayer://BlendModeClass::LAYER:
			 strMode=this->getSlotLAYER();break;
		case drawBlendLighten://BlendModeClass::LIGHTEN:
			 strMode=this->getSlotLIGHTEN();break;
		case drawBlendMulty://BlendModeClass::MULTIPLY:
			 strMode=this->getSlotMULTIPLY();break;
		case drawBlendNormal1:
		case drawBlendNormal0://BlendModeClass::NORMAL:
			 strMode=this->getSlotNORMAL();break;
		case drawBlendOverlay://BlendModeClass::OVERLAY:
			 strMode=this->getSlotOVERLAY();break;
		case drawBlendScreen://BlendModeClass::SCREEN:
			 strMode=this->getSlotSCREEN();break;
		case drawBlendSubtrace://BlendModeClass::SUBTRACT:
			 strMode=this->getSlotSUBTRACT();break;
			 break;
		}
	return strMode;
}
int	    BlendModeClass::getModeIDOf(Stringp strMode)
{
		if(strMode->equals(this->getSlotADD()))
			return drawBlendAdd;
		else if(strMode->equals(getSlotALPHA()))
			return drawBlendAlpha;//BlendModeClass::ALPHA:
		else if(strMode->equals(getSlotDARKEN()))
			return drawBlendDarken;//BlendModeClass::DARKEN:
		else if(strMode->equals(this->getSlotDIFFERENCE_FORSPACE()))
			return drawBlendDefrence;//BlendModeClass::DIFFERENCE_FORSPACE:
		else if(strMode->equals(this->getSlotERASE()))
			return drawBlendErease;//BlendModeClass::ERASE:
		else if(strMode->equals(this->getSlotHARDLIGHT()))
			return drawBlendHardLight;//BlendModeClass::HARDLIGHT:
		else if(strMode->equals(this->getSlotINVERT()))
			return drawBlendInvert;//BlendModeClass::INVERT:
		else if(strMode->equals(this->getSlotLAYER()))
			return drawBlendLayer;//BlendModeClass::LAYER:
		else if(strMode->equals(this->getSlotLIGHTEN()))
			return drawBlendLighten;//BlendModeClass::LIGHTEN:
		else if(strMode->equals(this->getSlotMULTIPLY()))
			return drawBlendMulty;//BlendModeClass::MULTIPLY:
		else if(strMode->equals(this->getSlotOVERLAY()))			 
			return drawBlendOverlay;//BlendModeClass::OVERLAY:
		else if(strMode->equals(this->getSlotSCREEN()))
			return drawBlendScreen;//BlendModeClass::SCREEN:
		else if(strMode->equals(this->getSlotSUBTRACT()))
			return drawBlendSubtrace;//BlendModeClass::SUBTRACT:
		else
			return drawBlendNormal0;
	
}
//////////////////////////////////////////////////////////
//Native Method start...
}