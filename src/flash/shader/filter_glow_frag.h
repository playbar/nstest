"vec4 temp_color = (texture2D(CC_Texture0,vec2(v_texcoord + vec2(-1.0,1.0)*vec2(1.0/u_filterData[0],1.0/u_filterData[1])))\n\
 + texture2D(CC_Texture0,vec2(v_texcoord + vec2(0.0,1.0)*vec2(1.0/u_filterData[0],1.0/u_filterData[1])))\n\
 + texture2D(CC_Texture0,vec2(v_texcoord + vec2(1.0,1.0)*vec2(1.0/u_filterData[0],1.0/u_filterData[1])))\n\
 + texture2D(CC_Texture0,vec2(v_texcoord + vec2(-1.0,0.0)*vec2(1.0/u_filterData[0],1.0/u_filterData[1])))\n\
 + texture2D(CC_Texture0,vec2(v_texcoord + vec2(1.0,0.0)*vec2(1.0/u_filterData[0],1.0/u_filterData[1])))\n\
 + texture2D(CC_Texture0,vec2(v_texcoord + vec2(-1.0,-1.0)*vec2(1.0/u_filterData[0],1.0/u_filterData[1])))\n\
 + texture2D(CC_Texture0,vec2(v_texcoord + vec2(0.0,-1.0)*vec2(1.0/u_filterData[0],1.0/u_filterData[1])))\n\
 + texture2D(CC_Texture0,vec2(v_texcoord + vec2(1.0,-1.0)*vec2(1.0/u_filterData[0],1.0/u_filterData[1]))))/8.0;\n\
 float temp_a = temp_color.a;\n\
 temp_color = vec4(u_filterData[2],u_filterData[3],u_filterData[4],u_filterData[6]*temp_a); \n\
 if(u_filterData[2]+u_filterData[3]+u_filterData[4]<2.9)temp_color.rgb=temp_color.rgb*temp_a;\n\
	temp_color= vec4(temp_color.rgb*(1.0-gl_FragColor.a)+gl_FragColor.rgb,temp_color.a+gl_FragColor.a*(1.0-gl_FragColor.a));   \n\
	if((gl_FragColor.r+gl_FragColor.g+gl_FragColor.b+gl_FragColor.a==0.0)&&temp_color.a==0.0||v_color.a<0.90)temp_color =vec4(0.0,0.0,0.0,0.0);\n\
	gl_FragColor = temp_color;\n\
"