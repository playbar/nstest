"													\n\
float len0 = length(v_texcoord);  \n\
len0 = len0-float(int(len0));		 \n\
gl_FragColor = texture2D(CC_Texture0, vec2( len0, 0 ));   \n\
gl_FragColor.rgb = gl_FragColor.rgb*gl_FragColor.a;					  \n\
"