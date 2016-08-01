//"                               \n\
//uniform float CC_BMPFunData[8];								\n\
//uniform int  CC_PreAlpha;									 \n\
//varying  vec2 v_texcoord;							\n\
//varying  vec2 v_texcoord1;							\n\
//uniform sampler2D CC_Texture0;						\n\
//uniform sampler2D CC_Texture1;						\n\
//\n\
//vec4 Merga()												\n\
//{															\n\
//	vec4 dc=texture2D(CC_Texture1,v_texcoord1);				\n\
//	vec4 sc=texture2D(CC_Texture0,v_texcoord);				\n\
//	vec4 mc=vec4(CC_BMPFunData[2],CC_BMPFunData[3],         \n\
//			 CC_BMPFunData[4],CC_BMPFunData[5]);                 \n\
//			 if(CC_BMPFunData[1]==1.0) mc.a=0.0;                 \n\
//				 return vec4(dc.r*(1.0-mc.r)+sc.r*mc.r,              \n\
//				 dc.g*(1.0-mc.g)+sc.g*mc.g,			\n\
//				 dc.b*(1.0-mc.b)+sc.b*mc.b,			\n\
//				 dc.a*(1.0-mc.a)+sc.a*mc.a);		\n\
//}															\n\
//vec4 MixWithAlpha()											\n\
//{															\n\
//	vec4 dc=texture2D(CC_Texture1,v_texcoord1);				\n\
//	vec4 sc=texture2D(CC_Texture0,v_texcoord);				\n\
//	return vec4(sc.rgba*dc.a);								\n\
//}															\n\
//vec4 Compare()                                              \n\
//{															\n\
//	vec4 dc=texture2D(CC_Texture1,v_texcoord1);				\n\
//	vec4 sc=texture2D(CC_Texture0,v_texcoord);				\n\
//	vec4 dc1=dc,sc1=sc;\n\
//	if(CC_BMPFunData[2]==1.0) sc1=vec4(sc.rgb*sc.a,sc.a);   \n\
//	if(CC_BMPFunData[3]==1.0) dc1=vec4(dc.rgb*sc.a,dc.a);   \n\
//	if(dc1.rgba==sc1.rgba)                                  \n\
//		return vec4(0,0,0,0);                               \n\
//	if(dc1.rgb!=sc1.rgb)									\n\
//	{														\n\
//		float r=sc.r>=dc.r?sc.r-dc.r:1.0+sc.r-dc.r;	\n\
//		float g=sc.g>=dc.g?sc.g-dc.g:1.0+sc.g-dc.g;	\n\
//		float b=sc.b>=dc.b?sc.b-dc.b:1.0+sc.b-dc.b;	\n\
//		return vec4(r,g,b,1.0);						\n\
//	}												\n\
//	float a=sc.a>dc.a?sc.a-dc.a:1.0+sc.a-dc.a;	\n\
//	return vec4(a,a,a,a);                       \n\
//}															\n\
//vec4 CopyChannel()                                          \n\
//{                                                           \n\
//	vec4 dc=texture2D(CC_Texture1,v_texcoord1);				\n\
//	vec4 sc=texture2D(CC_Texture0,v_texcoord);				\n\
//	int si=int(CC_BMPFunData[2]);\n\
//	int di=int(CC_BMPFunData[3]);\n\
//	dc[di]=sc[si];\n\
//	return dc;\n\
//}\n\
//\n\
//void main()						\n\
//{								\n\
//	 if(CC_BMPFunData[0]==0.0)									\n\
//		gl_FragColor = Merga();									\n\
//	 else if(CC_BMPFunData[0]==1.0)								\n\
//		gl_FragColor = MixWithAlpha();								\n\
//	 else if(CC_BMPFunData[0]==2.0)								\n\
//		gl_FragColor = Compare();									\n\
//	 else if(CC_BMPFunData[0]==3.0)\n\
//		gl_FragColor = CopyChannel();\n\
//	if(CC_PreAlpha!=0)										\n\
//		gl_FragColor.rgb = gl_FragColor.rgb*gl_FragColor.a;\n\
//}								\n\
//"