#ifndef _AS3Utils3D_
#define _AS3Utils3D_

#include "AS3Matrix3D.h"
#include "AS3Vector3D.h"

namespace avmplus{namespace NativeID{
class Utils3DClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class Utils3DObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class Utils3DClass : public ClassClosure
	{
	public:
		Utils3DClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::Utils3DClassSlots m_slots_Utils3DClass;
};
class Utils3DObject : public ScriptObject
{
	public:
		Utils3DObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Matrix3DObject* AS3_pointTowards(double percent, Matrix3DObject *pMat, Vector3DObject *pPos, Vector3DObject *pAt, Vector3DObject *pUp);
		Vector3DObject* AS3_projectVector(Matrix3DObject *pM, Vector3DObject *pV);
		void AS3_projectVectors(Matrix3DObject *pM, DoubleVectorObject *pVerts, DoubleVectorObject *pProjectedVerts, DoubleVectorObject *pUvts);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::Utils3DObjectSlots m_slots_Utils3DObject;
};}
#endif