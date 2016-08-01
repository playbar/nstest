"                               \n\
	vec4 dc=texture2D(CC_Texture1,v_texcoord1);				\n\
	vec4 sc=texture2D(CC_Texture0,v_texcoord);				\n\
	gl_FragColor = vec4(sc.rgba*dc.a);						\n\
	//gl_FragColor=vec4(1,0,0,1);\n\
"