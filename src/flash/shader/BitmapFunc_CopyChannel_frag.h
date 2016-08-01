"                               \n\
                                                        \n\
	vec4 dc=texture2D(CC_Texture1,v_texcoord1);				\n\
	vec4 sc=texture2D(CC_Texture0,v_texcoord);				\n\
	int si=int(CC_BMPFunData[2]);\n\
	int di=int(CC_BMPFunData[3]);\n\
	if(di==0) dc[0]=sc[si];\n\
	else if(di==1) dc[1]=sc[si];\n\
	else if(di==2) dc[2]=sc[si];\n\
	else dc[3]=sc[si];\n\
	gl_FragColor=dc;\n\
"