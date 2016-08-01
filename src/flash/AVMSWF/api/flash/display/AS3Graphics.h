#ifndef _AS3Graphics_
#define _AS3Graphics_

struct _XCOMMAND;
struct _XSObject;
class DisplayList;

#include "AS3Shader.h"
#include "AS3Matrix.h"
#include "AS3BitmapData.h"

namespace avmplus{namespace NativeID{
class GraphicsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GraphicsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DisplayObjectObject;
	class BitmapDataObject;
	class MatrixObject;
	class GraphicsObject;
	class IGraphicsDataVectorObject;
	class GraphicsClass : public ClassClosure
	{
	public:
		GraphicsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		GraphicsObject* CreateGraphics(DisplayObjectObject*);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsClassSlots m_slots_GraphicsClass;
};
class GraphicsObject : public ScriptObject
{
	public:
		enum {HASFILL=1,HASLINE=2};
		XU32 m_hasLineOrFill;
		GraphicsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
	public:		
		int m_nThick;
		_XCOMMAND* m_pCmds;
		DisplayList*m_pDisplayList;
		DRCWB(DisplayObjectObject*) m_pDisplay;
	protected:
		inline DisplayList* GetDisplay(){return m_pDisplayList;}
		inline _XSObject*	GetSObject();
		inline void ModifyObject();
		void AddCommand(_XCOMMAND*pCmd);
		void FreeCommands(_XCOMMAND*pCmds,bool bAll=true);
		void _lineTo(int x,int y,bool bMove);
		void _beginFill(Stringp strType,ArrayObject*colors,ArrayObject*alphas,ArrayObject*ratios,MatrixObject*matrix,Stringp spreadMothod,Stringp interMode,double fRatio,int nWidth);
		void _beginFill(uint32_t c,double alpha,int nWidth);
		void ClearAllActor();
	public:
		double lastAlpha;
		int m_nCurX,m_nCurY;
		//void UpdateBounds(int x,int y);
		void AS3_beginBitmapFill(BitmapDataObject*bmp,MatrixObject*mat,bool rep,bool smooth);
		void AS3_beginFill(uint32_t c,double alpha);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_beginGradientFill(Stringp strType,ArrayObject*colors,ArrayObject*alphas,ArrayObject*ratios,MatrixObject*matrix,Stringp spreadMothod,Stringp interMode,double fRatio);		
		void AS3_clear();
		void AS3_curveTo(double cx,double cy,double ax,double ay);
		void AS3_drawCircle(double x,double y,double r);
		void AS3_drawEllipse(double x,double y,double w,double h);
		void AS3_drawRect(double x,double y,double w,double h);
		void AS3_drawRoundRect(double x,double y,double w,double h,double rx,double ry);
		void AS3_endFill();
		void AS3_lineGradientStyle(Stringp strType,ArrayObject*colors,ArrayObject*alphas,ArrayObject*ratios,MatrixObject*matrix,Stringp spreadMothod,Stringp interMode,double fRatio);
		void AS3_lineStyle(double thick,uint32_t c,double alpha,bool pixelHinting,Stringp scaleMode,Stringp caps,Stringp jonts,double miterLimit);
		void AS3_lineTo(double x,double y);
		void AS3_moveTo(double x,double y);

		//new adds
		void AS3_beginShaderFill(ShaderObject* pShader, MatrixObject* pMatrix);
		void AS3_copyFrom(GraphicsObject *pSourceGraphics);
//		void AS3_drawGraphicsData(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_drawPath(ObjectVectorObject *pCommands, DoubleVectorObject *pData, Stringp winding);
		void AS3_drawTriangles(DoubleVectorObject *pVertices, IntVectorObject *pIndices, DoubleVectorObject *pUvtData, Stringp culling);
		void AS3_lineBitmapStyle(BitmapDataObject *pBitmap, MatrixObject* pMatrix, bool repeat, bool smooth);
		void AS3_lineShaderStyle(ShaderObject *pShader, MatrixObject* pMatrix);
		//new adds end

		//new adds 11
		void AS3_cubicCurveTo(double controlX1, double controlY1, double controlX2, double controlY2, double anchorX, double anchorY);
		IGraphicsDataVectorObject* AS3_readGraphicsData(bool recurse = true);
		//new adds 11 end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsObjectSlots m_slots_GraphicsObject;
};}
#endif