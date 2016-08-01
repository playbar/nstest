"	//gl_FragColor=vec4(gl_FragColor.rgb*0.8+vec3(u_filterData[2],u_filterData[3],u_filterData[4])*0.2*gl_FragColor.rgb,gl_FragColor.a);\n\
gl_FragColor=vec4(gl_FragColor.rgb+gl_FragColor.rgb*vec3(u_filterData[2],u_filterData[3],u_filterData[4]),gl_FragColor.a);		\n\
"