#ifndef _AS3GraphicsTrianglePath_
#define _AS3GraphicsTrianglePath_
namespace avmplus{namespace NativeID{
class GraphicsTrianglePathClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GraphicsTrianglePathObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	IntVectorObject *indices;
	DoubleVectorObject *vertices;
	DoubleVectorObject *uvtData;
	Stringp _culling;
private:};
}}
namespace avmshell{
	class GraphicsTrianglePathClass : public ClassClosure
	{
	public:
		GraphicsTrianglePathClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsTrianglePathClassSlots m_slots_GraphicsTrianglePathClass;
};
class GraphicsTrianglePathObject : public ScriptObject
{
	public:
		GraphicsTrianglePathObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		//Stringp AS3_culling_get();
		//void AS3_culling_set(Stringp pCulling);
		//AvmBox AS3_indices_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_indices_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_uvtData_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_uvtData_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_vertices_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_vertices_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsTrianglePathObjectSlots m_slots_GraphicsTrianglePathObject;
};}
#endif