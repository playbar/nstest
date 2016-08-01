//"                               \n\
//uniform vec4 u_color;								\n\
//uniform	 vec4 f_bcolor;								\n\
//uniform	 int filter_type;							\n\
//uniform	 float filter_con[9];						\n\
//uniform  float filter_cmatrix[20];					\n\
//varying  vec4 v_color;								\n\
//varying  vec2 v_texcoord;							\n\
//varying  vec2 v_texcoord1;							\n\
//varying  vec2 v_lineTexcoord;                       \n\
//uniform float filter_strength;						\n\
//uniform int filter_knockout;						\n\
//uniform sampler2D CC_Texture0;						\n\
//uniform sampler2D CC_Texture1;						\n\
//\n\
//vec4 filter_shadiv(vec2 coordxy,float i)			\n\
//{													\n\
//vec4 ret =  vec4(1.0,1.0,1.0,1.0);					\n\
//float temp_x=u_filteroffset[0]/u_filterbounds[0];	\n\
//float temp_y=u_filteroffset[1]/u_filterbounds[1];	\n\
//ret = texture2D(CC_Texture0,(coordxy)-vec2(temp_x,temp_y)*i);	\n\
//return ret;											\n\
//}													\n\
//vec4 filter_shaglow(vec2 coordxy,float temp)		\n\
//{													\n\
//vec4 temp_color = u_color;							\n\
//vec4 ret = filter_shadiv(coordxy.xy,temp);			\n\
//if(ret.a==0.0)                                      \n\
//temp_color = ret;									\n\
//return temp_color;									\n\
//}													\n\
//vec4 filter_sha()									\n\
//{													\n\
//vec4 ret =  vec4(0.0,0.0,0.0,0.0);					\n\
//vec4 tempret = vec4(1.0,1.0,1.0,1.0);				\n\
//float imax =32.0;									\n\
//float tempi =0.0;									\n\
//float i=0.0;										\n\
//for(i=1.0;i<=imax;i++)								\n\
//{													\n\
//tempi =i/imax;										\n\
//tempret = filter_shaglow(v_texcoord.xy,tempi)/imax;	\n\
//ret +=tempret;										\n\
//}													\n\
//return ret;											\n\
//}													\n\
//vec4 Filter_Shadow()								\n\
//{													\n\
//vec4 ret = texture2D(CC_Texture0,v_texcoord);		\n\
//if(ret.a==0.0)                                      \n\
//ret = filter_sha();									\n\
//else if(filter_knockout==1)						\n\
//ret = vec4(0.0,0.0,0.0,0.0);					\n\
//return ret;											\n\
//}													\n\
//vec4 Filter_Blur()									\n\
//{													\n\
//vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);		\n\
//for(float i =-3.0; i<4.0; i++)						\n\
//{													\n\
//for(float j=-3.0;j<4.0;j++)							\n\
//{													\n\
//vec2 offset = vec2(i,j)*vec2(u_filteroffset[0]/u_filterbounds[0],u_filteroffset[1]/u_filterbounds[1])/3.0;\n\
//vec2 _xy_new = vec2(v_texcoord + offset);																\n\
//vec4 test=texture2D(CC_Texture0,_xy_new)/49.0;															\n\
//final_color += test;																					\n\
//}																										\n\
//}																										\n\
//return final_color;																						\n\
//}																										\n\
//\n\
//vec4 GlowFilter_bg(vec2 coordxy)																		\n\
//{																										\n\
//vec4 temp_color = u_color;																				\n\
//vec4 ret = texture2D(CC_Texture0,coordxy);																\n\
//if(ret.a==0.0)																				\n\
//temp_color = vec4(u_color.rgb,0.0);																						\n\
//return temp_color;																						\n\
//}																										\n\
//vec4 Filter_Blurdiv()																\n\
//{																										\n\
//vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);															\n\
//for(float i =-3.0; i<4.0; i++)																			\n\
//{											 															\n\
//for(float j=-3.0;j<4.0;j++)																				\n\
//{																										\n\
//vec2 offset = vec2(i,j)*vec2(u_filteroffset[0]/u_filterbounds[0],u_filteroffset[1]/u_filterbounds[1])/3.0;\n\
//vec2 _xy_new = vec2(v_texcoord + offset);			\n\
//vec4 test=GlowFilter_bg(_xy_new)/49.0;             \n\
//final_color += vec4(test.rgb,test.a*filter_strength);	\n\
//}													\n\
//}													\n\
//return final_color;                                 \n\
//}													\n\
//vec4 Filter_Glow()									\n\
//{													\n\
//vec4 ret =texture2D(CC_Texture0,v_texcoord);		\n\
//vec4 temp_ret = Filter_Blurdiv();                   \n\
//if(temp_ret.a>0.0&&u_filteroffset[0]<3.0)			\n\
//temp_ret=u_color;						\n\
//if(ret.a==0.0)								 		\n\
//ret = temp_ret;                                     \n\
//else if(filter_knockout==1)						\n\
//ret = vec4(0.0,0.0,0.0,0.0);					\n\
//return ret;											\n\
//}													\n\
//vec4 filter_bevelmv(float tp,vec2 cood)				\n\
//{													\n\
//vec4 ret =  vec4(0.0,0.0,0.0,0.0);					\n\
//float x =tp;										\n\
//float y =tp;										\n\
//if(v_texcoord.x>x&&v_texcoord.x<(1.0+x)	&& v_texcoord.y>y&&v_texcoord.y<(1.0+y))				\n\
//ret = texture2D(CC_Texture0,(cood)-vec2(x,y));		\n\
//return ret;											\n\
//}													\n\
//\n\
//vec4 filter_bevelinit(vec2 cood,float temp)			\n\
//{													\n\
//vec4 ret1 =  vec4(1.0,1.0,1.0,1.0);					\n\
//vec4 col = texture2D(CC_Texture0,cood);				\n\
//vec4 col1 = filter_bevelmv(temp,cood);				\n\
//vec4 col2 = filter_bevelmv(-temp,cood);				\n\
//if(col.a!=0.0)										\n\
//{													\n\
//if(col1.a==0.0&&col2.a!=0.0)						\n\
//{													\n\
//ret1=f_bcolor;                                      \n\
//}													\n\
//else if(col1.a!=0.0&&col2.a==0.0)					\n\
//{													\n\
//ret1=u_color;                                       \n\
//}													\n\
//else												\n\
//{													\n\
//ret1=col;											\n\
//}													\n\
//}													\n\
//return ret1;										\n\
//}													\n\
//vec4 filter_bevelblur()								\n\
//{													\n\
//vec4 ret =  vec4(0.0,0.0,0.0,0.0);					\n\
//vec4 tempret = vec4(1.0,1.0,1.0,1.0);				\n\
//float imax =32.0;									\n\
//float tempi =0.0;									\n\
//for(float i=0.0;i<imax;i++)							\n\
//{													\n\
//tempi = u_filteroffset[0]/u_filterbounds[0]*i/imax;	\n\
//tempret = filter_bevelinit(v_texcoord.xy,tempi)/imax;\n\
//ret +=tempret;										\n\
//}													\n\
//return ret;											\n\
//}													\n\
//vec4 Filter_Bevel()									\n\
//{													\n\
//vec4 ret1 =  vec4(1.0,1.0,1.0,1.0);					\n\
//vec4 col = texture2D(CC_Texture0,v_texcoord.xy);	\n\
//vec4 col1 = filter_bevelmv(0.04,v_texcoord.xy);		\n\
//vec4 col2 = filter_bevelmv(-0.04,v_texcoord.xy);	\n\
//if(col.a!=0.0)										\n\
//{													\n\
//if((col2.a==0.0)||(col1.a==0.0))					\n\
//{													\n\
//ret1=filter_bevelblur();							\n\
//}													\n\
//else												\n\
//{													\n\
//ret1=col;											\n\
//}													\n\
//}													\n\
//return ret1;										\n\
//}													\n\
//vec4 Filter_Convolution()							\n\
//{													\n\
//vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);		\n\
//vec2 cood[9];										\n\
//cood[0]=vec2(-1.0,1.0);cood[1]=vec2(0.0,1.0);cood[2]=vec2(1.0,1.0);				\n\
//cood[3]=vec2(-1.0,0.0);cood[4]=vec2(0.0,0.0);cood[5]=vec2(1.0,0.0);				\n\
//cood[6]=vec2(-1.0,-1.0);cood[7]=vec2(0.0,-1.0);cood[8]=vec2(1.0,-1.0);			\n\
//float test=50.0;																\n\
//final_color += texture2D(CC_Texture0,v_texcoord+cood[0]/test)*filter_con[0];	\n\
//final_color += texture2D(CC_Texture0,v_texcoord+cood[1]/test)*filter_con[1];	\n\
//final_color += texture2D(CC_Texture0,v_texcoord+cood[2]/test)*filter_con[2];	\n\
//final_color += texture2D(CC_Texture0,v_texcoord+cood[3]/test)*filter_con[3];	\n\
//final_color += texture2D(CC_Texture0,v_texcoord+cood[4]/test)*filter_con[4];	\n\
//final_color += texture2D(CC_Texture0,v_texcoord+cood[5]/test)*filter_con[5];	\n\
//final_color += texture2D(CC_Texture0,v_texcoord+cood[6]/test)*filter_con[6];	\n\
//final_color += texture2D(CC_Texture0,v_texcoord+cood[7]/test)*filter_con[7];	\n\
//final_color += texture2D(CC_Texture0,v_texcoord+cood[8]/test)*filter_con[8];	\n\
//return final_color;																\n\
//}																				\n\
//vec4 Filter_Colormatrix()														\n\
//{																				\n\
//vec4 ret = vec4(0.0,0.0,0.0,0.0);												\n\
//vec4 bgcolor =texture2D(CC_Texture0,v_texcoord);								\n\
//ret.r=bgcolor.r*filter_cmatrix[0]+bgcolor.g*filter_cmatrix[1]+					\n\
//bgcolor.b*filter_cmatrix[2]+bgcolor.a*filter_cmatrix[3]+						\n\
//filter_cmatrix[4];																\n\
//ret.g=bgcolor.r*filter_cmatrix[5]+bgcolor.g*filter_cmatrix[6]+					\n\
//bgcolor.b*filter_cmatrix[7]+bgcolor.a*filter_cmatrix[8]+						\n\
//filter_cmatrix[9];																\n\
//ret.b=bgcolor.r*filter_cmatrix[10]+bgcolor.g*filter_cmatrix[11]+				\n\
//bgcolor.b*filter_cmatrix[12]+bgcolor.a*filter_cmatrix[13]+						\n\
//filter_cmatrix[14];																\n\
//ret.a=bgcolor.r*filter_cmatrix[15]+bgcolor.g*filter_cmatrix[16]+				\n\
//bgcolor.b*filter_cmatrix[17]+bgcolor.a*filter_cmatrix[18]+						\n\
//filter_cmatrix[19];																\n\
//return ret;																		\n\
//}																				\n\
//\n\
//void main()						\n\
//{								\n\
//bool ma=true; \n\
//vec4 out_color = vec4(0.0, 0.0, 0.0, 0.0); \n\
//if( filter_type == 0|| filter_type ==4)						\n\
//out_color = Filter_Shadow();							\n\
//else if( filter_type == 1 )                                 \n\
//out_color = Filter_Blur();								\n\
//else if( filter_type == 2 )									\n\
//out_color = Filter_Glow();								\n\
//else if( filter_type == 3 || filter_type ==7)				\n\
//out_color = Filter_Bevel();								\n\
//else if( filter_type == 5 )									\n\
//out_color = Filter_Convolution();                       \n\
//else if( filter_type == 6 )									\n\
//out_color = Filter_Colormatrix();                       \n\
//if(CC_LineFlag == 1)											\n\
//{																\n\
//float l=length(v_lineTexcoord);                             \n\
//if(l>1.0) discard;											\n\
//}																			\n\
//vec4 tf = vec4(CC_ColorTransform[0]); \n\
//if(tf[2]!=0.0)															\n\
//{\n\
//if(CC_ShaderType==9) \n\
//out_color=vec4(CC_ColorTransform[3].rgb,out_color.a*CC_ColorTransform[3].a);  \n																	\n\
//else out_color = vec4(CC_ColorTransform[3]);ma=true;						\n\
//}																		\n\
//else if(tf[0]!=0.0)														\n\
//{	\n\
//vec4 mv=vec4(CC_ColorTransform[1]);									\n\
//vec4 av=vec4(CC_ColorTransform[2]);									\n\
//if(CC_ShaderType==9)\n\
//{\n\
//av=av*out_color.a;\n\
//}\n\
//out_color = vec4(out_color.r*mv[0]+av[0],out_color.g*mv[1]+av[1], out_color.b*mv[2]+av[2],out_color.a*mv[3]+av[3]);	\n\
////ma=true;\n\
//}																		\n\
//if(ma) \n\
//out_color.rgb = out_color.rgb*out_color.a;\n\
//if(CC_ClipDraw==1&&out_color.a<=0.0)	discard;\n\
//gl_FragColor = out_color;		\n\
//}								\n\
//";
" \n\
"