#ifndef __SHADER_VERT_FRAG_H__
#define __SHADER_VERT_FRAG_H__



/////////////////////////////////
// 
const GLchar * ccPositionTextureColor_vert =
"                                                           \n\
uniform	 vec4 u_color;										\n\
attribute  vec4 a_position;									\n\
attribute  vec2 a_texcoord;									\n\
attribute  vec2 a_texcoord1;								\n\
attribute  vec4 a_color;									\n\
varying  vec4 v_color;										\n\
varying  vec2 v_texcoord;									\n\
varying  vec2 v_texcoord1;									\n\
varying  vec2 v_lineTexcoord;								\n\
																	\n\
void main()															\n\
{																	\n\
	gl_Position = CC_MVPMatrix *CC_OBJMatrix*a_position;			\n\
	if( CC_ShaderType == 0 )										\n\
	{																\n\
		//v_color = vec4(u_color.rgb * u_color.a, u_color.a);		\n\
		v_color = u_color;                                          \n\
	}																\n\
	else if( CC_ShaderType == 1||CC_ShaderType==9 )									\n\
		v_texcoord = vec2(CC_TEXMatrix *a_position);				\n\
	else if( CC_ShaderType == 3 )									\n\
	{																\n\
		v_texcoord = vec2(CC_TEXMatrix *a_position);				\n\
	}																\n\
	else if( CC_ShaderType == 4 )									\n\
		v_texcoord = a_texcoord;									\n\
	else if( CC_ShaderType == 5 )									\n\
	{																\n\
		v_color = u_color;											\n\
		v_texcoord = a_texcoord;									\n\
	}																\n\
	else if(CC_ShaderType==7)										\n\
	{																\n\
		if(CC_BMPFunData[0]<=3.0){									\n\
			v_texcoord = a_texcoord;								\n\
			v_texcoord1 = a_texcoord1;}								\n\
	}																\n\
	else if( CC_ShaderType ==8)										\n\
		v_texcoord = a_texcoord;									\n\
	else if( CC_ShaderType == 100 ) //for test						\n\
	{																\n\
		v_color = a_color;											\n\
		v_texcoord = a_texcoord;									\n\
	}																\n\
	else															\n\
		v_color = vec4(a_color.rgb * a_color.a, a_color.a);			\n\
	if(CC_LineFlag==1)												\n\
	{                                                               \n\
		//vec4 lp=vec4(0.0,CC_LineWidth,0.0,0.0);					\n\
		//vec4 lp2=CC_MVPMatrix *CC_OBJMatrix*lp;					\n\
		//v_lineWidth=length(lp2);\n\
		v_lineTexcoord = a_texcoord;								\n\
	}                                                               \n\
	//v_texcoord = a_texcoord;										\n\
}																	\n\
";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const GLchar * ccPositionTextureColor_frag =
"													\n\
uniform vec4 u_color;								\n\
uniform	 vec4 f_bcolor;								\n\
uniform	 int filter_type;							\n\
uniform	 float filter_con[9];						\n\
uniform  float filter_cmatrix[20];					\n\
varying  vec4 v_color;								\n\
varying  vec2 v_texcoord;							\n\
varying  vec2 v_texcoord1;							\n\
varying  vec2 v_lineTexcoord;                       \n\
uniform float filter_strength;						\n\
uniform int filter_knockout;						\n\
uniform sampler2D CC_Texture0;						\n\
uniform sampler2D CC_Texture1;						\n\
                                                    \n\
vec4 Filter_Blur()									\n\
{													\n\
vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);		\n\
for(float i =-3.0; i<4.0; i++)						\n\
{													\n\
for(float j=-3.0;j<4.0;j++)							\n\
{													\n\
vec2 offset = vec2(i,j)*vec2(u_filteroffset[0]/u_filterbounds[0],u_filteroffset[1]/u_filterbounds[1])/3.0;\n\
vec2 _xy_new = vec2(v_texcoord + offset);																\n\
vec4 test=texture2D(CC_Texture0,_xy_new)/49.0;															\n\
final_color += test;																					\n\
}																										\n\
}																										\n\
return final_color;																						\n\
}																										\n\
																										\n\
vec4 GlowFilter_bg(vec2 coordxy)																		\n\
{																										\n\
vec4 temp_color = u_color;																				\n\
vec4 ret = texture2D(CC_Texture0,coordxy);																\n\
if(ret.a==0.0)																				\n\
temp_color = vec4(u_color.rgb,0.0);																						\n\
return temp_color;																						\n\
}																										\n\
																										\n\
vec4 Filter_Blurdiv()																\n\
{																										\n\
vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);															\n\
for(float i =-3.0; i<4.0; i++)																			\n\
{											 															\n\
for(float j=-3.0;j<4.0;j++)																				\n\
{																										\n\
vec2 offset = vec2(i,j)*vec2(u_filteroffset[0]/u_filterbounds[0],u_filteroffset[1]/u_filterbounds[1])/3.0;\n\
vec2 _xy_new = vec2(v_texcoord + offset);			\n\
vec4 test=GlowFilter_bg(_xy_new)/49.0;             \n\
final_color += vec4(test.rgb,test.a*filter_strength);	\n\
}													\n\
}													\n\
return final_color;                                 \n\
}													\n\
													\n\
													\n\
vec4 Filter_Glow()									\n\
{													\n\
vec4 ret =texture2D(CC_Texture0,v_texcoord);		\n\
vec4 temp_ret = Filter_Blurdiv();                   \n\
if(temp_ret.a>0.0&&u_filteroffset[0]<3.0)			\n\
temp_ret=u_color;						\n\
if(ret.a==0.0)								 		\n\
ret = temp_ret;                                     \n\
else if(filter_knockout==1)						\n\
ret = vec4(0.0,0.0,0.0,0.0);					\n\
return ret;											\n\
}													\n\
													\n\
													\n\
vec4 filter_shadiv(vec2 coordxy,float i)			\n\
{													\n\
vec4 ret =  vec4(1.0,1.0,1.0,1.0);					\n\
float temp_x=u_filteroffset[0]/u_filterbounds[0];	\n\
float temp_y=u_filteroffset[1]/u_filterbounds[1];	\n\
ret = texture2D(CC_Texture0,(coordxy)-vec2(temp_x,temp_y)*i);	\n\
return ret;											\n\
}													\n\
													\n\
vec4 filter_shaglow(vec2 coordxy,float temp)		\n\
{													\n\
vec4 temp_color = u_color;							\n\
vec4 ret = filter_shadiv(coordxy.xy,temp);			\n\
if(ret.a==0.0)                                      \n\
temp_color = ret;									\n\
return temp_color;									\n\
}													\n\
													\n\
vec4 filter_sha()									\n\
{													\n\
vec4 ret =  vec4(0.0,0.0,0.0,0.0);					\n\
vec4 tempret = vec4(1.0,1.0,1.0,1.0);				\n\
float imax =32.0;									\n\
float tempi =0.0;									\n\
float i=0.0;										\n\
for(i=1.0;i<=imax;i++)								\n\
{													\n\
tempi =i/imax;										\n\
tempret = filter_shaglow(v_texcoord.xy,tempi)/imax;	\n\
ret +=tempret;										\n\
}													\n\
return ret;											\n\
}													\n\
													\n\
vec4 Filter_Shadow()								\n\
{													\n\
vec4 ret = texture2D(CC_Texture0,v_texcoord);		\n\
if(ret.a==0.0)                                      \n\
ret = filter_sha();									\n\
else if(filter_knockout==1)						\n\
ret = vec4(0.0,0.0,0.0,0.0);					\n\
return ret;											\n\
}													\n\
													\n\
													\n\
vec4 filter_bevelmv(float tp,vec2 cood)				\n\
{													\n\
vec4 ret =  vec4(0.0,0.0,0.0,0.0);					\n\
float x =tp;										\n\
float y =tp;										\n\
if(v_texcoord.x>x&&v_texcoord.x<(1.0+x)				\n\
&&v_texcoord.y>y&&v_texcoord.y<(1.0+y))				\n\
ret = texture2D(CC_Texture0,(cood)-vec2(x,y));		\n\
return ret;											\n\
}													\n\
													\n\
vec4 filter_bevelinit(vec2 cood,float temp)			\n\
{													\n\
vec4 ret1 =  vec4(1.0,1.0,1.0,1.0);					\n\
vec4 col = texture2D(CC_Texture0,cood);				\n\
vec4 col1 = filter_bevelmv(temp,cood);				\n\
vec4 col2 = filter_bevelmv(-temp,cood);				\n\
if(col.a!=0.0)										\n\
{													\n\
if(col1.a==0.0&&col2.a!=0.0)						\n\
{													\n\
ret1=f_bcolor;                                      \n\
}													\n\
else if(col1.a!=0.0&&col2.a==0.0)					\n\
{													\n\
ret1=u_color;                                       \n\
}													\n\
else												\n\
{													\n\
ret1=col;											\n\
}													\n\
}													\n\
return ret1;										\n\
}													\n\
													\n\
													\n\
vec4 filter_bevelblur()								\n\
{													\n\
vec4 ret =  vec4(0.0,0.0,0.0,0.0);					\n\
vec4 tempret = vec4(1.0,1.0,1.0,1.0);				\n\
float imax =32.0;									\n\
float tempi =0.0;									\n\
for(float i=0.0;i<imax;i++)							\n\
{													\n\
tempi = u_filteroffset[0]/u_filterbounds[0]*i/imax;	\n\
tempret = filter_bevelinit(v_texcoord.xy,tempi)/imax;\n\
ret +=tempret;										\n\
}													\n\
return ret;											\n\
}													\n\
													\n\
													\n\
vec4 Filter_Bevel()									\n\
{													\n\
vec4 ret1 =  vec4(1.0,1.0,1.0,1.0);					\n\
vec4 col = texture2D(CC_Texture0,v_texcoord.xy);	\n\
vec4 col1 = filter_bevelmv(0.04,v_texcoord.xy);		\n\
vec4 col2 = filter_bevelmv(-0.04,v_texcoord.xy);	\n\
if(col.a!=0.0)										\n\
{													\n\
if((col2.a==0.0)||(col1.a==0.0))					\n\
{													\n\
ret1=filter_bevelblur();							\n\
}													\n\
else												\n\
{													\n\
ret1=col;											\n\
}													\n\
}													\n\
return ret1;										\n\
}													\n\
vec4 Filter_Convolution()							\n\
{													\n\
vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);		\n\
vec2 cood[9];										\n\
cood[0]=vec2(-1.0,1.0);cood[1]=vec2(0.0,1.0);cood[2]=vec2(1.0,1.0);				\n\
cood[3]=vec2(-1.0,0.0);cood[4]=vec2(0.0,0.0);cood[5]=vec2(1.0,0.0);				\n\
cood[6]=vec2(-1.0,-1.0);cood[7]=vec2(0.0,-1.0);cood[8]=vec2(1.0,-1.0);			\n\
float test=50.0;																\n\
final_color += texture2D(CC_Texture0,v_texcoord+cood[0]/test)*filter_con[0];	\n\
final_color += texture2D(CC_Texture0,v_texcoord+cood[1]/test)*filter_con[1];	\n\
final_color += texture2D(CC_Texture0,v_texcoord+cood[2]/test)*filter_con[2];	\n\
final_color += texture2D(CC_Texture0,v_texcoord+cood[3]/test)*filter_con[3];	\n\
final_color += texture2D(CC_Texture0,v_texcoord+cood[4]/test)*filter_con[4];	\n\
final_color += texture2D(CC_Texture0,v_texcoord+cood[5]/test)*filter_con[5];	\n\
final_color += texture2D(CC_Texture0,v_texcoord+cood[6]/test)*filter_con[6];	\n\
final_color += texture2D(CC_Texture0,v_texcoord+cood[7]/test)*filter_con[7];	\n\
final_color += texture2D(CC_Texture0,v_texcoord+cood[8]/test)*filter_con[8];	\n\
return final_color;																\n\
}																				\n\
vec4 Filter_Colormatrix()														\n\
{																				\n\
vec4 ret = vec4(0.0,0.0,0.0,0.0);												\n\
vec4 bgcolor =texture2D(CC_Texture0,v_texcoord);								\n\
																				\n\
ret.r=bgcolor.r*filter_cmatrix[0]+bgcolor.g*filter_cmatrix[1]+					\n\
bgcolor.b*filter_cmatrix[2]+bgcolor.a*filter_cmatrix[3]+						\n\
filter_cmatrix[4];																\n\
ret.g=bgcolor.r*filter_cmatrix[5]+bgcolor.g*filter_cmatrix[6]+					\n\
bgcolor.b*filter_cmatrix[7]+bgcolor.a*filter_cmatrix[8]+						\n\
filter_cmatrix[9];																\n\
ret.b=bgcolor.r*filter_cmatrix[10]+bgcolor.g*filter_cmatrix[11]+				\n\
bgcolor.b*filter_cmatrix[12]+bgcolor.a*filter_cmatrix[13]+						\n\
filter_cmatrix[14];																\n\
ret.a=bgcolor.r*filter_cmatrix[15]+bgcolor.g*filter_cmatrix[16]+				\n\
bgcolor.b*filter_cmatrix[17]+bgcolor.a*filter_cmatrix[18]+						\n\
filter_cmatrix[19];																\n\
																				\n\
return ret;																		\n\
}																				\n\
vec4 Merga()												\n\
{															\n\
	vec4 dc=texture2D(CC_Texture1,v_texcoord1);				\n\
	vec4 sc=texture2D(CC_Texture0,v_texcoord);				\n\
	vec4 mc=vec4(CC_BMPFunData[2],CC_BMPFunData[3],         \n\
		CC_BMPFunData[4],CC_BMPFunData[5]);                 \n\
		if(CC_BMPFunData[1]==1.0) mc.a=0.0;                 \n\
		return vec4(dc.r*(1.0-mc.r)+sc.r*mc.r,              \n\
						 dc.g*(1.0-mc.g)+sc.g*mc.g,			\n\
						 dc.b*(1.0-mc.b)+sc.b*mc.b,			\n\
						 dc.a*(1.0-mc.a)+sc.a*mc.a);		\n\
}															\n\
vec4 MixWithAlpha()											\n\
{															\n\
	vec4 dc=texture2D(CC_Texture1,v_texcoord1);				\n\
	vec4 sc=texture2D(CC_Texture0,v_texcoord);				\n\
	return vec4(sc.rgba*dc.a);								\n\
}															\n\
vec4 CopyChannel()                                          \n\
{                                                           \n\
	vec4 dc=texture2D(CC_Texture1,v_texcoord1);				\n\
	vec4 sc=texture2D(CC_Texture0,v_texcoord);				\n\
	int si=int(CC_BMPFunData[2]);\n\
	int di=int(CC_BMPFunData[3]);\n\
	dc[di]=sc[si];\n\
	return dc;\n\
}\n\
vec4 Compare()                                              \n\
{															\n\
	vec4 dc=texture2D(CC_Texture1,v_texcoord1);				\n\
	vec4 sc=texture2D(CC_Texture0,v_texcoord);				\n\
	vec4 dc1=dc,sc1=sc;\n\
	if(CC_BMPFunData[2]==1.0) sc1=vec4(sc.rgb*sc.a,sc.a);   \n\
	if(CC_BMPFunData[3]==1.0) dc1=vec4(dc.rgb*sc.a,dc.a);   \n\
	if(dc1.rgba==sc1.rgba)                                  \n\
        return vec4(0,0,0,0);                               \n\
	if(dc1.rgb!=sc1.rgb)									\n\
	{														\n\
		float r=sc.r>=dc.r?sc.r-dc.r:1.0+sc.r-dc.r;	\n\
		float g=sc.g>=dc.g?sc.g-dc.g:1.0+sc.g-dc.g;	\n\
		float b=sc.b>=dc.b?sc.b-dc.b:1.0+sc.b-dc.b;	\n\
		return vec4(r,g,b,1.0);						\n\
	}												\n\
    float a=sc.a>dc.a?sc.a-dc.a:1.0+sc.a-dc.a;	\n\
    return vec4(a,a,a,a);                       \n\
}															\n\
//vec2 TextureCoord(vec2 pos)			\n\
//{	//return pos;						\n\
//	vec2 coord=pos;	return coord;\n\
//	if(CC_SpreadMode==0)	\n\
//	{  \n\
//		//if(coord.x<0) coord.x=0;	\n\
//		//else if(coord.x>1) coord.x=2;\n\
//		//if(coord.y<0) coord.y=0;	\n\
//		//else if(coord.y>1) coord.y=2;\n\
//	}							\n\
//	else if(CC_SpreadMode==1)	\n\
//	{							\n\
//		if(coord.x>1)			\n\
//		{						\n\
//		}						\n\
//		else if(coord.x<0)\n\
//		{						\n\
//		}						\n\
//		if(coord.y>1)						\n\
//		{						\n\
//		}						\n\
//		else if(coord.y<0)\n\
//		{						\n\
//		}						\n\
//	}							\n\
//	else if(CC_SpreadMode==4)	\n\
//	{							\n\
//		if(coord.x<0||coord.x>1) discard;\n\
//		if(coord.y<0||coord.y>1) discard;\n\
//	}							\n\
//	return coord;				\n\
//}\n\
	                                                         \n\
void main()													\n\
{                                                           \n\
    bool ma=true;                                           \n\
    vec4 out_color = vec4(0.0, 0.0, 0.0, 0.0);              \n\
	if( CC_ShaderType == 0 )                                \n\
		out_color = v_color;								\n\
	else if( CC_ShaderType == 1||CC_ShaderType==9 )							\n\
	{													\n\
		out_color = texture2D(CC_Texture0, v_texcoord);						\n\
		if(CC_PreAlpha==0) ma=false;                                                 \n\
            //out_color = vec4(out_color.rgb*out_color.a,out_color.a);		\n\
	}																		\n\
	else if(CC_ShaderType == 3 )											\n\
	{																		\n\
		float flen = length( v_texcoord);									\n\
		vec2 tex = vec2( flen, 0 );											\n\
		out_color = texture2D(CC_Texture0, tex);                            \n\
	}																		\n\
	else if( CC_ShaderType == 4 )                                           \n\
	{                                                                       \n\
		out_color = texture2D(CC_Texture0, v_texcoord);						\n\
		if(CC_PreAlpha==0) ma=false;                                                  \n\
            //out_color = vec4(out_color.rgb*out_color.a,out_color.a);		\n\
	}                                                                       \n\
	else if( CC_ShaderType == 5 )											\n\
	{																		\n\
		vec4 color = texture2D(CC_Texture0,v_texcoord);						\n\
		//if(color.a<=0.0) discard;\n\
		out_color = vec4(v_color.rgb,v_color.a*color.a);ma=true;			\n\
	}																		\n\
	else if(CC_ShaderType==7)										\n\
	{																\n\
		if(CC_BMPFunData[0]==0.0)									\n\
			out_color = Merga();									\n\
		else if(CC_BMPFunData[0]==1.0)								\n\
			out_color = MixWithAlpha();								\n\
		else if(CC_BMPFunData[0]==2.0)								\n\
			out_color = Compare();									\n\
		else if(CC_BMPFunData[0]==3.0)\n\
			out_color = CopyChannel();ma=false;\n\
	}																\n\
	else if( CC_ShaderType == 8)									\n\
	{																\n\
		if( filter_type == 0|| filter_type ==4)						\n\
			out_color = Filter_Shadow();							\n\
		else if( filter_type == 1 )                                 \n\
			out_color = Filter_Blur();								\n\
		else if( filter_type == 2 )									\n\
			out_color = Filter_Glow();								\n\
		else if( filter_type == 3 || filter_type ==7)				\n\
			out_color = Filter_Bevel();								\n\
		else if( filter_type == 5 )									\n\
			out_color = Filter_Convolution();                       \n\
		else if( filter_type == 6 )									\n\
			out_color = Filter_Colormatrix();                       \n\
	}																\n\
	else if( CC_ShaderType == 100 ) // for test						\n\
	{																\n\
		out_color = v_color;										\n\
	}																\n\
	vec4 tf = vec4(CC_ColorTransform[0]);							\n\
	if(CC_LineFlag == 1)											\n\
	{																\n\
		float l=length(v_lineTexcoord);                             \n\
        if(l>1.0) discard;											\n\
		//float fl=l*CC_LineWidth/2.0;                              \n\
		//float il=float(int(CC_LineWidth/2.0));//step(0.0,fl);		\n\
		//float il1=il+1.0;                                         \n\
		//if(fl>=il1) discard;                                      \n\
		//else if(fl>il-0.5)	   \n\
		//{	float alpha=(il1-fl-0.5);\n\
		//	out_color = out_color*alpha;	}\n\
		//float sl=step(l);\n\
		//out_color = out_color*sl;\n\
	    //out_color = out_color*step(0.0, 1.0 - length(v_lineTexcoord));		\n\
		//out_color = out_color*(1.0 - length(v_lineTexcoord));					\n\
		//if(out_color.r == 0.0 ) discard;                                      \n\
	}																			\n\
	else if( CC_LineFlag == 2 )                                                 \n\
	{                                                                           \n\
		if(out_color.r > CC_AliasingColor.r ) out_color.r = CC_AliasingColor.r; \n\
		if(out_color.g > CC_AliasingColor.g ) out_color.g = CC_AliasingColor.g; \n\
		if(out_color.b > CC_AliasingColor.b ) out_color.b = CC_AliasingColor.b; \n\
		if(out_color.a > CC_AliasingColor.a ) out_color.a = CC_AliasingColor.a; \n\
	}\n\
	if(CC_ShaderType!=0)														\n\
	{																			\n\
		if(tf[2]!=0.0)															\n\
		{	if(CC_ShaderType==9) \n\
			out_color=vec4(CC_ColorTransform[3].rgb,out_color.a*CC_ColorTransform[3].a);  \n																	\n\
			else out_color = vec4(CC_ColorTransform[3]);ma=true;						\n\
		}																		\n\
		else if(tf[0]!=0.0)														\n\
		{	\n\
			vec4 mv=vec4(CC_ColorTransform[1]);									\n\
			vec4 av=vec4(CC_ColorTransform[2]);									\n\
			if(CC_ShaderType==9)\n\
			{\n\
				av=av*out_color.a;\n\
			}\n\
			out_color = vec4(out_color.r*mv[0]+av[0],out_color.g*mv[1]+av[1],	\n\
				out_color.b*mv[2]+av[2],out_color.a*mv[3]+av[3]);					\n\
			ma=true;\n\
		}																		\n\
	}\n\
	if(ma) \n\
        out_color.rgb = out_color.rgb*out_color.a;\n\
	if(CC_ClipDraw==1&&out_color.a<=0.0)	discard;\n\
	gl_FragColor = out_color;													\n\
}																				\n\
";
#endif

