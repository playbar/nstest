#ifndef _AS3GraphicsGradientFill_
#define _AS3GraphicsGradientFill_

#include "AS3Matrix.h"

namespace avmplus{namespace NativeID{

class MatrixObject;

class GraphicsGradientFillClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GraphicsGradientFillObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	Stringp _type;
	ArrayObject *colors;
	ArrayObject *alphas;
	ArrayObject *ratios;
	MatrixObject *matrix;
	Stringp _spreadMethod;
	Stringp _interpolationMethod;
	double focalPointRatio;
private:};
}}
namespace avmshell{
	class GraphicsGradientFillClass : public ClassClosure
	{
	public:
		GraphicsGradientFillClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsGradientFillClassSlots m_slots_GraphicsGradientFillClass;
};
class GraphicsGradientFillObject : public ScriptObject
{
	public:
		GraphicsGradientFillObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//ArrayObject* AS3_alphas_get();
		//void AS3_alphas_set(ArrayObject *pAlphas);
		//ArrayObject* AS3_colors_get();
		//void AS3_colors_set(ArrayObject *pColors);
		//double AS3_focalPointRatio_get();
		//void AS3_focalPointRatio_set(double focalPointRatio);
		//Stringp AS3_interpolationMethod_get();
		//void AS3_interpolationMethod_set(Stringp pInterpolationMethod);
		//MatrixObject* AS3_matrix_get();
		//void AS3_matrix_set(MatrixObject *pMatrix);
		//ArrayObject* AS3_ratios_get();
		//void AS3_ratios_set(ArrayObject *pRatios);
		//Stringp AS3_spreadMethod_get();
		//void AS3_spreadMethod_set(Stringp pSpreadMethod);
		//Stringp AS3_type_get();
		//void AS3_type_set(Stringp pType);
		//void AS3_constructor(Stringp pType, ArrayObject *pColors, ArrayObject *pAlphas, 
		//						ArrayObject *pRatios, Atom matrix, Atom spreadMethod, 
		//						Stringp pInterpolationMethod, double focalPointRatio);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsGradientFillObjectSlots m_slots_GraphicsGradientFillObject;
};}
#endif