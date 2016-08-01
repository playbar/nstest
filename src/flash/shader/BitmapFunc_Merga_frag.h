"                               \n\
								\n\
	vec4 dc=texture2D(CC_Texture1,v_texcoord1);				\n\
	vec4 sc=texture2D(CC_Texture0,v_texcoord);				\n\
	vec4 mc=vec4(CC_BMPFunData[2],CC_BMPFunData[3],         \n\
			     CC_BMPFunData[4],CC_BMPFunData[5]);                 \n\
	if(CC_BMPFunData[1]==1.0) mc.a=0.0;                 \n\
	gl_FragColor=vec4(dc.r*(1.0-mc.r)+sc.r*mc.r,              \n\
				 dc.g*(1.0-mc.g)+sc.g*mc.g,			\n\
				 dc.b*(1.0-mc.b)+sc.b*mc.b,			\n\
				 dc.a*(1.0-mc.a)+sc.a*mc.a);		\n\
															\n\
	//gl_FragColor=vec4(1,0,0,1);\n\
"