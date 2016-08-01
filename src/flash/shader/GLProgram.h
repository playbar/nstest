#ifndef __GLPROGRAMP_H__
#define __GLPROGRAMP_H__
#include "kazmath/mat4.h"
#include "XFilter.h"
#include "raster_gl.h"
#ifdef _WIN32
#include "gl/glew.h"
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined __ANDROID__
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif


#define kCCUniformOBJMatrix_s			"CC_OBJMatrix"
#define kCCUniformTexMatrix_s			"CC_TEXMatrix"
#define kCCUniformColorTransform_s		"CC_ColorTransform"
#define kCCUniformBMPFunData_s			"CC_BMPFunData"
#define kCCUniformPreAlpha_s			"CC_PreAlpha"
#define kCCUniformTexture0_s			"CC_Texture0"
#define kCCUniformTexture1_s			"CC_Texture1"
#define kCCUniformClipDraw_s			"CC_ClipDraw"
#define kCCUniformColor_s				"u_color"

// Attribute names
#define    kCCAttributeNameColor           "a_color"
#define    kCCAttributeNamePosition        "a_position"
#define    kCCAttributeNameTexCoord        "a_texcoord"
#define	   kCCAttributeNameTexCoord1       "a_texcoord1"
#define	   kCCUniformFilterData_s	"u_filterData"
//#define	   kCCUniformFilterOffset_s        "u_filteroffset"
//#define    kCCUniformFilterBounds_s        "u_filterbounds"
//#define    kCCUniformFilterBcolors_s       "f_bcolor"
//#define    kCCUniformFiltercon_s           "filter_con"
#define    kCCUniformFiltercmatrix_s       "filter_cmatrix"
//#define    kCCUniformFiltertype_s       "filter_type"
//#define    kCCUniformFilterstrength_s       "filter_strength"
//#define    kCCUniformFilterkonckout_s       "filter_knockout"

#define    kCCAttributeNameColor           "a_color"
#define    kCCAttributeNamePosition        "a_position"
#define    kCCAttributeNameTexCoord        "a_texcoord"


enum
{
	kCCUniformOBJMatrix = 0,
	kCCUniformTexMatrix,
	kCCUniformColorTransform,
	kCCUniformBMPFunData,
	kCCUniformPreAlpha,
	kCCUniformTexture0,
	kCCUniformTexture1,
	kCCUniformClipDraw,
	kCCUniformColor,
	kCCUniformFilterData,
	kCCUniformFiltercmatrix,
	kkCCUniform_MAX,
};

enum enShaderType
{
	enShaderType_VerColor = 0,
	enShaderType_VerTexMat = 1,
	//enShaderType_VerTexMat2 = 2,
	enShaderType_RadialGradient = 3,
	enShaderType_VerTexCoord = 4,
	enShaderType_Font = 5,
	//enShaderType_VerTexCoord2 = 6,
	enShaderType_BitmapFunc = 7,
	//enShaderType_Filter = 8,
	enShaderType_ForTest = 100,
};

enum 
{
	VertexAttrib_None = 0,
	VertexAttrib_Position = 1 << 0,
	VertexAttrib_Color = 1 << 1,
	VertexAttrib_TexCoords = 1 << 2,
	VertexAttrib_TexCoords1 = 1 << 3,
};


enum enProgram
{
	Program_VerColor = 0,
	Program_VerTexMat,
	Program_RadialGradient,
	Program_VerTexCoords,
	Program_Font,
	Program_BitmapFunc,
	Program_MAX,
};


class GLProgram
{
public:
	static void SetObjMatrix(float* matrix);
	static kmMat4	kMVPMatrix;
	static GLProgram* mCurProgram;
	static int s_uCurrentShaderProgram;
	static bool initShaders();
	static void releaseShaders();
	static bool selectShader(enProgram type,int repeat,int lineFlag,int hasTransform,int hasFilter,bool isMask);
	static int selectfilterid(XFilter* pFilter,enProgram type,enBlendMode mode);
	static bool selectBitmapShader(int hasTransform,int bitmapID);
public:
	GLProgram();
	~GLProgram();

	void BindAttribLocation( GLuint index, const char* attributeName );
	bool initWithVertexShaderByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray);
	bool compileShader(GLuint * shader, GLenum type, const GLchar* source);
	bool updateUniformLocation(GLint location, GLvoid* data, unsigned int bytes);
	void updateUniforms();//GLuint* ids,char**keys,int nCount);
	bool link();
	void use();
	void unuse();

	GLuint Program() { return m_uProgram;};

//private:
	GLuint m_uProgram;
	GLuint m_uVertShader;
	GLuint m_uFragShader;
	GLint  m_uUniforms[kkCCUniform_MAX];
};

#endif
