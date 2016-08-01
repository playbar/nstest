
#include "StdAfxflash.h"
#include "avmplus.h"
#include "splayer.h"


// The macro AVMPLUS_WORD_CODE is true if the representation of ABC code is as an array of words and
// not an array of bytes.

//static int nDebugCount=0;
#ifdef _WINEMU
bool _analyFunction = false;
#endif

namespace avmplus
{	

	void ShowFunName(const Multiname* name,Atom classAtom )
	{
		if(name==NULL) 
			return;
		char szTmp[256];
		StUTF8String strFun1(name->getName());
		sprintf( szTmp, "F-->:%s", strFun1.c_str() );
		//OutputDebugStringA( szTmp );
		return;


		ScriptObject* objClassName = (ScriptObject*)(classAtom&~7);
		StUTF8String strClassName(objClassName->vtable->traits->name());
		sprintf( szTmp, "C-->:%s, F-->:%s  ", strClassName.c_str(), strFun1.c_str() );
		//OutputDebugStringA( szTmp );
	}

	XBOOL IsFunction(const Multiname* name,Atom classAtom,char*strClass,char*strFun)
	{
		if(name==NULL) 
			return XFALSE;
		if(name->isAnyName()||name->isRtname())
			return XFALSE;
		if(AvmCore::isNullOrUndefined(classAtom))
			return XFALSE;
		if(strFun!=NULL)
		{
			StUTF8String ss(name->getName());
			if(strcmp(ss.c_str(),strFun)!=0) 
				return XFALSE;
		}
		ScriptObject*obj=(ScriptObject*)(classAtom&~7);
		if(strClass!=NULL)
		{
			StUTF8String s1(obj->vtable->traits->name());
			if(strstr(s1.c_str(),strClass)==NULL)
				return XFALSE;
		}
		return XTRUE;
	}
	// an auto-ptr for managing MethodFrame calls.
	class EnterMethodEnv
	{
	public:
		inline explicit EnterMethodEnv(AvmCore* core, MethodEnv* env, MethodFrame& frame) : m_core(core), m_frame(frame)
		{
			m_frame.enter(m_core, env);
		}
		inline ~EnterMethodEnv()
		{
			m_frame.exit(m_core);
		}
	private:
		AvmCore* m_core;
		MethodFrame& m_frame;
	};


#define CHECK_INT_ATOM(a) (a)
#define MAKE_INTEGER(v)   ((intptr_t(v) << 3) | kIntptrType)


#define IS_INTEGER(v)        (((v) & 7) == kIntptrType)
#define IS_DOUBLE(v)         (((v) & 7) == kDoubleType)
#define IS_BOOLEAN(v)        (((v) & 7) == kBooleanType)
#define IS_STRING(v)		 (((v) & 7) == kStringType)
#define SIGN_EXTEND(v)       (intptr_t(v))
#define CLAMP_32(v)			(intptr_t(v))

#define INT32_VALUE(v)       int32_t(atomGetIntptr(v))
#define UINT32_VALUE(v)      uint32_t(atomGetIntptr(v))
#define DOUBLE_VALUE(v)		 (*(double*)((v) ^ kDoubleType))
#define IS_BOTH_INTEGER(a,b) ((((a ^ kIntptrType) | (b ^ kIntptrType)) & 7) == 0) 
#define IS_BOTH_DOUBLE(a,b)  ((((a ^ kDoubleType) | (b ^ kDoubleType)) & 7) == 0)

	typedef uint8_t bytecode_t;


#  define INSTR(op) case OP_##op:
#  define NEXT              continue
#  define U30ARG            (tmp_pc=pc, tmp_u30 = uint32_t(readU30(tmp_pc)), pc = tmp_pc, tmp_u30)
#  define U8ARG             (*pc++)
#  define S24ARG            (pc+=3, readS24(pc-3))
#  define SAVE_EXPC	        expc = pc-1-codeStart
#  define SAVE_EXPC_S24     expc = pc-4-codeStart



#define FAST_INC_MAYBE(a1,dest) \
	if (IS_INTEGER(a1)) { \
	u1t = a1 ^ kIntptrType; \
	u3t = SIGN_EXTEND(u1t + (1 << 3)); \
	if ((intptr_t)u1t < 0 || (intptr_t)(u3t ^ u1t) >= 0) { \
	dest = CHECK_INT_ATOM(u3t | kIntptrType); \
	NEXT; \
	} \
	} \
else if (IS_DOUBLE(a1)) { \
	dest = core->doubleToAtom(DOUBLE_VALUE(a1) + 1.0); \
	NEXT; \
	}

#define ADD_TWO_VALUES_AND_NEXT(a1, a2, dest) \
	if (IS_BOTH_INTEGER(a1, a2)) { \
	u1t = a1 ^ kIntptrType; \
	u2t = a2 ^ kIntptrType; \
	u3t = SIGN_EXTEND(u1t + u2t); \
	if ((intptr_t)(u1t ^ u2t) < 0 || (intptr_t)(u1t ^ u3t) >= 0) { \
	dest = CHECK_INT_ATOM(u3t | kIntptrType); \
	NEXT; \
	} \
	} \
else if (IS_BOTH_DOUBLE(a1, a2)) { \
	dest = core->doubleToAtom(DOUBLE_VALUE(a1) + DOUBLE_VALUE(a2)); \
	NEXT; \
	} \
	SAVE_EXPC; \
	dest = toplevel->add2(a1, a2); \
	NEXT

	// Subtract 1 from a1 if a1 is a fixnum and computation does not overflow.
	// On success, store the result in dest, and NEXT. 
#define FAST_DEC_MAYBE(a1,dest) \
	if (IS_INTEGER(a1)) { \
	u1t = a1 ^ kIntptrType; \
	u3t = SIGN_EXTEND(u1t - (1 << 3)); \
	if ((intptr_t)u1t >= 0 || (intptr_t)(u1t ^ u3t) >= 0) { \
	dest = CHECK_INT_ATOM(u3t | kIntptrType); \
	NEXT; \
	} \
	} \
else if (IS_DOUBLE(a1)) { \
	dest = core->doubleToAtom(DOUBLE_VALUE(a1) - 1.0); \
	NEXT; \
	}


#define BITOP_TWO_VALUES_AND_NEXT(op, a1, a2, dest, tag) \
	if (IS_BOTH_INTEGER(a1,a2)) { \
	dest = (CLAMP_32((a1) op (a2)) | tag); \
	NEXT; \
	} \
	SAVE_EXPC; \
	i1 = AvmCore::integer(a1); \
	i2 = AvmCore::integer(a2); \
	dest = core->intToAtom((int32_t)(i1 op i2)); \
	NEXT

	// note that the mops "addr" (offset from globalMemoryBase) is in fact a signed int, so we have to check
	// for it being < 0 ... but we can get by with a single unsigned compare since all values < 0 will be > size
#define MOPS_RANGE_CHECK(addr, type) \
	if (uint32_t(addr) > (envDomain->globalMemorySize() - sizeof(type))) { avmplus::mop_rangeCheckFailed(env); }

#define MOPS_LOAD(addr, type, call, result) \
	MOPS_RANGE_CHECK(addr, type) \
	union { const uint8_t* p8; const type* p; }; \
	p8 = envDomain->globalMemoryBase() + (addr); \
	result = *p;

#define MOPS_STORE(addr, type, call, value) \
	MOPS_RANGE_CHECK(addr, type) \
	union { uint8_t* p8; type* p; }; \
	p8 = envDomain->globalMemoryBase() + (addr); \
	*p = (type)(value);

	// The client of IFCMP_TWO_VALUES must save the EXPC before extracting
	// the operands from the instruction stream because it is used in various
	// contexts where the instruction sizes and layouts aren't the same.

#define GENERATE_FLAG(numeric_cmp, generic_cmp, a1, a2) \
	if (IS_BOTH_INTEGER(a1, a2)) \
	b1 = a1 numeric_cmp a2; \
	else if (IS_BOTH_DOUBLE(a1, a2)) \
	b1 = DOUBLE_VALUE(a1) numeric_cmp DOUBLE_VALUE(a2); \
	else \
	b1 = generic_cmp

#define IFCMP_TWO_VALUES(numeric_cmp, generic_cmp, a1, a2, i1) \
	GENERATE_FLAG(numeric_cmp, generic_cmp, a1, a2); \
	if (b1) \
	{ \
	if (i1 < 0) \
	core->branchCheck(env, interruptable, (int32_t)i1); \
	pc += i1; \
	}

#define IFNCMP_TWO_VALUES(numeric_cmp, generic_cmp, a1, a2, i1) \
	GENERATE_FLAG(numeric_cmp, generic_cmp, a1, a2); \
	if (!b1) \
	{ \
	if (i1 < 0) \
	core->branchCheck(env, interruptable, (int32_t)i1); \
	pc += i1; \
	}

#define LOAD_OFFSET_AND_FETCH_SS(a1,a2,i1) \
	SAVE_EXPC; \
	a1 = sp[-1]; \
	a2 = sp[0]; \
	sp -= 2; \
	i1 = S24ARG


#define CMP2(numeric_cmp, generic_cmp) \
	a1 = sp[-1]; \
	a2 = sp[0]; \
	sp--; \
	if (IS_BOTH_INTEGER(a1, a2)) \
	b1 = a1 numeric_cmp a2; \
	else if (IS_BOTH_DOUBLE(a1, a2)) \
	b1 = DOUBLE_VALUE(a1) numeric_cmp DOUBLE_VALUE(a2); \
	else { \
	SAVE_EXPC; \
	b1 = generic_cmp; \
	} \
	sp[0] = b1 ? trueAtom : falseAtom;

#define GET_MULTINAME_PTR(decl, arg)  Multiname _multiname; pool->parseMultiname(_multiname, arg); decl = &_multiname

	struct InterpreterAuxiliaryFrame
	{
		MethodFrame methodFrame;
		ExceptionFrame ef;
		Multiname multiname2;
	};


	inline intptr_t readS24(const uint8_t* pc)
	{
		return AvmCore::readS24(pc);
	}
	inline uintptr_t readU30(const uint8_t*& pc)
	{
		return AvmCore::readU30(pc);
	}

	uintptr_t interpGPR(MethodEnv* env, int argc, uint32_t *ap)
	{
		Atom* const atomv = (Atom*)ap;
		MethodSignaturep ms = env->method->getMethodSignature();
		ms->boxArgs(env->core(), argc, (uint32 *)ap, atomv);
		Atom a = interpBoxed(env, argc, atomv);
		const BuiltinType bt = ms->returnTraitsBT();
		const uint32_t ATOM_MASK = (1U<<BUILTIN_object) | (1U<<BUILTIN_void) | (1U << BUILTIN_any);
		if ((1U<<bt) & ATOM_MASK)
			return a;
		if (bt == BUILTIN_int)
			return AvmCore::integer_i(a);
		if (bt == BUILTIN_uint)
			return AvmCore::integer_u(a);
		if (bt == BUILTIN_boolean)
			return a>>3;
		return a & ~7; // possibly null pointer
	}

	double interpFPR(MethodEnv* env, int argc, uint32_t * ap)
	{
		Atom* const atomv = (Atom*)ap;
		MethodSignaturep ms = env->method->getMethodSignature();
		ms->boxArgs(env->core(), argc, (uint32 *)ap, atomv);
		Atom a = interpBoxed(env, argc, atomv);
		return AvmCore::number_d(a);
	}


	// Note, this function is not on the hot path normally, so optimizing it is not urgent.
	//
	// OPTIMIZEME - statically knowable if name isRtname or isRtns; exploit this somehow?
	// OPTIMIZEME - often knowable whether the TOS is an object or something simple; exploit this?

	Atom* initMultiname(MethodEnv* env, Multiname &name, Atom* sp)
	{
		if (name.isRtname())
		{
			Atom index = *(sp--);
			AvmCore* core = env->core();

			// is it a qname?
			if (AvmCore::isObject(index))
			{
				ScriptObject* i = AvmCore::atomToScriptObject(index);
				if (i->traits() == core->traits.qName_itraits)
				{
					QNameObject* qname = (QNameObject*) i;
					bool attr = name.isAttr();
					qname->getMultiname(name);
					if (attr)
						name.setAttr(attr);

					// Discard runtime namespace if present
					if (name.isRtns())
						sp--;

					return sp;
				}
			}

			name.setName(core->intern(index));
		}

		if (name.isRtns())
			name.setNamespace(env->internRtns(*(sp--)));

		return sp;
	}

	Atom* initMultinameNoXMLList(MethodEnv* env, Multiname &name, Atom* sp)
	{
		if (name.isRtname() && AvmCore::isXMLList(sp[0]))
		{
			env->toplevel()->throwTypeError(kDeleteTypeError, env->core()->toErrorString(env->toplevel()->toTraits(sp[0])));
		}
		return initMultiname(env, name, sp);
	}

	Traits* getTraits(const Multiname* name, PoolObject* pool, Toplevel* toplevel, AvmCore* core)
	{
		Traits* t = pool->getTraits(*name, toplevel);
		if( name->isParameterizedType() )
		{
			const Multiname* param_name;
			GET_MULTINAME_PTR(param_name, name->getTypeParameter());
			Traits* param_traits = name->getTypeParameter() ? getTraits(param_name, pool, toplevel, core) : NULL;
			t = pool->resolveParameterizedType(toplevel, t, param_traits);
		}
		return t;
	}

	Atom interpBoxed(register MethodEnv* env, register int _argc, register Atom* _atomv)
	{

		register AvmCore* const core = env->core();
		register Toplevel* const toplevel = env->toplevel();
		register MethodInfo* const info = env->method;
		register PoolObject* const pool = info->pool();


		//const size_t kAuxFrameSize = core->debugger() ? sizeof(InterpreterAuxiliaryFrameWithCSN) : sizeof(InterpreterAuxiliaryFrame);
		const size_t kAuxFrameSize = sizeof(InterpreterAuxiliaryFrame);
		core->stackCheck(env);

		register List<double*, LIST_GCObjects> const & cpool_double = pool->cpool_double;
		register const bool interruptable = !info->isNonInterruptible();
		register const Domain* envDomain = env->domainEnv()->domain();

		MethodSignaturep volatile ms = env->method->getMethodSignature();
		register const bytecode_t* volatile codeStart = ms->abc_code_start();

		register const bytecode_t* pc = ms->abc_code_start();
		intptr_t volatile expc=0;
		MMgc::GC::AllocaAutoPtr _framep;

		register Atom* const framep = (Atom*)VMPI_alloca(core, _framep, sizeof(Atom)*(ms->frame_size()) + kAuxFrameSize);
		Atom *fa = framep + ms->frame_size();
		register InterpreterAuxiliaryFrame* const aux_memory = (InterpreterAuxiliaryFrame*)fa;

		// It's essential that the MethodFrame is cleaned up upon normal exit, to keep core->currentMethodFrame
		// in order. (a throw past the frame will not perform cleanup.) A manual call just before returning 
		// is *not* adequate, as we need to be able to call aux_memory->methodFrame->exit() *after* our TRY/CATCH
		//  code has completed (otherwise, it may attempt to "restore" currentStack to a bogus value). 
		// Using a real dtor here ensures we are called after any endTry().
		EnterMethodEnv methodFrame(core, env, aux_memory->methodFrame);

		register Atom* const scopeBase = framep + ms->local_count();
		register Atom* volatile withBase = NULL;
		register int volatile scopeDepth = 0;
		register ScopeChain* const scope = env->scope();
		register Atom* sp = scopeBase + ms->max_scope() - 1;
		ScriptObject* globalScope = (scope->getSize() > 0) ? AvmCore::atomToScriptObject(scope->getScope(0)) : NULL;

		// Copy instance and args to local frame
		const int param_count = ms->param_count();
		for (int i=0, n = _argc < param_count ? _argc : param_count; i <= n; i++)
			framep[i] = _atomv[i];

		// Store original value of argc for createRest and createArguments.
		// argc may be changed by the optional parameter check below.
		int arguments_argc = _argc;

		// Set optional param values.  these not aliased to arguments[] since arguments[]
		// only present with traditional prototype functions (no optional args)
		if (info->hasOptional())
		{
			if (_argc < param_count)
			{
				// initialize default values
				for (int i=_argc+1, o=_argc + ms->optional_count() - param_count, n=param_count; i <= n; i++, o++)
					framep[i] = ms->getDefaultValue(o);
				_argc = param_count;
			}
		}

		// Set remaining locals to undefined.  Don't have to init scope or stack because
		// our conservative GC scan knows how to ignore garbage.
		for (Atom *p = framep + 1 + param_count; p < scopeBase; p++)
			*p = undefinedAtom;

		// Capture arguments or rest array.
		if (info->needRest())
		{
			framep[param_count+1] = env->createRest(_atomv, arguments_argc)->atom();
		}
		else if (info->needArguments())
		{
			framep[param_count+1] = env->createArguments(_atomv, arguments_argc)->atom();
		}

		for (int i = 0; i < ms->max_scope(); ++i)
			scopeBase[i] = nullObjectAtom;

		register Atom a1 = 0, a2 = 0, a3 = 0;
		register Atom* a2p = XNULL;
		register intptr_t i1 = 0, i2 = 0;
		register uintptr_t u1 = 0, u2 = 0;
		register uintptr_t u1t = 0, u2t = 0, u3t = 0;	// private to the generic arithmetic macros
		register double d1 = 0, d2 = 0;
		register bool b1 = false;
		register uint8_t ub2 = 0;
		register ScriptObject *o1 = XNULL;
		register const Multiname* multiname = XNULL;
		register MethodEnv* f = XNULL;
		register Traits* t1 = XNULL;
		uint16_t uh2l = 0;				// not register - its address /can/ be taken
		int32_t i32l = 0;				// ditto
		float f2l = 0;					// ditto
		double d2l = 0;					// ditto
		Atom a1l = 0;					// ditto
		register uint32_t tmp_u30 = 0;
		const bytecode_t* tmp_pc = NULL;

		//_ANALYENTER("AS3 Mechine");
MainLoop:

		TRY_UNLESS_HEAPMEM((char*)aux_memory + offsetof(InterpreterAuxiliaryFrame, ef), core, !info->abc_exceptions(), kCatchAction_SearchForActionScriptExceptionHandler)

			while ( true )
			{
				switch (*pc++)
				{

					INSTR(returnvoid) 
					{
						a1 = undefinedAtom;
						SAVE_EXPC;
						a1 = toplevel->coerce(a1, ms->returnTraits());
						return a1;
					}

					INSTR(returnvalue)
					{
						a1 = *sp;
						SAVE_EXPC;
						a1 = toplevel->coerce(a1, ms->returnTraits());
						return a1;
					}

					INSTR(nop) 
					{
						NEXT;
					}

					INSTR(label)
					{
						NEXT;
					}

					INSTR(timestamp) 
					{
						NEXT;
					}

					INSTR(coerce_a) 
					{ // no-op since interpreter only uses atoms
						NEXT;
					}

					INSTR(debugline)
					{
						SAVE_EXPC;
						u1 = U30ARG;
						NEXT;
					}

					INSTR(debug)
					{
						//WORD_CODE_ONLY( pc += 4 );
						pc += AvmCore::calculateInstructionWidth(pc-1) - 1;
						NEXT;
					}

					INSTR(debugfile) 
					{
						SAVE_EXPC;
						u1 = U30ARG;
						NEXT;
					}

					INSTR(jump)
					{
						i1 = S24ARG;
						if (i1 < 0) 
						{
							SAVE_EXPC_S24;
							core->branchCheck(env, interruptable, (int32_t)i1);
						}
						pc += i1;
						NEXT;
					}

					INSTR(pushnull)
					{
						*(++sp) = nullObjectAtom;
						NEXT;
					}

					INSTR(pushundefined) 
					{
						*(++sp) = undefinedAtom;
						NEXT;
					}

					INSTR(pushstring) 
					{
						*(++sp) = pool->getString((uint32_t)U30ARG)->atom();
						NEXT;
					}

					INSTR(pushint) 
					{
						*(++sp) = core->intToAtom(pool->cpool_int[(uint32_t)U30ARG]);
						NEXT;
					}

					INSTR(pushuint)
					{
						*(++sp) = core->uintToAtom(pool->cpool_uint[(uint32_t)U30ARG]);
						NEXT;
					}

					INSTR(pushdouble) 
					{
						*(++sp) = kDoubleType|(uintptr_t)cpool_double[(uint32_t)U30ARG];
						NEXT;
					}

					INSTR(pushnamespace) {
						*(++sp) = pool->cpool_ns[(uint32_t)U30ARG]->atom();
						NEXT;
					}

					INSTR(getlocal) {
						u1 = U30ARG;
						*(++sp) = framep[u1];
						NEXT;
					}

					INSTR(getlocal0) {
						*(++sp) = framep[0];
						NEXT;
					}

					INSTR(getlocal1) {
						*(++sp) = framep[1];
						NEXT;
					}

					INSTR(getlocal2) {
						*(++sp) = framep[2];
						NEXT;
					}

					INSTR(getlocal3) {
						*(++sp) = framep[3];
						NEXT;
					}

					INSTR(pushtrue) {
						*(++sp) = trueAtom;
						NEXT;
					}

					INSTR(pushfalse) {
						*(++sp) = falseAtom;
						NEXT;
					}

					INSTR(pushnan) {
						*(++sp) = core->kNaN;
						NEXT;
					}

					INSTR(pop) {
						sp--;
						NEXT;
					}

					INSTR(dup) {
						sp++;
						sp[0] = sp[-1];
						NEXT;
					}

					INSTR(swap) {
						a1 = sp[0];
						sp[0] = sp[-1];
						sp[-1] = a1;
						NEXT;
					}

					INSTR(convert_s) {
						if ((sp[0] & 7) != kStringType) 
						{
							SAVE_EXPC;
							sp[0] = core->string(sp[0])->atom();
						}
						NEXT;
					}

					INSTR(esc_xelem) { // ToXMLString will call EscapeElementValue
						SAVE_EXPC;
						sp[0] = core->ToXMLString(sp[0])->atom();
						NEXT;
					}

					INSTR(esc_xattr) {
						SAVE_EXPC;
						sp[0] = core->EscapeAttributeValue(sp[0])->atom();
						NEXT;
					}

					INSTR(convert_d) {
convert_d_impl:
						if (!IS_DOUBLE(sp[0])) {
							SAVE_EXPC;
							sp[0] = core->numberAtom(sp[0]);
						}
						NEXT;
					}

					INSTR(coerce_d) {
						goto convert_d_impl;
					}

					INSTR(convert_b) {
convert_b_impl:
						a1 = sp[0];	// boolean value
						if (IS_BOOLEAN(a1))
							;
						else if (IS_INTEGER(a1))
							sp[0] = a1 == zeroIntAtom ? falseAtom : trueAtom;
						else
							sp[0] = AvmCore::booleanAtom(a1);
						NEXT;
					}

					INSTR(coerce_b) {
						goto convert_b_impl;
					}

					INSTR(convert_o) {
						if (AvmCore::isNullOrUndefined(sp[0])) {
							SAVE_EXPC;
							env->nullcheck(sp[0]);
						}
						NEXT;
					}

					INSTR(negate) {
						a1 = sp[0];
						if (IS_INTEGER(a1) && a1 != zeroIntAtom) {
							i1 = -atomGetIntptr(a1); // *not* INT32_VALUE
							if (atomIsValidIntptrValue(i1)) {
								sp[0] = MAKE_INTEGER(i1);
								NEXT;
							}
						}
						SAVE_EXPC;
						sp[0] = core->doubleToAtom(-AvmCore::number(a1));
						NEXT;
					}

					INSTR(negate_i) {
						// OPTIMIZEME - negate_i
						SAVE_EXPC;
						sp[0] = core->intToAtom(-AvmCore::integer(sp[0]));
						NEXT;
					}

					INSTR(kill) {
						u1 = U30ARG;
						framep[u1] = undefinedAtom;
						NEXT;
					}

					INSTR(typeof) {
						*sp = core->_typeof(*sp)->atom();
						NEXT;
					}

					INSTR(not) {
						a1 = sp[0];		// boolean value
						if (IS_BOOLEAN(a1))
							;
						else if (IS_INTEGER(a1))
							a1 = a1 == zeroIntAtom ? falseAtom : trueAtom;
						else
							a1 = AvmCore::booleanAtom(a1);
						sp[0] = a1 ^ (trueAtom ^ falseAtom);
						NEXT;
					}

					INSTR(bitnot) {
						a1 = sp[0];
						if (IS_INTEGER(a1)) {
							sp[0] = MAKE_INTEGER(~int32_t(atomGetIntptr(a1)));
							NEXT;
						}
						SAVE_EXPC;
						*sp = core->intToAtom(~AvmCore::integer(a1));
						NEXT;
					}

					INSTR(setlocal) {
						u1 = U30ARG;
						framep[u1] = *(sp--);
						NEXT;
					}

					INSTR(setlocal0) {
						framep[0] = *(sp--);
						NEXT;
					}

					INSTR(setlocal1) {
						framep[1] = *(sp--);
						NEXT;
					}

					INSTR(setlocal2) {
						framep[2] = *(sp--);
						NEXT;
					}

					INSTR(setlocal3) {
						framep[3] = *(sp--);
						NEXT;
					}	

					// Add 1 to a1 if it is a fixnum and computation does not overflow, or
					// if a1 is a flonum.  On success, store the result in dest, and NEXT.


					INSTR(increment)
					{
						SAVE_EXPC;
						a2p = sp;
						a1 = *a2p;
						FAST_INC_MAYBE(a1,*a2p);	// note, *a2p is lvalue here
						*a2p = core->numberAtom(a1);
						core->increment_d(a2p, 1);
						NEXT;
					}

					INSTR(inclocal) {
						SAVE_EXPC;
						a2p = framep+U30ARG;
						a1 = *a2p;
						FAST_INC_MAYBE(a1,*a2p);	// note, *a2p is lvalue here
						*a2p = core->numberAtom(a1);
						core->increment_d(a2p, 1);
						NEXT;
					}

					INSTR(increment_i) 
					{
						SAVE_EXPC;
						a2p = sp;
						a1 = *a2p;
						if (!IS_INTEGER(a1))
						{
							a1 = core->intAtom(a1);
						}
						FAST_INC_MAYBE(a1,*a2p);	// note, *a2p is lvalue here
						core->increment_i(a2p, 1);
						NEXT;
					}

					INSTR(inclocal_i) {
						SAVE_EXPC;
						a2p = framep+U30ARG;
						a1 = *a2p;
						if (!IS_INTEGER(a1))
						{
							a1 = core->intAtom(a1);
						}
						FAST_INC_MAYBE(a1,*a2p);	// note, *a2p is lvalue here
						core->increment_i(a2p, 1);
						NEXT;
					}

					INSTR(decrement) {
						SAVE_EXPC;
						a2p = sp;
						a1 = *a2p;
						FAST_DEC_MAYBE(a1,*a2p);	// note, *a2p is lvalue here
						*a2p = core->numberAtom(a1);
						core->increment_d(a2p, -1);
						NEXT;
					}

					INSTR(declocal) {
						SAVE_EXPC;
						a2p = framep+U30ARG;
						a1 = *a2p;
						FAST_DEC_MAYBE(a1,*a2p);	// note, *a2p is lvalue here
						*a2p = core->numberAtom(a1);
						core->increment_d(a2p, -1);
						NEXT;
					}

					INSTR(decrement_i) {
						SAVE_EXPC;
						a2p = sp;
						a1 = *a2p;
						FAST_DEC_MAYBE(a1,*a2p);	// note, *a2p is lvalue here
						core->increment_i(a2p, -1);
						NEXT;
					}

					INSTR(declocal_i) {
						SAVE_EXPC;
						a2p = framep+U30ARG;
						a1 = *a2p;
						FAST_DEC_MAYBE(a1,*a2p);	// note, *a2p is lvalue here
						core->increment_i(a2p, -1);
						NEXT;
					}

					INSTR(add) {
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						ADD_TWO_VALUES_AND_NEXT(a1, a2, sp[0]);
					}

					INSTR(add_i) {
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						if (IS_BOTH_INTEGER(a1, a2)) 
						{
							u1t = a1 ^ kIntptrType;
							u2t = a2 ^ kIntptrType;
							u3t = CLAMP_32(u1t + u2t);
							sp[0] = CHECK_INT_ATOM(u3t | kIntptrType);
							NEXT;
						}
						if (IS_BOTH_DOUBLE(a1, a2)) { 
							i1 = (int32_t)AvmCore::integer_d(DOUBLE_VALUE(a1));
							i2 = (int32_t)AvmCore::integer_d(DOUBLE_VALUE(a2));
							sp[0] = core->intToAtom((int32_t)(i1 + i2));
							NEXT;
						}
						SAVE_EXPC;
						i1 = AvmCore::integer(a1);
						i2 = AvmCore::integer(a2);
						sp[0] = core->intToAtom((int32_t)(i1 + i2));
						NEXT;
					}

					INSTR(subtract) 
					{
						a1 = sp[-1];
						a2 = sp[0];
						sp--;

						if (IS_BOTH_INTEGER(a1, a2)) 
						{ 
							u1t = a1 ^ kIntptrType; 
							u2t = a2 ^ kIntptrType;
							u3t = SIGN_EXTEND(u1t - u2t); 
							if ((intptr_t)(u1t ^ u2t) >= 0 || (intptr_t)(u1t ^ u3t) >= 0) { 
								sp[0] = CHECK_INT_ATOM(u3t | kIntptrType); 
								NEXT; 
							} 
						} 
						if (IS_BOTH_DOUBLE(a1, a2)) { 
							sp[0] = core->doubleToAtom(DOUBLE_VALUE(a1) - DOUBLE_VALUE(a2)); 
							NEXT; 
						}
						SAVE_EXPC;
						d1 = AvmCore::number(a1);
						d2 = AvmCore::number(a2);
						sp[0] = core->doubleToAtom(d1 - d2);
						NEXT;
					}

					INSTR(subtract_i) {
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						if (IS_BOTH_INTEGER(a1, a2)) {
							u1t = a1 ^ kIntptrType;
							u2t = a2 ^ kIntptrType;
							u3t = CLAMP_32(u1t - u2t);
							sp[0] = CHECK_INT_ATOM(u3t | kIntptrType);
							NEXT;
						}
						if (IS_BOTH_DOUBLE(a1, a2)) { 
							i1 = (int32_t)AvmCore::integer_d(DOUBLE_VALUE(a1));
							i2 = (int32_t)AvmCore::integer_d(DOUBLE_VALUE(a2));
							sp[0] = core->intToAtom((int32_t)(i1 - i2));
							NEXT;
						}
						SAVE_EXPC;
						i1 = AvmCore::integer(a1);
						i2 = AvmCore::integer(a2);
						sp[0] = core->intToAtom((int32_t)(i1 - i2));
						NEXT;
					}

					INSTR(multiply) {
						a1 = sp[-1];
						a2 = sp[0];
						sp--;

						// OPTIMIZEME - multiplication of small integers might be optimized?
						if (IS_BOTH_DOUBLE(a1, a2)) {
							sp[0] = core->doubleToAtom(DOUBLE_VALUE(a1) * DOUBLE_VALUE(a2)); \
								NEXT;
						}
						SAVE_EXPC;
						d1 = AvmCore::number(a1);
						d2 = AvmCore::number(a2);
						sp[0] = core->doubleToAtom(d1 * d2);
						NEXT;
					}

					INSTR(multiply_i) {
						// OPTIMIZEME - multiplication of small integers might be optimized?
						SAVE_EXPC;
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						i1 = AvmCore::integer(a1);
						i2 = AvmCore::integer(a2);
						sp[0] = core->intToAtom((int32_t)(i1 * i2));
						NEXT;
					}

					INSTR(divide) {
						// OPTIMIZEME - division of small integers might be optimized?
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						if (IS_BOTH_DOUBLE(a1, a2)) {
							sp[0] = core->doubleToAtom(DOUBLE_VALUE(a1) / DOUBLE_VALUE(a2));
							NEXT;
						}
						SAVE_EXPC;
						d1 = AvmCore::number(a1);
						d2 = AvmCore::number(a2);
						sp[0] = core->doubleToAtom(d1 / d2);
						NEXT;
					}

					INSTR(modulo) {

						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						if (IS_BOTH_INTEGER(a1, a2) && a2 != zeroIntAtom) {
							i1 = INT32_VALUE(a1) % INT32_VALUE(a2);
							if (atomIsValidIntptrValue(i1)) {
								sp[0] = MAKE_INTEGER(i1);
								NEXT;
							}
						}
						else if (IS_BOTH_DOUBLE(a1, a2)) {
							sp[0] = core->doubleToAtom(MathUtils::mod(DOUBLE_VALUE(a1), DOUBLE_VALUE(a2)));
							NEXT;
						}
						SAVE_EXPC;
						d1 = AvmCore::number(a1);
						d2 = AvmCore::number(a2);
						sp[0] = core->doubleToAtom(MathUtils::mod(d1, d2));
						NEXT;
					}

					INSTR(lshift) {
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						if (IS_BOTH_INTEGER(a1,a2)) {
							i1 = INT32_VALUE(a1) << (INT32_VALUE(a2) & 0x1F);
							if (atomIsValidIntptrValue(i1)) {
								sp[0] = SIGN_EXTEND(MAKE_INTEGER(i1));
								NEXT;
							}
						}
						SAVE_EXPC;
						i1 = AvmCore::integer(a1);
						u2 = AvmCore::toUInt32(a2);
						sp[0] = core->intToAtom( (int32_t)(i1 << (u2 & 0x1F)) );
						NEXT;
					}

					INSTR(rshift) {
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						if (IS_BOTH_INTEGER(a1,a2)) {
							sp[0] = MAKE_INTEGER(INT32_VALUE(a1) >> (INT32_VALUE(a2) & 0x1F));
							NEXT;
						}
						SAVE_EXPC;
						i1 = AvmCore::integer(a1);
						u2 = AvmCore::toUInt32(a2);
						sp[0] = core->intToAtom( (int32_t)(i1 >> (u2 & 0x1F)) );
						NEXT;
					}

					INSTR(urshift) {
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						if (IS_BOTH_INTEGER(a1,a2)) {
							u1 = (UINT32_VALUE(a1) >> (INT32_VALUE(a2) & 0x1F));
							if (atomIsValidIntptrValue_u(u1)) {
								sp[0] = MAKE_INTEGER(u1);
								NEXT;
							}
						}
						SAVE_EXPC;
						u1 = AvmCore::toUInt32(a1);
						u2 = AvmCore::toUInt32(a2);
						sp[0] = core->uintToAtom( (uint32_t)(u1 >> (u2 & 0x1F)) );
						NEXT;
					}

					INSTR(bitand) 
					{
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						BITOP_TWO_VALUES_AND_NEXT(&, a1, a2, sp[0], 0);
					}

					INSTR(bitor) {
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						BITOP_TWO_VALUES_AND_NEXT(|, a1, a2, sp[0], 0);
					}

					INSTR(bitxor) {
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						BITOP_TWO_VALUES_AND_NEXT(^, a1, a2, sp[0], kIntptrType);
					}

					INSTR(equals) {
						// OPTIMIZEME - equals on some classes of values?
						SAVE_EXPC;
						sp[-1] = core->equals(sp[-1], sp[0]);
						sp--;
						NEXT;
					}

					INSTR(strictequals) {
						// OPTIMIZEME - strictequals on some classes of values?
						sp[-1] = core->stricteq(sp[-1], sp[0]);
						sp--;
						NEXT;
					}

					INSTR(lookupswitch)
					{
						const uint8_t* base = pc-1;
						// safe to assume int since verifier checks for int
						uint32_t index = AvmCore::integer_i(*(sp--));
						const uint8_t* switch_pc = pc+3;
						uint32_t case_count = uint32_t(readU30(switch_pc)) + 1;
						pc = base+readS24( index < case_count ? (switch_pc + 3*index) : pc );
						NEXT;
					}

					INSTR(iftrue) {
						a2 = trueAtom;
						a1 = *(sp--);
						if (IS_BOOLEAN(a1))
							;
						else if (IS_INTEGER(a1))
							a1 = a1 == zeroIntAtom ? falseAtom : trueAtom;
						else
							a1 = AvmCore::booleanAtom(a1);  // does not throw or change the XML namespace
						i1 = S24ARG;
						if (a1 == a2)
						{
							if (i1 < 0) {
								SAVE_EXPC_S24;
								core->branchCheck(env, interruptable, (int32_t)i1);
							}
							pc += i1;
						}
						NEXT;
					}

					INSTR(iffalse) {
						a2 = falseAtom;
						a1 = *(sp--);
						if (IS_BOOLEAN(a1))
							;
						else if (IS_INTEGER(a1))
							a1 = a1 == zeroIntAtom ? falseAtom : trueAtom;
						else
							a1 = AvmCore::booleanAtom(a1);  // does not throw or change the XML namespace
						i1 = S24ARG;
						if (a1 == a2)
						{
							if (i1 < 0) {
								SAVE_EXPC_S24;
								core->branchCheck(env, interruptable, (int32_t)i1);
							}
							pc += i1;
						}
						NEXT;
					}

					INSTR(ifeq) {
						LOAD_OFFSET_AND_FETCH_SS(a1,a2,i1);
						IFCMP_TWO_VALUES(==, core->equals(a1,a2) == trueAtom, a1, a2, i1);
						NEXT;
					}

					INSTR(ifne) {
						LOAD_OFFSET_AND_FETCH_SS(a1,a2,i1);
						IFNCMP_TWO_VALUES(==, core->equals(a1,a2) == trueAtom, a1, a2, i1);
						NEXT;
					}

					INSTR(ifstricteq) {
						LOAD_OFFSET_AND_FETCH_SS(a1,a2,i1);
						IFCMP_TWO_VALUES(==, core->stricteq(a1,a2) == trueAtom, a1, a2, i1);
						NEXT;
					}

					INSTR(ifstrictne) {
						LOAD_OFFSET_AND_FETCH_SS(a1,a2,i1);
						IFNCMP_TWO_VALUES(==, core->stricteq(a1,a2) == trueAtom, a1, a2, i1);
						NEXT;
					}

					INSTR(iflt) {
						LOAD_OFFSET_AND_FETCH_SS(a1,a2,i1);
						IFCMP_TWO_VALUES(<, core->compare(a1,a2) == trueAtom, a1, a2, i1);
						NEXT;
					}

					INSTR(ifnlt) {
						LOAD_OFFSET_AND_FETCH_SS(a1,a2,i1);
						IFNCMP_TWO_VALUES(<, core->compare(a1, a2) == trueAtom, a1, a2, i1);
						NEXT;
					}

					INSTR(ifle) {
						LOAD_OFFSET_AND_FETCH_SS(a1,a2,i1);
						IFCMP_TWO_VALUES(<=, core->compare(a2, a1) == falseAtom,a1,a2,i1);
						NEXT;
					}

					INSTR(ifnle) {
						LOAD_OFFSET_AND_FETCH_SS(a1,a2,i1);
						IFNCMP_TWO_VALUES(<=, core->compare(a2, a1) == falseAtom,a1,a2,i1);
						NEXT;
					}

					INSTR(ifgt) {
						LOAD_OFFSET_AND_FETCH_SS(a1,a2,i1);
						IFCMP_TWO_VALUES(>, core->compare(a2, a1) == trueAtom,a1,a2,i1);
						NEXT;
					}

					INSTR(ifngt) {
						LOAD_OFFSET_AND_FETCH_SS(a1,a2,i1);
						IFNCMP_TWO_VALUES(>, core->compare(a2, a1) == trueAtom,a1,a2,i1);
						NEXT;
					}

					INSTR(ifge) {
						LOAD_OFFSET_AND_FETCH_SS(a1,a2,i1);
						IFCMP_TWO_VALUES(>=, core->compare(a1, a2) == falseAtom,a1,a2,i1);
						NEXT;
					}

					INSTR(ifnge) {
						LOAD_OFFSET_AND_FETCH_SS(a1,a2,i1);
						IFNCMP_TWO_VALUES(>=, core->compare(a1, a2) == falseAtom,a1,a2,i1);
						NEXT;
					}

					INSTR(lessthan) {
						CMP2(<, core->compare(a1,a2) == trueAtom);
						NEXT;
					}

					INSTR(lessequals) {
						CMP2(<=, core->compare(a2, a1) == falseAtom);
						NEXT;
					}

					INSTR(greaterthan) {
						CMP2(>, core->compare(a2, a1) == trueAtom);
						NEXT;
					}

					INSTR(greaterequals) {
						CMP2(>=, core->compare(a1, a2) == falseAtom);
						NEXT;
					}

					INSTR(newobject) {
						SAVE_EXPC;
						i1 = (intptr_t)U30ARG;
						a1 = env->op_newobject(sp, (int)i1)->atom();
						*(sp -= 2*i1-1) = a1;
						NEXT;
					}

					INSTR(newarray) {
						SAVE_EXPC;
						i1 = (intptr_t)U30ARG;
						a1 = avmplus::newarray(toplevel, (int)i1, sp-i1+1)->atom();
						*(sp -= i1-1) = a1;
						NEXT;
					}

					INSTR(getlex) 
					{
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);
						a1 = env->findproperty(scope, scopeBase, scopeDepth, multiname, true, withBase);
						*(++sp) = toplevel->getproperty(a1, multiname, toplevel->toVTable(a1));
						NEXT;
					}	

					// get a property using a multiname ref
					INSTR(getproperty)
					{
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);
						if (!multiname->isRuntime())
						{					
							Atom a1=*sp;
							*sp = toplevel->getproperty(*sp, multiname, toplevel->toVTable(*sp));
						}
						else if (!multiname->isRtns() && IS_INTEGER(*sp) && *sp >= 0 && AvmCore::isObject(sp[-1]))
						{
							a2 = *(sp--);	// key
							*sp = AvmCore::atomToScriptObject(*sp)->getUintProperty(UINT32_VALUE(a2));
						}
						else if(multiname->isRtns() || !AvmCore::isDictionaryLookup(*sp, *(sp-1))) 
						{
							aux_memory->multiname2 = *multiname;
							sp = initMultiname(env, aux_memory->multiname2, sp);
							*sp = toplevel->getproperty(*sp, &aux_memory->multiname2, toplevel->toVTable(*sp));
						} 
						else 
						{
							a2 = *(sp--);
							*sp = AvmCore::atomToScriptObject(*sp)->getAtomProperty(a2);
						}
						NEXT;
					}

					INSTR(setproperty) 
					{
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);

						

						a1 = *(sp--);
						if (!multiname->isRuntime())
						{
							//StUTF8String str(multiname->getName());
							//if(strstr(str.c_str(),"frameData")!=XNULL)
							//	int v=0;
							a3 = *(sp--);
							toplevel->setproperty(a3, multiname, a1, toplevel->toVTable(a3));
						}
						else if (!multiname->isRtns() && IS_INTEGER(*sp) && *sp >= 0 && AvmCore::isObject(sp[-1]))
						{
							a2 = *(sp--);		// key
							a3 = *(sp--);		// object
							AvmCore::atomToScriptObject(a3)->setUintProperty(UINT32_VALUE(a2), a1);
						}
						else if(multiname->isRtns() || !AvmCore::isDictionaryLookup(*sp, *(sp-1)))
						{
							aux_memory->multiname2 = *multiname;
							sp = initMultiname(env, aux_memory->multiname2, sp);
							a3 = *(sp--);		// object
							toplevel->setproperty(a3, &aux_memory->multiname2, a1, toplevel->toVTable(a3));
						} 
						else 
						{
							a2 = *(sp--);	// key
							a3 = *(sp--);	// object
							AvmCore::atomToScriptObject(a3)->setAtomProperty(a2, a1);
						}
						NEXT;
					}

					INSTR(initproperty)
					{
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);
						a1 = *(sp--);		// value

						if (multiname->isRuntime())
						{

							aux_memory->multiname2 = *multiname;
							sp = initMultiname(env, aux_memory->multiname2, sp);
							multiname = &aux_memory->multiname2;
						}
						a2 = *(sp--);
						env->initproperty(a2, multiname, a1, toplevel->toVTable(a2));
						NEXT;
					}

					INSTR(getdescendants) 
					{
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);
						if (multiname->isRuntime())
						{
							aux_memory->multiname2 = *multiname;
							sp = initMultiname(env, aux_memory->multiname2, sp);
							multiname = &aux_memory->multiname2;
						}
						sp[0] = env->getdescendants(sp[0], multiname);
						NEXT;
					}

					INSTR(checkfilter) {
						SAVE_EXPC;
						env->checkfilter(sp[0]);
						NEXT;
					}

					INSTR(findpropstrict) {
						b1 = true;
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);
						if (multiname->isRuntime())
						{
							aux_memory->multiname2 = *multiname;
							sp = initMultiname(env, aux_memory->multiname2, sp);
							multiname = &aux_memory->multiname2;
						}
						*(++sp) = env->findproperty(scope, scopeBase, scopeDepth, multiname, b1, withBase);
						NEXT;
					}

					INSTR(findproperty) {
						b1 = false;
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);
						if (multiname->isRuntime())
						{
							aux_memory->multiname2 = *multiname;
							sp = initMultiname(env, aux_memory->multiname2, sp);
							multiname = &aux_memory->multiname2;
						}
						*(++sp) = env->findproperty(scope, scopeBase, scopeDepth, multiname, b1, withBase);
						NEXT;
					}

					INSTR(finddef) {
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);
						*(++sp) = env->finddef(multiname)->atom();
						NEXT;
					}

					INSTR(nextname) {
						SAVE_EXPC;
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						// verifier checks for int
						sp[0] = env->nextname(a1, AvmCore::integer_i(a2));
						NEXT;
					}

					INSTR(nextvalue) {
						SAVE_EXPC;
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						// verifier checks for int
						sp[0] = env->nextvalue(a1, AvmCore::integer_i(a2));
						NEXT;
					}

					INSTR(hasnext) {
						SAVE_EXPC;
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						// verifier checks for int
						sp[0] = core->intToAtom(env->hasnext(a1, AvmCore::integer_i(a2)));
						NEXT;
					}

					INSTR(hasnext2)
					{
						SAVE_EXPC;
						u1 = U30ARG;                       // objReg
						u2  = U30ARG;                      // indexReg
						a1l = framep[u1];                  // objAtom
						i32l = AvmCore::integer(framep[u2]);  // index
						*(++sp) = env->hasnextproto(a1l, i32l) ? trueAtom : falseAtom;
						framep[u1] = a1l;
						framep[u2] = core->intToAtom(i32l);
						NEXT;
					}

					INSTR(sxi1) 
					{
						i1 = AvmCore::integer(sp[0]);
						sp[0] = CHECK_INT_ATOM(Atom(((i1 << (8*sizeof(Atom)-1)) >> ((8*sizeof(Atom)-1)-3)) | kIntptrType));
						NEXT;
					}

					INSTR(sxi8) {
						i1 = AvmCore::integer(sp[0]);
						sp[0] = CHECK_INT_ATOM(Atom(((i1 << (8*(sizeof(Atom)-1))) >> ((8*(sizeof(Atom)-1))-3)) | kIntptrType));
						NEXT;
					}

					INSTR(sxi16) {
						i1 = AvmCore::integer(sp[0]);
						sp[0] = CHECK_INT_ATOM(Atom(((i1 << (8*(sizeof(Atom)-2))) >> ((8*(sizeof(Atom)-2))-3)) | kIntptrType));
						NEXT;
					}

					INSTR(li8) {
						i1 = AvmCore::integer(sp[0]);		// i1 = addr
						MOPS_LOAD(i1, uint8_t, liz8, ub2);	// ub2 = result
						sp[0] = MAKE_INTEGER(ub2);		// always fits in atom
						NEXT;
					}

					INSTR(li16) {
						i1 = AvmCore::integer(sp[0]);		// i1 = addr
						MOPS_LOAD(i1, uint16_t, liz16, uh2l);	// uh2l = result
						sp[0] = MAKE_INTEGER(uh2l);		// always fits in atom
						NEXT;
					}

					INSTR(li32) {
						i1 = AvmCore::integer(sp[0]);		// i1 = addr
						MOPS_LOAD(i1, int32_t, li32, i32l);	// i32l = result
						sp[0] = core->intToAtom(i32l);
						NEXT;
					}

					INSTR(lf32) {
						i1 = AvmCore::integer(sp[0]);		// i1 = addr
						MOPS_LOAD(i1, float, lf32, f2l);		// f2l = result
						sp[0] = core->doubleToAtom(f2l);
						NEXT;
					}

					INSTR(lf64) {
						i1 = AvmCore::integer(sp[0]);		// i1 = addr
						MOPS_LOAD(i1, double, lf64, d2l);		// d2l = addr
						sp[0] = core->doubleToAtom(d2l);
						NEXT;
					}

					// stores
					INSTR(si8) {
						i1 = AvmCore::integer(sp[0]);		        // i1 = addr
						ub2 = (uint8_t)AvmCore::integer(sp[-1]);	// u2 = value
						MOPS_STORE(i1, uint8_t, si8, ub2);
						sp -= 2;
						NEXT;
					}

					INSTR(si16) {
						i1 = AvmCore::integer(sp[0]);		// i1 = addr
						uh2l = (uint16_t)AvmCore::integer(sp[-1]);	// uh2l = value
						MOPS_STORE(i1, uint16_t, si16, uh2l);
						sp -= 2;
						NEXT;
					}

					INSTR(si32) {
						i1 = AvmCore::integer(sp[0]);		// i1 = addr
						i32l = AvmCore::integer(sp[-1]);	// i32l = value
						MOPS_STORE(i1, uint32_t, si32, i32l);
						sp -= 2;
						NEXT;
					}

					INSTR(sf32) {
						i1 = AvmCore::integer(sp[0]);		// i1 = addr
						f2l = (float)AvmCore::number(sp[-1]);		// d2l = value
						MOPS_STORE(i1, float, sf32, f2l);
						sp -= 2;
						NEXT;
					}

					INSTR(sf64) {
						i1 = AvmCore::integer(sp[0]);
						d2l = AvmCore::number(sp[-1]);
						MOPS_STORE(i1, double, sf64, d2l);
						sp -= 2;
						NEXT;
					}

					// delete property using multiname
					INSTR(deleteproperty) {
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);
						if (!multiname->isRuntime())
						{
							sp[0] = env->delproperty(sp[0], multiname);
						}
						else if (!multiname->isRtns() && IS_INTEGER(*sp) && *sp >= 0 && AvmCore::isObject(sp[-1]))
						{
							a2 = *(sp--);	// key
							*sp = AvmCore::atomToScriptObject(*sp)->delUintProperty(UINT32_VALUE(a2)) ? trueAtom : falseAtom;
						}
						else if(multiname->isRtns() || !AvmCore::isDictionaryLookup(*sp, *(sp-1))) 
						{
							aux_memory->multiname2 = *multiname;
							sp = initMultinameNoXMLList(env, aux_memory->multiname2, sp);
							sp[0] = env->delproperty(sp[0], &aux_memory->multiname2);
						} 
						else 
						{
							a2 = *(sp--);	// key
							sp[0] = AvmCore::atomToScriptObject(sp[0])->deleteAtomProperty(a2) ? trueAtom : falseAtom;
						}
						NEXT;
					}

					INSTR(setslot) {
						SAVE_EXPC;
						a1 = sp[-1];
						a2 = sp[0];
						sp -= 2;
						env->nullcheck(a1);
						u1 = U30ARG-1;    // slot_id
						AvmCore::atomToScriptObject(a1)->coerceAndSetSlotAtom((uint32_t)u1, a2);
						NEXT;
					}

					INSTR(getslot) {
						SAVE_EXPC;
						env->nullcheck(sp[0]);
						sp[0] = AvmCore::atomToScriptObject(sp[0])->getSlotAtom((uint32_t)U30ARG-1);
						NEXT;
					}

					INSTR(setglobalslot) {
						SAVE_EXPC;
						// find the global activation scope (object at depth 0 on scope chain)
						AvmAssert(globalScope != NULL);

						// OPTIMIZEME - cleanup after ABC interpreter defenestration.
						// Perform the -1 adjustment in the bytecode translator, not here every time.
						u1 = U30ARG-1;    // slot_id
						a1 = sp[0];       // value
						sp--;
						globalScope->coerceAndSetSlotAtom((uint32_t)u1, a1);
						NEXT;
					}

					INSTR(getglobalslot) {
						SAVE_EXPC;
						// find the global activation scope (object at depth 0 on scope chain)
						AvmAssert(globalScope != NULL);

						// OPTIMIZMEME - cleanup after ABC interpreter defenestration.
						// Perform the -1 adjustment in the bytecode translator, not here every time.
						sp++;
						sp[0] = globalScope->getSlotAtom((uint32_t)(U30ARG-1));
						NEXT;
					}

					// OPTIMIZEME - presumably there are many ways in which the call opcodes may be specialized 
					// to avoid the full function prologue?

					INSTR(call) {
						SAVE_EXPC;
						i1 = (intptr_t)U30ARG; // argc
						// stack in: function, receiver, arg1, ... argN
						// stack out: result
						a1 = toplevel->op_call(sp[-i1-1]/*function*/, (int32_t)i1, sp-i1);
						*(sp = sp-i1-1) = a1;
						NEXT;
					}

					INSTR(construct) {
						SAVE_EXPC;
						i1 = (intptr_t)U30ARG;  // argc
						// stack in: function, arg1, ..., argN
						// stack out: new instance
						a1 = toplevel->op_construct(sp[-i1]/*function*/, (int32_t)i1, sp-i1);
						*(sp = sp-i1) = a1;
						NEXT;
					}

					INSTR(newfunction) {
						SAVE_EXPC;
						sp++;
						MethodInfo *body = pool->getMethodInfo((uint32_t)U30ARG);
						sp[0] = env->newfunction(body, scope, scopeBase)->atom();
						NEXT;
					}

					INSTR(newclass) {
						SAVE_EXPC;
						u1 = U30ARG;
						Traits* ctraits = pool->getClassTraits((uint32_t)u1);
						o1 = (ScriptObject*)(~7 & toplevel->coerce(sp[0], CLASS_TYPE));
						sp[0] = env->newclass(ctraits, (ClassClosure*)o1, scope, scopeBase)->atom();
						NEXT;
					}

					INSTR(callstatic) {
						SAVE_EXPC;
						// stack in: receiver, arg1..N
						// stack out: result
						u1 = U30ARG;            // method_id
						i2 = (intptr_t)U30ARG;  // argc
						env->nullcheck(sp[-i2]);
						// ISSUE if arg types were checked in verifier, this coerces again.
						f = env->abcEnv()->getMethod((uint32_t)u1);
						a1 = f->coerceEnter((int32_t)i2, sp-i2);
						*(sp -= i2) = a1;
						NEXT;
					}

					INSTR(callmethod) {
						SAVE_EXPC;
						// stack in: receiver, arg1..N
						// stack out: result
						u1 = U30ARG-1;         // disp_id
						i2 = (intptr_t)U30ARG; // argc
						a2p = sp-i2;           // atomv

						// must be a real class instance for this to be used.  primitives that have
						// methods will only have final bindings and no dispatch table.
						VTable* vtable = toplevel->toVTable(a2p[0]); // includes null check
						AvmAssert(u1 < vtable->traits->getTraitsBindings()->methodCount);
						f = vtable->methods[u1];
						// ISSUE if arg types were checked in verifier, this coerces again.
						a1 = f->coerceEnter((int32_t)i2, a2p);
						*(sp -= i2) = a1;
						NEXT;
					}

					INSTR(callproperty) {
						u1 = OP_callproperty;
callproperty_impl:
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);

						i1 = (intptr_t)U30ARG; /* argc */
						a2p = sp - i1; /* atomv */
						sp = a2p;
						if (multiname->isRuntime())
						{
							aux_memory->multiname2 = *multiname;
							sp = initMultiname(env, aux_memory->multiname2, sp);
							multiname = &aux_memory->multiname2;
						}
#ifdef _WINEMU
						//////////////////////////////////////////////////////////////////////////

						/*if ( IsFunction( multiname, *sp, NULL, "reset" ))
						{
							Stringp s=(Stringp)(a2p[2]&~7);
							StUTF16String s16(s);
							XSWFPlayer::m_pInstance->Trace(s16.c_str());
						}*/
						if ( IsFunction( multiname, *sp, "WarScene", "showScene" ))
						{
							int v = 0;
							//return 0;
						}
						//ScriptObject*obj=(ScriptObject*)( (*sp)&~7);
#endif				
						
					
						a1 = *sp;
						if (u1 == OP_callproplex)
							a2p[0] = nullObjectAtom;
#ifdef _WINEMU
						
						if (!_analyFunction||multiname->isAnyName()||multiname->isRtname())
						{
							*sp = toplevel->callproperty(a1, multiname, (int32_t)i1, a2p, toplevel->toVTable(a1));
						}
						else
						{
							StUTF8String ss(multiname->getName());
							_ANALYENTER(ss.c_str());
							*sp = toplevel->callproperty(a1, multiname, (int32_t)i1, a2p, toplevel->toVTable(a1));
						}
#else
						{

							*sp = toplevel->callproperty(a1, multiname, (int32_t)i1, a2p, toplevel->toVTable(a1));
						}
#endif

						if (u1 ==  OP_callpropvoid)
							sp--;
						NEXT;
					}

					INSTR(callproplex) {
						u1 =  OP_callproplex;
						goto callproperty_impl;
					}

					INSTR(callpropvoid) {
						u1 =  OP_callpropvoid;
						goto callproperty_impl;
					}

					INSTR(constructprop) 
					{
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);
						i1 = (intptr_t)U30ARG;  // argc
						sp -= i1;
						a2p = sp;				// argv
						if (multiname->isRuntime())
						{
							aux_memory->multiname2 = *multiname;
							sp = initMultiname(env, aux_memory->multiname2, sp);
							multiname = &aux_memory->multiname2;
							a2p[0] = *sp;
						}
						*sp = toplevel->constructprop(multiname, (int32_t)i1, a2p, toplevel->toVTable(a2p[0]));
						NEXT;
					}

					INSTR(applytype) {
						SAVE_EXPC;
						i1 = (intptr_t)U30ARG;  // argc
						a1 = op_applytype(env, sp[-i1]/*function*/, (int32_t)i1, sp-i1+1);
						*(sp = sp-i1) = a1;
						NEXT;
					}

					INSTR(callsuper) {
						u1 =  OP_callsuper;
callsuper_impl:
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);
						i1 = (intptr_t)U30ARG; /* argc */
						sp -= i1;
						a2p = sp; /* atomv */
						if (multiname->isRuntime())
						{
							aux_memory->multiname2 = *multiname;
							sp = initMultiname(env, aux_memory->multiname2, sp);
							multiname = &aux_memory->multiname2;
							a2p[0] = *sp;
						}
						env->nullcheck(a2p[0]);
						*sp = env->callsuper(multiname, (int32_t)i1, a2p);
						if (u1 ==  OP_callsupervoid)
							sp--;
						NEXT;
					}

					INSTR(callsupervoid) {
						u1 =  OP_callsupervoid;
						goto callsuper_impl;
					}

					INSTR(getsuper) {
						a1 = 0;			// not a value - instruction is getsuper
getsuper_setsuper_impl:
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);
						if (multiname->isRuntime())
						{
							aux_memory->multiname2 = *multiname;
							sp = initMultiname(env, aux_memory->multiname2, sp);
							multiname = &aux_memory->multiname2;
						}
						a2 = *(sp--);	// object
						env->nullcheck(a2);
						if (a1 == 0)
							*(++sp) = env->getsuper(a2, multiname);
						else
							env->setsuper(a2, multiname, a1);
						NEXT;
					}

					INSTR(setsuper) {
						a1 = *(sp--);	// value
						goto getsuper_setsuper_impl;
					}
					INSTR(constructsuper) {
						SAVE_EXPC;
						i1 = (intptr_t)U30ARG;  // argc
						env->nullcheck(sp[-i1]);
						env->super_init()->coerceEnter((int32_t)i1, sp-i1);
						sp -= i1+1;
						NEXT;
					}

					INSTR(pushshort)
					{
						// this just pushes an integer since we dont have short atoms
						*(++sp) = MAKE_INTEGER(((int16_t)U30ARG));
						NEXT;
					}

					INSTR(astype)
					{
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);
						sp[0] = AvmCore::astype(sp[0], getTraits(multiname, pool, toplevel, core));
						NEXT;
					}

					INSTR(astypelate) {
						SAVE_EXPC;
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						sp[0] = AvmCore::astype(a1, toplevel->toClassITraits(a2));
						NEXT;
					}

					INSTR(coerce) 
					{
						SAVE_EXPC;
						// expects a CONSTANT_Multiname cpool index
						// this is the ES4 implicit coersion
						GET_MULTINAME_PTR(multiname, U30ARG);
						sp[0] = toplevel->coerce(sp[0], getTraits(multiname, pool, toplevel, core));
						NEXT;
					}

					INSTR(coerce_o) {
						if (sp[0] == undefinedAtom)
							sp[0] = nullObjectAtom;
						NEXT;
					}

					INSTR(coerce_s) {
						a1 = sp[0];
						if (!IS_STRING(a1)) {
							SAVE_EXPC;
							sp[0] = AvmCore::isNullOrUndefined(a1) ? nullStringAtom : core->string(a1)->atom();
						}
						NEXT;
					}

					INSTR(istype) 
					{
						SAVE_EXPC;
						GET_MULTINAME_PTR(multiname, U30ARG);
						t1 = getTraits(multiname, pool, toplevel, core);	// itraits
						sp[0] = AvmCore::istypeAtom(sp[0], t1);
						NEXT;
					}

					INSTR(istypelate) {
						SAVE_EXPC;
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						sp[0] = AvmCore::istypeAtom(a1, toplevel->toClassITraits(a2));
						NEXT;
					}

					INSTR(pushbyte) {
						*(++sp) = MAKE_INTEGER((int8_t)U8ARG);
						NEXT;
					}

					INSTR(getscopeobject) 
					{
						u1 = U8ARG;  // scope_index
						*(++sp) = scopeBase[(uint32_t)u1];
						NEXT;
					}

					INSTR(getouterscope) {
						u1 = U30ARG;  // scope_index
						*(++sp) = scope->getScope((uint32_t)u1);
						NEXT;
					}

					INSTR(getglobalscope) {
						AvmAssert(globalScope != NULL);
						*(++sp) = globalScope->atom();
						NEXT;
					}

					INSTR(pushscope) {
						a1 = sp[0]; // scope
						sp--;
						if (AvmCore::isNullOrUndefined(a1)) {
							SAVE_EXPC;
							env->nullcheck(a1);
						}
						if (scopeDepth == 0 && globalScope == NULL)
						{
							AvmAssert(scope->getSize() == 0);
							globalScope = AvmCore::atomToScriptObject(a1);
						}
						scopeBase[scopeDepth++] = a1;
						NEXT;
					}

					INSTR(pushwith) {
						a1 = sp[0]; // scope object
						sp--;
						if (AvmCore::isNullOrUndefined(a1)) {
							SAVE_EXPC;
							env->nullcheck(a1);
						}
						if (!withBase)
							withBase = scopeBase+scopeDepth;
						// is it possible to have pushWith for scope 0? not sure, let's do this just in case
						if (scopeDepth == 0 && globalScope == NULL)
						{
							AvmAssert(scope->getSize() == 0);
							globalScope = AvmCore::atomToScriptObject(a1);
						}
						scopeBase[scopeDepth++] = a1;
						NEXT;
					}

					INSTR(newactivation) {
						SAVE_EXPC;
						*(++sp) = env->newActivation()->atom();
						NEXT;
					}

					INSTR(newcatch) 
					{
						SAVE_EXPC;
						u1 = U30ARG;  // catch_index
						t1 = info->abc_exceptions()->exceptions[u1].scopeTraits;
						*(++sp) = env->newcatch(t1)->atom();
						NEXT;
					}

					INSTR(popscope) 
					{
						if (!--scopeDepth)
						{
							if (scope->getSize() == 0)
								globalScope = NULL;
						}
						scopeBase[scopeDepth] = nullObjectAtom;
						if (withBase >= scopeBase + scopeDepth)
							withBase = NULL;
						NEXT;
					}

					INSTR(convert_i)
					{
						a1 = sp[0];
						if (!IS_INTEGER(a1)) {
							SAVE_EXPC;
							sp[0] = core->intAtom(a1);
						}
						else {
							sp[0] = CLAMP_32(a1);
						}
						NEXT;
					}

					INSTR(coerce_i) {
						a1 = sp[0];
						if (!IS_INTEGER(a1)) {
							SAVE_EXPC;
							sp[0] = core->intAtom(a1);
						}
						else {
							sp[0] = CLAMP_32(a1);
						}
						NEXT;
					}

					INSTR(convert_u) 
					{
						a1 = sp[0];
						if (!IS_INTEGER(a1) || a1 < 0) {
							SAVE_EXPC;
							sp[0] = core->uintAtom(a1);
						}
						NEXT;
					}

					INSTR(coerce_u) 
					{
						a1 = sp[0];
						if (!IS_INTEGER(a1) || a1 < 0) {
							SAVE_EXPC;
							sp[0] = core->uintAtom(a1);
						}
						NEXT;
					}

					INSTR(throw)
					{
						SAVE_EXPC;
						core->throwAtom(*sp--);
						// unreachable
						NEXT;
					}

					INSTR(instanceof) {
						SAVE_EXPC;
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						sp[0] = toplevel->instanceof(a1, a2);
						NEXT;
					}

					INSTR(in) {
						SAVE_EXPC;
						a1 = sp[-1];
						a2 = sp[0];
						sp--;
						sp[0] = toplevel->in_operator(a1, a2);
						NEXT;
					}

					INSTR(dxns) {
						AvmAssert(info->setsDxns());
						SAVE_EXPC;
						core->setDxns(&aux_memory->methodFrame, pool->getString((uint32_t)U30ARG));
						NEXT;
					}

					INSTR(dxnslate) {
						AvmAssert(info->setsDxns());
						SAVE_EXPC;
						core->setDxnsLate(&aux_memory->methodFrame, *sp);
						sp--;
						NEXT;
					}

					// 'OP_abs_jump' always boils away in the translation to word code, see
					// comments in WordcodeTranslator.cpp.

					INSTR(abs_jump)	{
						if (core->interruptCheck(interruptable)) {
							SAVE_EXPC;
							AvmCore::handleInterruptMethodEnv(env);
						}
						const uint8_t *target = (const uint8_t *) U30ARG;
						codeStart = pc = target;
						NEXT;
					}

				} // switch
				// illegal instruction or accidental break
				goto L_illegal_op;
			} // for


			CATCH (Exception *exception)
			{
				ExceptionHandler *handler = core->findExceptionHandler(info, expc, exception);
				pc = codeStart + handler->target;
				scopeDepth = 0;
				// in debugger builds, ensure that non-active scope entries are nulled out
				for (int i = 0, n = ms->max_scope(); i < n; ++i)
					scopeBase[i] = nullObjectAtom;
				globalScope = (scope->getSize() > 0) ? AvmCore::atomToScriptObject(scope->getScope(0)) : NULL;
				sp = scopeBase + ms->max_scope() - 1;
				*(++sp) = exception->atom;
				goto MainLoop;
			}
			END_CATCH;
			END_TRY;

				// Target of various error cases

L_illegal_op:
			AvmAssert(!"Illegal operation!");
			goto L_illegal_op;
	}


}
