"													\n\
gl_FragColor = texture2D(CC_Texture0, vec2( length( v_texcoord), 0 ));   \n\
gl_FragColor.rgb = gl_FragColor.rgb*gl_FragColor.a;					  \n\
"