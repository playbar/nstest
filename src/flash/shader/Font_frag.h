"													\n\
float a = texture2D(CC_Texture0,v_texcoord).a;		\n\
gl_FragColor = vec4(v_color.rgb*a,v_color.a*a);     \n\
"