"\n\
gl_FragColor=gl_FragColor*mat4(filter_cmatrix[0],filter_cmatrix[1],filter_cmatrix[2],filter_cmatrix[3],\n\
								filter_cmatrix[5],filter_cmatrix[6],filter_cmatrix[7],filter_cmatrix[8],\n\
								filter_cmatrix[10],filter_cmatrix[11],filter_cmatrix[12],filter_cmatrix[13],\n\
								filter_cmatrix[15],filter_cmatrix[16],filter_cmatrix[17],filter_cmatrix[18])\n\
								+vec4(filter_cmatrix[4],filter_cmatrix[9],filter_cmatrix[14],filter_cmatrix[19]);\n\
gl_FragColor=vec4(gl_FragColor.rgb*gl_FragColor.a,gl_FragColor.a);\n\
								//gl_FragColor = vec4(1.0,0.0,0.0,0.0);\n\
"