"		if(gl_FragColor.a==0.0)								\n\
{                       									\n\
	vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);			\n\
	for(float i =-1.0; i<2.0; i++)						 	\n\
	{											 			\n\
		for(float j=-1.0;j<2.0;j++)							\n\
		{													\n\
			vec2 offset = vec2(i,j)*vec2(1.0/u_filterData[0],1.0/u_filterData[1]);\n\
			vec2 _xy_new = vec2(v_texcoord + offset);		\n\
			vec4 temp_color = vec4(u_filterData[2],u_filterData[3],u_filterData[4],1.0);		\n\
			vec4 ret = texture2D(CC_Texture0,_xy_new);		\n\
			if(ret.a==0.0)									\n\
			temp_color = vec4(temp_color.rgb,0.0);			\n\
			vec4 test=temp_color/9.0;					  	\n\
			final_color += vec4(test.rgb,test.a);			\n\
		}													\n\
	}														\n\
	if (final_color.a>0.0)									\n\
	final_color.a=1.0;										\n\
	else final_color=vec4(0.0,0.0,0.0,0.0);					\n\
	gl_FragColor = final_color;								\n\
	}                       									\n\
	//gl_FragColor = vec4(1.0,0.0,0.0,0.0);					\n\
"