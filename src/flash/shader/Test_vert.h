"                               \n\
uniform	 vec4 u_color;										\n\
attribute  vec4 a_position;									\n\
attribute  vec2 a_texcoord;									\n\
attribute  vec2 a_texcoord1;								\n\
attribute  vec4 a_color;									\n\
varying  vec4 v_color;										\n\
varying  vec2 v_texcoord;									\n\
varying  vec2 v_texcoord1;									\n\
varying  vec2 v_lineTexcoord;								\n\
\n\
void main()						\n\
{								\n\
	gl_Position = CC_OBJMatrix*a_position; \n\
	//v_color = u_color;											\n\
}								\n\
";