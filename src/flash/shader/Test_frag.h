"                               \n\
uniform vec4 u_color;								\n\
uniform	 vec4 f_bcolor;								\n\
uniform	 int filter_type;							\n\
uniform	 float filter_con[9];						\n\
uniform  float filter_cmatrix[20];					\n\
varying  vec4 v_color;								\n\
varying  vec2 v_texcoord;							\n\
varying  vec2 v_texcoord1;							\n\
varying  vec2 v_lineTexcoord;                       \n\
uniform float filter_strength;						\n\
uniform int filter_knockout;						\n\
uniform sampler2D CC_Texture0;						\n\
uniform sampler2D CC_Texture1;						\n\
\n\
void main()						\n\
{								\n\
	//gl_FragColor = v_color;		\n\
	gl_FragColor = vec4( 1.0, 0.0, 1.0f, 1.0f ); \n\
}								\n\
";