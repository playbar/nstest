LOCAL_PATH := $(call my-dir)

PROJ_PATH = $(LOCAL_PATH)/../

#缂�璇�GGFlash妯″��
include $(CLEAR_VARS)

LOCAL_MODULE := GGFlash

LOCAL_ARM_MODE := arm

#LOCAL_CFLAGS :=-Wl,--no-undefined 
LOCAL_CFLAGS += -include "../StdAfxGGlib.h" \
				-D_FULLEDIT \
				-fshort-wchar
#				-D_ARGB_PIXEL_  \
				

LOCAL_C_INCLUDES :=  \
					$(JNI_H_INCLUDE)  \
					$(LOCAL_PATH)      \
					$(LOCAL_PATH)/../GGlibrary		\
					$../../../include  \
					$../../../include/base  \
					$../../../include/base/android  \
					$../../../Resource/include  \
					$../../../dependency/mad/include  \
					$../../../dependency/zlib   \
					$../../../dependency/gzip   \
					$../../../dependency/gdx2d			\
					$../../../dependency/7z			\
					$../../../dependency/gles/gles \
					$../../../dependency/gles/gles/support \
					$../../../dependency/gles/gles/raster \
					$../../../dependency/gles/gles/shader \
					$../../../dependency/libpng \
					$../../../dependency/gles/gles/kazmath \
					$../../../dependency/gles \
					$../../../dependency/glew \
					$../../../dependency/freetype/include \
					$../../../include/dom        \
					$../../../include/flash        \
					$../../../src/flash/AVMSWF    \
					$../../../src/flash/AVMSWF/api    \
					$../../../src/flash/AVMSWF/api/flash/accessibility \
					$../../../src/flash/AVMSWF/api/flash/data  \
					$../../../src/flash/AVMSWF/api/flash/desktop \
					$../../../src/flash/AVMSWF/api/flash/display \
					$../../../src/flash/AVMSWF/api/flash/errors \
					$../../../src/flash/AVMSWF/api/flash/events \
					$../../../src/flash/AVMSWF/api/flash/external \
					$../../../src/flash/AVMSWF/api/flash/filesystem \
					$../../../src/flash/AVMSWF/api/flash/filters \
					$../../../src/flash/AVMSWF/api/flash/geom \
					$../../../src/flash/AVMSWF/api/flash/globalization \
					$../../../src/flash/AVMSWF/api/flash/html \
					$../../../src/flash/AVMSWF/api/flash/media \
					$../../../src/flash/AVMSWF/api/flash/net/dns \
					$../../../src/flash/AVMSWF/api/flash/net/drm \
					$../../../src/flash/AVMSWF/api/flash/net \
					$../../../src/flash/AVMSWF/api/flash/printing \
					$../../../src/flash/AVMSWF/api/flash/sampler \
					$../../../src/flash/AVMSWF/api/flash/security \
					$../../../src/flash/AVMSWF/api/flash/sensors \
					$../../../src/flash/AVMSWF/api/flash/system \
					$../../../src/flash/AVMSWF/api/flash/text/engine \
					$../../../src/flash/AVMSWF/api/flash/text/ime \
					$../../../src/flash/AVMSWF/api/flash/text \
					$../../../src/flash/AVMSWF/api/flash/ui \
					$../../../src/flash/AVMSWF/api/flash/utils \
					$../../../src/flash/AVMSWF/api/flash/xml \
					$../../../src/flash/AVMSWF/api/BWLoader \
					$../../../src/flash/AVMSWF/api/adobe/utils \
					$../../../src/flash/AVMSWF/api/JavaScript \
					$../../../src/flash/core       \
					$../../../src/flash/eval      \
					$../../../src/flash/MMgc         \
					$../../../src/flash/nanojit         \
					$../../../src/flash/pcre            \
					$../../../src/flash/kazmath/include \
					$../../../src/flash/platform        \
					$../../../src/flash/VMPI     \
					$../../../src/flash

					
LOCAL_LDLIBS := $(MY_LIBRARY)

#GGFlash
MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
# remove "GGAllocator.cpp" from source list
MY_FILES := $(foreach n, $(MY_FILES), $(if $(findstring GGAllocator.cpp,$(n)),,$(n)))
LOCAL_SRC_FILES += $(MY_FILES)

#AVMSWF
MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/adobe/utils/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/BWLoader/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/com/jiuyu/frameworks/utils/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/JavaScript/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/org/util/zip/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/accessibility/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/data/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/desktop/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/display/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/errors/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/events/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/external/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/filesystem/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/filters/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/geom/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/globalization/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/html/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/media/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/net/dns/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/net/drm/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/net/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/printing/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/sampler/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/security/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/sensors/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/system/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/text/engine/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/text/ime/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/text/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/ui/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/utils/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/flash/xml/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

#MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/api/*.cpp)
#MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
#LOCAL_SRC_FILES += $(MY_FILES)

LOCAL_SRC_FILES += /../../../../src/dom/XWAPText.cpp \


LOCAL_SRC_FILES += /../../../../src/flash/AVMSWF/api/AS3DataFile.cpp \
				   /../../../../src/flash/AVMSWF/api/AS3DataIO.cpp \
				   /../../../../src/flash/AVMSWF/api/AS3DataMemory.cpp \
				   /../../../../src/flash/AVMSWF/api/AS3SocketFile.cpp \
				   /../../../../src/flash/AVMSWF/api/AS3arguments.cpp \
				   /../../../../src/flash/AVMSWF/api/AS3Private.cpp \
				   /../../../../src/flash/AVMSWF/api/AS3JSON.cpp 

MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

##AVMSWF
#MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/AS3/*.cpp)
#MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += /../../../../src/flash/AVMSWF/AS3/XExternAPI.cpp

#AVMSWF/src
#MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/AVMSWF/src/*.cpp)
#MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
#MY_FILES := $(foreach n, $(MY_FILES), $(if $(findstring Inflater.cpp,$(n)),,$(n)))
#LOCAL_SRC_FILES += $(MY_FILES)

#core
#MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/core/*.cpp)
#MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
#LOCAL_SRC_FILES += $(MY_FILES)

LOCAL_SRC_FILES +=  /../../../../src/flash/core/AbcData.cpp \
					/../../../../src/flash/core/AbcEnv.cpp \
					/../../../../src/flash/core/AbcGen.cpp \
					/../../../../src/flash/core/AbcParser.cpp \
					/../../../../src/flash/core/ActionBlockConstants.cpp \
					/../../../../src/flash/core/ArrayClass.cpp \
					/../../../../src/flash/core/ArrayObject.cpp \
					/../../../../src/flash/core/atom.cpp \
					/../../../../src/flash/core/AtomArray.cpp \
					/../../../../src/flash/core/avm.cpp \
					/../../../../src/flash/core/AvmCore.cpp \
					/../../../../src/flash/core/AvmDebug.cpp \
					/../../../../src/flash/core/avmfeatures.cpp \
					/../../../../src/flash/core/AvmLog.cpp \
					/../../../../src/flash/core/avmplus.cpp \
					/../../../../src/flash/core/avmplusDebugger.cpp \
					/../../../../src/flash/core/avmplusDebuggerStubs.cpp \
					/../../../../src/flash/core/avmplusHashtable.cpp \
					/../../../../src/flash/core/avmplusProfiler.cpp \
					/../../../../src/flash/core/AvmPlusScriptableObject.cpp \
					/../../../../src/flash/core/BigInteger.cpp \
					/../../../../src/flash/core/BooleanClass.cpp \
					/../../../../src/flash/core/BuiltinTraits.cpp \
					/../../../../src/flash/core/ClassClass.cpp \
					/../../../../src/flash/core/ClassClosure.cpp \
					/../../../../src/flash/core/CodegenLIR.cpp \
					/../../../../src/flash/core/Coder.cpp \
					/../../../../src/flash/core/Date.cpp \
					/../../../../src/flash/core/DateClass.cpp \
					/../../../../src/flash/core/DateObject.cpp \
					/../../../../src/flash/core/DescribeTypeClass.cpp \
					/../../../../src/flash/core/Domain.cpp \
					/../../../../src/flash/core/DomainEnv.cpp \
					/../../../../src/flash/core/E4XNode.cpp \
					/../../../../src/flash/core/ErrorClass.cpp \
					/../../../../src/flash/core/ErrorConstants.cpp \
					/../../../../src/flash/core/Exception.cpp \
					/../../../../src/flash/core/FrameState.cpp \
					/../../../../src/flash/core/FunctionClass.cpp \
					/../../../../src/flash/core/instr.cpp \
					/../../../../src/flash/core/IntClass.cpp \
					/../../../../src/flash/core/Interpreter.cpp \
					/../../../../src/flash/core/MathClass.cpp \
					/../../../../src/flash/core/MathUtils.cpp \
					/../../../../src/flash/core/MethodClosure.cpp \
					/../../../../src/flash/core/MethodEnv.cpp \
					/../../../../src/flash/core/MethodInfo.cpp \
					/../../../../src/flash/core/Multiname.cpp \
					/../../../../src/flash/core/MultinameHashtable.cpp \
					/../../../../src/flash/core/Namespace.cpp \
					/../../../../src/flash/core/NamespaceClass.cpp \
					/../../../../src/flash/core/NamespaceSet.cpp \
					/../../../../src/flash/core/NativeFunction.cpp \
					/../../../../src/flash/core/NumberClass.cpp \
					/../../../../src/flash/core/ObjectClass.cpp \
					/../../../../src/flash/core/peephole.cpp \
					/../../../../src/flash/core/PoolObject.cpp \
					/../../../../src/flash/core/PrintWriter.cpp \
					/../../../../src/flash/core/QCache.cpp \
					/../../../../src/flash/core/RegExpClass.cpp \
					/../../../../src/flash/core/RegExpObject.cpp \
					/../../../../src/flash/core/Sampler.cpp \
					/../../../../src/flash/core/ScopeChain.cpp \
					/../../../../src/flash/core/ScriptBuffer.cpp \
					/../../../../src/flash/core/ScriptObject.cpp \
					/../../../../src/flash/core/StackTrace.cpp \
					/../../../../src/flash/core/StringBuffer.cpp \
					/../../../../src/flash/core/StringClass.cpp \
					/../../../../src/flash/core/StringObject.cpp \
					/../../../../src/flash/core/Toplevel.cpp \
					/../../../../src/flash/core/Traits.cpp \
					/../../../../src/flash/core/TypeDescriber.cpp \
					/../../../../src/flash/core/UnicodeUtils.cpp \
					/../../../../src/flash/core/VectorClass.cpp \
					/../../../../src/flash/core/Verifier.cpp \
					/../../../../src/flash/core/VTable.cpp \
					/../../../../src/flash/core/wopcodes.cpp \
					/../../../../src/flash/core/WordcodeEmitter.cpp \
					/../../../../src/flash/core/WordcodeTranslator.cpp \
					/../../../../src/flash/core/XMLClass.cpp \
					/../../../../src/flash/core/XMLListClass.cpp \
					/../../../../src/flash/core/XMLListObject.cpp \
					/../../../../src/flash/core/XMLObject.cpp \
					/../../../../src/flash/core/XMLParser16.cpp
					
LOCAL_SRC_FILES +=  /../../../../src/flash/kazmath/src/aabb.c	\
					/../../../../src/flash/kazmath/src/mat3.c	\
					/../../../../src/flash/kazmath/src/mat4.c	\
					/../../../../src/flash/kazmath/src/neon_matrix_impl.c  \
					/../../../../src/flash/kazmath/src/plane.c		\
					/../../../../src/flash/kazmath/src/quaternion.c	\
					/../../../../src/flash/kazmath/src/ray2.c		\
					/../../../../src/flash/kazmath/src/utility.c	\
					/../../../../src/flash/kazmath/src/vec2.c		\
					/../../../../src/flash/kazmath/src/vec3.c		\
					/../../../../src/flash/kazmath/src/vec4.c
		    			
				
#eval
MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/eval/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)

#MMgc
MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/MMgc/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(MY_FILES)


#nanojit

nanojit_cpu_cxxsrc = $(error Unrecognized target CPU.)
ifeq (i686,$(TARGET_CPU))
nanojit_cpu_cxxsrc := Nativei386.cpp
endif

ifeq (x86_64,$(TARGET_CPU))
nanojit_cpu_cxxsrc := NativeX64.cpp
endif

ifeq (powerpc,$(TARGET_CPU))
nanojit_cpu_cxxsrc := NativePPC.cpp
endif

ifeq (arm,$(TARGET_CPU))
nanojit_cpu_cxxsrc := NativeARM.cpp
endif

ifeq (sparc,$(TARGET_CPU))
nanojit_cpu_cxxsrc := NativeSparc.cpp
endif

LOCAL_SRC_FILES +=  /../../../../src/flash/nanojit/Allocator.cpp \
					/../../../../src/flash/nanojit/Assembler.cpp \
					/../../../../src/flash/nanojit/CodeAlloc.cpp \
					/../../../../src/flash/nanojit/Containers.cpp \
					/../../../../src/flash/nanojit/Fragmento.cpp \
					/../../../../src/flash/nanojit/LIR.cpp \
					/../../../../src/flash/nanojit/RegAlloc.cpp \
					/../../../../src/flash/nanojit/Nativei386.cpp \
					/../../../../src/flash/nanojit/NativeX64.cpp \
					/../../../../src/flash/nanojit/NativePPC.cpp \
					/../../../../src/flash/nanojit/NativeARM.cpp
#					/../../../../src/flash/nanojit/NativeSparc.cpp 
#					/../../../../src/flash/nanojit/$(nanojit_cpu_cxxsrc)
					
					
					
#MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/nanojit/*.cpp)
#MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
#LOCAL_SRC_FILES += $(MY_FILES)

#pcre
#MY_FILES := $(wildcard $(LOCAL_PATH)/../../../src/flash/pcre/*.cpp)
#MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
#LOCAL_SRC_FILES += $(MY_FILES)
LOCAL_SRC_FILES += /../../../../src/flash/pcre/pcre_chartables.cpp \
				   /../../../../src/flash/pcre/pcre_compile.cpp  \
				   /../../../../src/flash/pcre/pcre_config.cpp \
				   /../../../../src/flash/pcre/pcre_exec.cpp \
				   /../../../../src/flash/pcre/pcre_fullinfo.cpp \
				   /../../../../src/flash/pcre/pcre_get.cpp  \
				   /../../../../src/flash/pcre/pcre_globals.cpp \
				   /../../../../src/flash/pcre/pcre_info.cpp \
				   /../../../../src/flash/pcre/pcre_ord2utf8.cpp \
				   /../../../../src/flash/pcre/pcre_refcount.cpp \
				   /../../../../src/flash/pcre/pcre_study.cpp \
				   /../../../../src/flash/pcre/pcre_tables.cpp \
				   /../../../../src/flash/pcre/pcre_try_flipped.cpp \
				   /../../../../src/flash/pcre/pcre_valid_utf8.cpp \
				   /../../../../src/flash/pcre/pcre_version.cpp \
				   /../../../../src/flash/pcre/pcre_xclass.cpp \
				   /../../../../src/flash/pcre/pcre_newline.cpp

#VMPI
LOCAL_SRC_FILES += /../../../../src/flash/VMPI/MMgcPortUnix.cpp \
				   /../../../../src/flash/VMPI/UnixDebugUtils.cpp \
				   /../../../../src/flash/VMPI/AndroidDebugUtils.cpp \
				   /../../../../src/flash/VMPI/AndroidPortUtils.cpp \
				   /../../../../src/flash/VMPI/ThreadsPosix.cpp
				   
#glu
LOCAL_SRC_FILES += /../../../../src/flash/glu/dict.c \
				   /../../../../src/flash/glu/glugeom.c \
				   /../../../../src/flash/glu/memalloc.c	\
				   /../../../../src/flash/glu/mesh.c		\
				   /../../../../src/flash/glu/normal.c		\
				   /../../../../src/flash/glu/priorityq.c	\
				   /../../../../src/flash/glu/render.c		\
				   /../../../../src/flash/glu/sweep.c		\
				   /../../../../src/flash/glu/tess.c		\
				   /../../../../src/flash/glu/tessmono.c
				   
#shader
LOCAL_SRC_FILES += /../../../../src/flash/shader/bwShader.cpp \
				   /../../../../src/flash/shader/GLProgram.cpp
				   
#platform
LOCAL_SRC_FILES += /../../../../src/flash/platform/android/MathUtilsUnix.cpp
			   
#MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/flash/platform/android/*.cpp)
#MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
#LOCAL_SRC_FILES += $(MY_FILES)

#LOCAL_STATIC_LIBRARYS := GGBase

include $(BUILD_STATIC_LIBRARY)
