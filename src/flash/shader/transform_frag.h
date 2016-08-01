"															\n\
vec4 tf = vec4(CC_ColorTransform[0]);	 \n\
//if(tf[2]!=0.0)															\n\
//{																		\n\
//	gl_FragColor = vec4(CC_ColorTransform[3]);					\n\
//	gl_FragColor.rgb = gl_FragColor.rgb*gl_FragColor.a;	\n\
//}																		\n\
//else if(tf[0]!=0.0)														\n\
//{	\n\
	vec4 mv=vec4(CC_ColorTransform[1]);									\n\
	vec4 av=vec4(CC_ColorTransform[2]);									\n\
	gl_FragColor = vec4((gl_FragColor.r*mv[0]+av[0]*gl_FragColor.a)*mv[3],\n\
	(gl_FragColor.g*mv[1]+av[1]*gl_FragColor.a)*mv[3],	\n\
	(gl_FragColor.b*mv[2]+av[2]*gl_FragColor.a)*mv[3],gl_FragColor.a*mv[3]);				\n\
//} \n\
//gl_FragColor.rgb = gl_FragColor.rgb*gl_FragColor.a;	\n\
"