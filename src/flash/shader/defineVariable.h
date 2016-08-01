"							\n\
uniform mat4 CC_OBJMatrix;	\n\
uniform mat4 CC_TEXMatrix;	\n\
uniform sampler2D CC_Texture0;\n\
uniform sampler2D CC_Texture1;\n\
uniform	vec4 u_color;		\n\
uniform int  CC_PreAlpha;	\n\
uniform float CC_BMPFunData[8];	\n\
uniform mat4 CC_ColorTransform;	\n\
uniform float u_filterData[7]; \n\
uniform float filter_cmatrix[20];\n\
\n\
varying vec2 v_texcoord;	\n\
varying  vec2 v_texcoord1;	\n\
varying vec4 v_color;		\n\
varying vec2 v_lineTexcoord;\n\
"