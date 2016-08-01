"														\n\
gl_FragColor = texture2D(CC_Texture0, v_texcoord);		\n\
if(CC_PreAlpha!=0)										\n\
	gl_FragColor.rgb = gl_FragColor.rgb*gl_FragColor.a; \n\
"