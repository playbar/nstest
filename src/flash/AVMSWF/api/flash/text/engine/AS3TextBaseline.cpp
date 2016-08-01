#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextBaseline.h"
namespace avmshell{
TextBaselineClass::TextBaselineClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TextBaselineClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextBaselineObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextBaselineObject::TextBaselineObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...

int TextBaselineClass::GetBaseMode(Stringp strBaseLine)
{
	if(strBaseLine==NULL)
		return ROMAN;
	else if(strBaseLine->equals(m_slots_TextBaselineClass.ASCENT))
		return ASCENT;
	else if(strBaseLine->equals(m_slots_TextBaselineClass.DESCENT))
		return DESCENT;
	else if(strBaseLine->equals(m_slots_TextBaselineClass.IDEOGRAPHIC_BOTTOM))
		return IDEOGRAPHIC_BOTTOM;
	else if(strBaseLine->equals(m_slots_TextBaselineClass.IDEOGRAPHIC_CENTER))
		return IDEOGRAPHIC_CENTER;
	else if(strBaseLine->equals(m_slots_TextBaselineClass.IDEOGRAPHIC_TOP))
		return IDEOGRAPHIC_TOP;
	else if(strBaseLine->equals(m_slots_TextBaselineClass.ROMAN))
		return ROMAN;
	else if(strBaseLine->equals(m_slots_TextBaselineClass.USE_DOMINANT_BASELINE))
		return USE_DOMINANT_BASELINE;
	return USE_DOMINANT_BASELINE;
}
Stringp TextBaselineClass::GetBaseName(int iMode)
{
	switch(iMode)
	{
	case ASCENT:
		 return m_slots_TextBaselineClass.ASCENT;
	case DESCENT:
		 return m_slots_TextBaselineClass.DESCENT;
	case IDEOGRAPHIC_BOTTOM:
		 return m_slots_TextBaselineClass.IDEOGRAPHIC_BOTTOM;
	case IDEOGRAPHIC_CENTER:
		 m_slots_TextBaselineClass.IDEOGRAPHIC_CENTER;
	case IDEOGRAPHIC_TOP:
		 return m_slots_TextBaselineClass.IDEOGRAPHIC_TOP;
	case ROMAN:
		 return m_slots_TextBaselineClass.ROMAN;
	default:
	case USE_DOMINANT_BASELINE:
		 return m_slots_TextBaselineClass.USE_DOMINANT_BASELINE;
		 break;
	}
}

}