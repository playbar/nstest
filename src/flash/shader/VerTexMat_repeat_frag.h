"														\n\
/*vec2 coords=vec2(v_texcoord.x,v_texcoord.y);	\n\
coords.x = coords.x-float(int(coords.x));\n\
coords.y = coords.y-float(int(coords.y));*/\n\
float x = v_texcoord.x;\n\
float y = v_texcoord.y;\n\
x = x-float(int(x));\n\
y = y-float(int(y));\n\
if(x < 0.0) x = x + 1.0; \n\
if(y < 0.0) y = y + 1.0; \n\
gl_FragColor = texture2D(CC_Texture0, vec2(x,y));		\n\
if(CC_PreAlpha!=0)										\n\
	gl_FragColor.rgb = gl_FragColor.rgb*gl_FragColor.a; \n\
//	gl_FragColor = vec4(1,0,0,1);\n\
"