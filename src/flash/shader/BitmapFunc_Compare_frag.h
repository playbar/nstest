"                               \n\
	vec4 dc=texture2D(CC_Texture1,v_texcoord1);				\n\
	vec4 sc=texture2D(CC_Texture0,v_texcoord);				\n\
	vec4 dc1=dc,sc1=sc;\n\
	if(CC_BMPFunData[2]==1.0) sc1=vec4(sc.rgb*sc.a,sc.a);   \n\
	if(CC_BMPFunData[3]==1.0) dc1=vec4(dc.rgb*sc.a,dc.a);   \n\
	//gl_FragColor=vec4(1,0,0,1);\n\
	if(dc1.rgba==sc1.rgba)                                  \n\
		gl_FragColor=vec4(0.0,0.0,0.0,0.0);                               \n\
	else if(dc1.rgb!=sc1.rgb)									\n\
	{														\n\
		float r=sc.r>=dc.r?sc.r-dc.r:1.0+sc.r-dc.r;	\n\
		float g=sc.g>=dc.g?sc.g-dc.g:1.0+sc.g-dc.g;	\n\
		float b=sc.b>=dc.b?sc.b-dc.b:1.0+sc.b-dc.b;	\n\
		gl_FragColor=vec4(r,g,b,1.0);						\n\
	}												\n\
	else											\n\
	{												\n\
		float a=sc.a>dc.a?sc.a-dc.a:1.0+sc.a-dc.a;	\n\
		gl_FragColor=vec4(a,a,a,a);                       \n\
	}\n\
	"