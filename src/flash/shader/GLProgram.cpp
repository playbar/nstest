#include "StdAfxflash.h"
#include "GLProgram.h"
#include "BwShader.h"
#include "XFilter.h"
#include "raster_gl.h"
int    GLProgram::s_uCurrentShaderProgram = -1;

///////////////////////////////////////////////////////////////////////////////////////////////////
//

struct _ProgramInterface
{
	GLProgram*  instance;
	const char* vertSource;
	const char* fragSource;
	_ProgramInterface* link;
};
kmMat4	GLProgram::kMVPMatrix={1,0,0,0,
							   0,1,0,0,
							   0,0,1,0,
							   0,0,0,1};
GLProgram* GLProgram::mCurProgram = NULL;
#define _LINKCOUNT 6
static _ProgramInterface _VerTexMat_filter[_LINKCOUNT]=
{
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_bevel_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_blur_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_colormatix_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_convolution_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_glow_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_shadow_frag,NULL}
};

static _ProgramInterface _VerTexMat_filter_line[_LINKCOUNT]=
{
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_bevel_line_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_blur_line_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_colormatix_line_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_convolution_line_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_glow_line_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_shadow_line_frag,NULL}
};

static _ProgramInterface _VerTexMat_filter_line_transform[_LINKCOUNT]=
{
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_bevel_line_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_blur_line_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_colormatix_line_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_convolution_line_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_glow_line_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_shadow_line_transform_frag,NULL}
};

static _ProgramInterface _VerTexMat_filter_transform[_LINKCOUNT]=
{
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_bevel_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_blur_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_colormatix_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_convolution_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_glow_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_filter_shadow_transform_frag,NULL}
};

static _ProgramInterface _VerTexCoords_filter[_LINKCOUNT]=
{
	{NULL,VerTexCoords_filter_vert,VerTexCoords_filter_bevel_frag,NULL},
	{NULL,VerTexCoords_filter_vert,VerTexCoords_filter_blur_frag,NULL},
	{NULL,VerTexCoords_filter_vert,VerTexCoords_filter_colormatix_frag,NULL},
	{NULL,VerTexCoords_filter_vert,VerTexCoords_filter_convolution_frag,NULL},
	{NULL,VerTexCoords_filter_vert,VerTexCoords_filter_glow_frag,NULL},
	{NULL,VerTexCoords_filter_vert,VerTexCoords_filter_shadow_frag,NULL},
};

static _ProgramInterface _VerTexCoords_filter_transform[_LINKCOUNT]=
{
	{NULL,VerTexCoords_filter_vert,VerTexCoords_filter_bevel_transform_frag,NULL},
	{NULL,VerTexCoords_filter_vert,VerTexCoords_filter_blur_transform_frag,NULL},
	{NULL,VerTexCoords_filter_vert,VerTexCoords_filter_colormatix_transform_frag,NULL},
	{NULL,VerTexCoords_filter_vert,VerTexCoords_filter_convolution_transform_frag,NULL},
	{NULL,VerTexCoords_filter_vert,VerTexCoords_filter_glow_transform_frag,NULL},
	{NULL,VerTexCoords_filter_vert,VerTexCoords_filter_shadow_transform_frag,NULL},
};

static _ProgramInterface _Font_filter[_LINKCOUNT]=
{
	{NULL,Font_filter_vert,Font_filter_bevel_frag,NULL},
	{NULL,Font_filter_vert,Font_filter_blur_frag,NULL},
	{NULL,Font_filter_vert,Font_filter_colormatix_frag,NULL},
	{NULL,Font_filter_vert,Font_filter_convolution_frag,NULL},
	{NULL,Font_filter_vert,Font_filter_glow_frag,NULL},
	{NULL,Font_filter_vert,Font_filter_shadow_frag,NULL},
};
///////////////////////////////////////////////////////////////////////////////////
static _ProgramInterface _VerTexMat_repeat_filter[_LINKCOUNT]=
{
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_bevel_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_blur_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_colormatix_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_convolution_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_glow_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_shadow_frag,NULL}
};

static _ProgramInterface _VerTexMat_repeat_filter_line[_LINKCOUNT]=
{
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_bevel_line_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_blur_line_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_colormatix_line_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_convolution_line_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_glow_line_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_shadow_line_frag,NULL}
};

static _ProgramInterface _VerTexMat_repeat_filter_line_transform[_LINKCOUNT]=
{
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_bevel_line_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_blur_line_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_colormatix_line_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_convolution_line_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_glow_line_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_shadow_line_transform_frag,NULL}
};

static _ProgramInterface _VerTexMat_repeat_filter_transform[_LINKCOUNT]=
{
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_bevel_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_blur_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_colormatix_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_convolution_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_glow_transform_frag,NULL},
	{NULL,VerTexMat_filter_vert,VerTexMat_repeat_filter_shadow_transform_frag,NULL}
};

//////////////////////////////////////////////////////////////////////////////////
static _ProgramInterface _BitmapFunc[_LINKCOUNT]=
{
	{NULL,BitmapFunc_vert,BitmapFunc_Merga_frag,NULL},
	{NULL,BitmapFunc_vert,BitmapFunc_MixWithAlpha_frag,NULL},
	{NULL,BitmapFunc_vert,BitmapFunc_Compare_frag,NULL},
	{NULL,BitmapFunc_vert,BitmapFunc_CopyChannel_frag,NULL},
	
	{NULL,NULL,NULL,NULL},
	{NULL,NULL,NULL,NULL}
};

static _ProgramInterface _programs_mask[Program_MAX][2]=
{
	{
		{NULL,NULL,NULL,NULL}, //VecColor
		{NULL,NULL,NULL,NULL},
	},
	{
		{NULL,VerTexMat_vert,VerTexMat_mask_frag,NULL}, //VerTexMat
		{NULL,VerTexMat_vert,VerTexMat_repeat_mask_frag,NULL}, //VerTexMat
	},
	{
		{NULL,NULL,NULL,NULL}, //VecColor
		{NULL,NULL,NULL,NULL}, //VecColor
	},
	{
		{NULL,VerTexCoords_vert,VerTexCoords_mask_frag,NULL},
		{NULL,NULL,NULL,NULL}, //VecColor
	},
	{
		{NULL,Font_vert,Font_mask_frag,NULL},
		{NULL,NULL,NULL,NULL}, //VecColor
	},
	{
		{NULL,NULL,NULL,NULL}, //VecColor
		{NULL,NULL,NULL,NULL}, //VecColor
	}

};


static _ProgramInterface _programs[Program_MAX][2][2][2][2]=
	{
		{//ShaderType = VecColor
			{ //No Repeat
				{//LineFlag = 0
					{//ColorTransform = 0
						{NULL,VerColor_vert,VerColor_frag,NULL},	//VecColor,No Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,With Filters
					}
				},
				{//LineFlag = 1
					{//ColorTransform = 0
						{NULL,VerColor_line_vert,VerColor_line_frag,NULL},	//VecColor,With Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,With Filters
					}
				}
			},
			{ //Repeat
				{//LineFlag = 0
					{//ColorTransform = 0
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,With Filters
					}
				},
				{//LineFlag = 1
					{//ColorTransform = 0
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,With Filters
					}
				}
			}
		},
		{//ShaderType = VerTexMat
			{//No Repeat
				{//LineFlag = 0
					{//ColorTransform = 0
						{NULL,VerTexMat_vert,VerTexMat_frag,NULL},	//VerTexMat,No Lines,No Transform,No Filters
						{NULL,NULL,NULL,_VerTexMat_filter},	//VerTexMat,No Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,VerTexMat_transform_vert,VerTexMat_transform_frag,NULL},	//VerTexMat,No Lines,With Transform,No Filters
						{NULL,NULL,NULL,_VerTexMat_filter_transform},	//VerTexMat,No Lines,With Transform,With Filters
					}
				},
				{//LineFlag = 1
					{//ColorTransform = 0
						{NULL,VerTexMat_line_vert,VerTexMat_line_frag,NULL},	//VerTexMat,With Lines,No Transform,No Filters
						{NULL,NULL,NULL,_VerTexMat_filter_line},	//VerTexMat,With Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,VerTexMat_line_transform_vert,VerTexMat_line_transform_frag,NULL},	//VerTexMat,With Lines,With Transform,No Filters
						{NULL,NULL,NULL,_VerTexMat_filter_line_transform},	//VerTexMat,With Lines,With Transform,With Filters
					}
				}
			},
			{//Repeat
				{//LineFlag = 0
					{//ColorTransform = 0
						{NULL,VerTexMat_vert,VerTexMat_repeat_frag,NULL},	//VerTexMat,No Lines,No Transform,No Filters
						{NULL,NULL,NULL,_VerTexMat_repeat_filter},	//VerTexMat,No Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,VerTexMat_transform_vert,VerTexMat_repeat_transform_frag,NULL},	//VerTexMat,No Lines,With Transform,No Filters
						{NULL,NULL,NULL,_VerTexMat_repeat_filter_transform},	//VerTexMat,No Lines,With Transform,With Filters
					}
				},
				{//LineFlag = 1
					{//ColorTransform = 0
						{NULL,VerTexMat_line_vert,VerTexMat_line_frag,NULL},	//VerTexMat,With Lines,No Transform,No Filters
						{NULL,NULL,NULL,_VerTexMat_repeat_filter_line},	//VerTexMat,With Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,VerTexMat_line_transform_vert,VerTexMat_line_transform_frag,NULL},	//VerTexMat,With Lines,With Transform,No Filters
						{NULL,NULL,NULL,_VerTexMat_repeat_filter_line_transform},	//VerTexMat,With Lines,With Transform,With Filters
					}
				}
			},
		},
		{//ShaderType = RadialGradient
			{//No Repeat
				{//LineFlag = 0
					{//ColorTransform = 0
						{NULL,RadialGradient_vert,RadialGradient_frag,NULL},	//VecColor,No Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,RadialGradient_transform_vert,RadialGradient_transform_frag,NULL},	//VecColor,No Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,With Filters
					}
				},
				{//LineFlag = 1
					{//ColorTransform = 0
						{NULL,RadialGradient_line_vert,RadialGradient_line_frag,NULL},	//VecColor,With Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,RadialGradient_line_transform_vert,RadialGradient_line_transform_frag,NULL},	//VecColor,With Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,With Filters
					}
				}
			},
			{//No Repeat
				{//LineFlag = 0
					{//ColorTransform = 0
						{NULL,RadialGradient_vert,RadialGradient_repeat_frag,NULL},	//VecColor,No Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,RadialGradient_transform_vert,RadialGradient_repeat_transform_frag,NULL},	//VecColor,No Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,With Filters
					}
				},
				{//LineFlag = 1
					{//ColorTransform = 0
						{NULL,RadialGradient_line_vert,RadialGradient_repeat_line_frag,NULL},	//VecColor,With Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,RadialGradient_line_transform_vert,RadialGradient_repeat_line_transform_frag,NULL},	//VecColor,With Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,With Filters
					}
				}
			}
		},
		{//ShaderType = VerTexCoords
			{//No Repeat
				{//LineFlag = 0
					{//ColorTransform = 0
						{NULL,VerTexCoords_vert,VerTexCoords_frag,NULL},	//VecColor,No Lines,No Transform,No Filters
						{NULL,NULL,NULL,_VerTexCoords_filter},	//VecColor,No Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,VerTexCoords_transform_vert,VerTexCoords_transform_frag,NULL},	//VecColor,No Lines,With Transform,No Filters
						{NULL,NULL,NULL,_VerTexCoords_filter_transform},	//VecColor,No Lines,With Transform,With Filters
					}
				},
				{//LineFlag = 1
					{//ColorTransform = 0
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,With Filters
					}
				}
			},
			{ //Repeat
				{//LineFlag = 0
					{//ColorTransform = 0
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,With Filters
					}
				},
				{//LineFlag = 1
					{//ColorTransform = 0
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,With Filters
					}
				}
			}
		},
		{//ShaderType =Font
			{//No Repeat
				{//LineFlag = 0
					{//ColorTransform = 0
						{NULL,Font_vert,Font_frag,NULL},	//VecColor,No Lines,No Transform,No Filters
						{NULL,NULL,NULL,_Font_filter},	//VecColor,No Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,With Filters
					}
				},
				{//LineFlag = 1
					{//ColorTransform = 0
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,With Filters
					}
				}
			},
			{ //Repeat
				{//LineFlag = 0
					{//ColorTransform = 0
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,With Filters
					}
				},
				{//LineFlag = 1
					{//ColorTransform = 0
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,With Filters
					}
				}
			}
		},
		{//ShaderType =BitmapFunc
			{
				{//LineFlag = 0
					{//ColorTransform = 0
						{NULL,NULL,NULL,_BitmapFunc},	//VecColor,No Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,With Filters
					}
				},
				{//LineFlag = 1
					{//ColorTransform = 0
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,With Filters
					}
				}
			},
			{ //Repeat
				{//LineFlag = 0
					{//ColorTransform = 0
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,No Lines,With Transform,With Filters
					}
				},
				{//LineFlag = 1
					{//ColorTransform = 0
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,No Transform,With Filters
					},
					{//ColorTransform = 1 
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,No Filters
						{NULL,NULL,NULL,NULL},	//VecColor,With Lines,With Transform,With Filters
					}
				}
			}
		},
	};

void GLProgram::SetObjMatrix(float* matrix)
{
	if(matrix==NULL)
		glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, kMVPMatrix.mat);
	else
	{
		kmMat4 objMat;
		kmMat4Multiply(&objMat,&kMVPMatrix,(kmMat4*)matrix);
		glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE,objMat.mat);
	}
}

bool GLProgram::selectBitmapShader(int hasTransform,int bitmapID)
{
	_ProgramInterface*t=&_programs[Program_BitmapFunc][0][0][hasTransform][0];
	if(t->link)
		t=&t->link[bitmapID];
	GLProgram* p = t->instance;
	if(p!=NULL)
	{
		p->use();
		mCurProgram = p;
	}
#ifdef _DEBUG
	else
	{
		abort();
	}
#endif
	//GLProgram* mCurProgram;

	return p!=NULL;
}


int GLProgram::selectfilterid(XFilter *pFilter,enProgram type,enBlendMode mode)
{
	switch(mode)
	{
	case enBlendMode_screen:
	case enBlendMode_lighten:
	case enBlendMode_add:
	//case enBlendMode_overlay:
	case enBlendMode_difference:
	case enBlendMode_subtrace:
		 return 3;
	}
	//return 0;
	if(pFilter==NULL)
		return 0;
	switch(pFilter->m_nFilterID)
	{
		case XFilterBlur:
			return 2;
			break;
		case XFilterGlow:
			if(type==Program_Font)
			{
				if(pFilter->pGlow.blurX==0.0||pFilter->pGlow.blurY==0.0)
				{
				return 0;
				}
				return 5;
			}
			else return 6;
			break;
		case XFilterDropShadow:
			if(type==Program_Font)
			{
				if(pFilter->pDropShadow.blurX==0.0||pFilter->pDropShadow.blurY==0.0)
				{
					return 0;
				}
				return 5;
			}
			else return 6;
			break;
		case XFilterBevel:
			return 1;
			break;
		case XFilterGradientGlow:
			return 5;
			break;
		case XFilterGradientBevel:
			return 1;
			break;
		case XFilterConvolution:
			return 4;
			break;
		case XFilterColorMatrix:
			return 3;
			break;
		default :return 0;
	}
	return 0;
}

bool GLProgram::selectShader(enProgram type,int repeat, int lineFlag, int hasTransform, int hasFilter,bool isMask)
{
	//return NULL;
	//if(type==Program_RadialGradient)
	//	int v=0;
	//lineFlag, hasTransform, hasFilter is 0 OR 1 
	//hasFilter = 0;
	//hasTransform = 0;
	/*repeat = 0;
	lineFlag = 0;
	hasTransform = 0;
	isMask = false;*/
	//hasFilter = 0;
	//repeat = 0;
	_ProgramInterface*t=&_programs[type][repeat][lineFlag][hasTransform][hasFilter?1:0];
	//if(isMask&&_programs_mask[type].instance)
	//	t=&_programs_mask[type];
	if(hasFilter&&t->link)
		t=&t->link[hasFilter-1];
	if(isMask)
	{
		if(_programs_mask[type][repeat].instance)
			t=&_programs_mask[type][repeat];
		else if(_programs_mask[type][0].instance)
			t=&_programs_mask[type][0];
		else
			t=&_programs[type][repeat][0][0][0];
	}
	GLProgram* p = t->instance;
	if(p!=NULL)
	{
		p->use();
		mCurProgram = p;
	}
#ifdef _DEBUG
	else
	{
		abort();
	}
#endif
	//GLProgram* mCurProgram;

	return p!=NULL;
}

bool GLProgram::initShaders()
{
	int t,l,c,f,r;
	int aCount = 0,bCount = 0,cCount = 0;
	for(t=Program_VerColor;t<Program_MAX;t++)
	{
		for(r=0;r<2;r++)
		{
			if(_programs_mask[t][r].vertSource)
			{
				_ProgramInterface&fc=_programs_mask[t][r];
				GLProgram*p=new GLProgram();
				fc.instance=p;
				//printf("VerColor_frag:%s\n",face.fragSource);
				p->initWithVertexShaderByteArray( fc.vertSource, fc.fragSource );
				p->BindAttribLocation( VertexAttrib_Position, kCCAttributeNamePosition );
				p->BindAttribLocation( VertexAttrib_Color, kCCAttributeNameColor);
				p->BindAttribLocation( VertexAttrib_TexCoords, kCCAttributeNameTexCoord );
				p->BindAttribLocation( VertexAttrib_TexCoords1, kCCAttributeNameTexCoord1 );
				p->link();
				p->updateUniforms();
				aCount++;
			}
			for(l=0;l<2;l++)
			{
				for(c=0;c<2;c++)
				{
					for(f=0;f<2;f++)
					{
						_ProgramInterface& face=_programs[t][r][l][c][f];
						if(face.instance) continue;
						if(!face.vertSource||!face.fragSource)
						{
							if(face.link)
							{
								int m;
								for(m=0;m<_LINKCOUNT;m++)
								{
									_ProgramInterface&fl=face.link[m];
									GLProgram*p=new GLProgram();
									fl.instance=p;
									//printf("VerColor_frag:%s\n",face.fragSource);
									p->initWithVertexShaderByteArray( fl.vertSource, fl.fragSource );
									p->BindAttribLocation( VertexAttrib_Position, kCCAttributeNamePosition );
									p->BindAttribLocation( VertexAttrib_Color, kCCAttributeNameColor);
									p->BindAttribLocation( VertexAttrib_TexCoords, kCCAttributeNameTexCoord );
									p->BindAttribLocation( VertexAttrib_TexCoords1, kCCAttributeNameTexCoord1 );
                                    //printf("VerColor_vert:%s\n",fl.vertSource);
                                    //printf("VerColor_frag:%s\n",fl.fragSource);
									p->link();
									p->updateUniforms();
									bCount++;
								}
							}
							continue;
						}
						
						GLProgram*p=new GLProgram();
						face.instance=p;
						//printf("VerColor_frag:%s\n",face.fragSource);
						p->initWithVertexShaderByteArray( face.vertSource, face.fragSource );
						p->BindAttribLocation( VertexAttrib_Position, kCCAttributeNamePosition );
						p->BindAttribLocation( VertexAttrib_Color, kCCAttributeNameColor);
						p->BindAttribLocation( VertexAttrib_TexCoords, kCCAttributeNameTexCoord );
						p->BindAttribLocation( VertexAttrib_TexCoords1, kCCAttributeNameTexCoord1 );
						p->link();
						p->updateUniforms();
						cCount++;
					}
				}
			}
		}
	}
	return true;
}

void GLProgram::releaseShaders()
{
	int t,r,l,c,f;
	int aCount = 0,bCount=0,cCount=0;
	for(t=Program_VerColor;t<Program_MAX;t++)
	{
		for(r=0;r<2;r++)
		{
			_ProgramInterface& fc=_programs_mask[t][r];
			if(fc.instance)
			{
				delete fc.instance;
				fc.instance = NULL;
				aCount++;
			}
			for(l=0;l<2;l++)
			{
				for(c=0;c<2;c++)
				{
					for(f=0;f<2;f++)
					{
						_ProgramInterface& face=_programs[t][r][l][c][f];
						if(!face.instance) 
						{
							if(face.link)
							{
								int m;
								for(m=0;m<_LINKCOUNT;m++)
								{
									_ProgramInterface&fl=face.link[m];
									if(fl.instance)
										delete fl.instance;
									fl.instance=NULL;
									bCount++;
								}
							}
							continue;
						}
						delete face.instance;
						face.instance = NULL;
						cCount++;
					}
				}
			}
		}
	}
	mCurProgram = NULL;
	s_uCurrentShaderProgram = -1;
}

GLProgram::GLProgram()
{
	//s_uCurrentShaderProgram = -1;
}

GLProgram::~GLProgram()
{
	unuse();
}

void GLProgram::BindAttribLocation( GLuint index, const char* attributeName )
{
	glBindAttribLocation(m_uProgram, index, attributeName);
}

bool GLProgram::initWithVertexShaderByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{
	m_uProgram = glCreateProgram();

	m_uVertShader = m_uFragShader = 0;

	if (vShaderByteArray)
	{
		compileShader(&m_uVertShader, GL_VERTEX_SHADER, vShaderByteArray);
	}

	// Create and compile fragment shader
	if (fShaderByteArray)
	{
		compileShader(&m_uFragShader, GL_FRAGMENT_SHADER, fShaderByteArray);
	}

	if (m_uVertShader)
	{
		glAttachShader(m_uProgram, m_uVertShader);
	}

	if (m_uFragShader)
	{
		glAttachShader(m_uProgram, m_uFragShader);
	}

	return true;
}

bool GLProgram::compileShader(GLuint * shader, GLenum type, const GLchar* source)
{
	GLint status;

	const GLchar *sources[] = 
	{
#ifndef _WIN32
		(type == GL_VERTEX_SHADER ? "\n"
		:"precision mediump float;precision mediump int;precision lowp sampler2D;\n"),
#endif
		//"uniform mat4 CC_OBJMatrix;	\n"
		//"uniform mat4 CC_MVPMatrix;	\n"
		/*"uniform mat4 CC_TEXMatrix;	\n"
		"uniform mat4 CC_ColorTransform;\n"
		"uniform vec4 CC_AliasingColor;	\n"
		"uniform vec2  u_filteroffset;  \n"
		"uniform vec2  u_filterbounds;  \n"
		"uniform float CC_BMPFunData[8];	\n"
		"uniform float  CC_LineWidth;  \n"
		"uniform int  CC_ShaderType;\n"
		"uniform int  CC_LineFlag;  \n"
		"uniform int  CC_ClipDraw;	\n"
		"uniform int  CC_SpreadMode;  \n"
		"uniform int  CC_PreAlpha;	\n"*/
		"//CC INCLUDES END\n\n",
		source,
	};

	*shader = glCreateShader(type);
	glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, NULL);
	glCompileShader(*shader);

	glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

	if (! status)
	{
		GLsizei length;
		glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
		GLchar* src = (GLchar *)malloc(sizeof(GLchar) * length);

		glGetShaderSource(*shader, length, NULL, src);
		//CCLog("cocos2d: ERROR: Failed to compile shader:\n%s", src);
		free(src);
        #ifndef __ANDROID__
        abort();
        #endif
	}
	return (status == GL_TRUE);
}


void GLProgram::updateUniforms()//GLuint* ids,char**keys,int nCount)
{
	/*int i;
	for(i=0;i<nCount;i++)
	{
		m_uUniforms[ids[i]] = glGetUniformLocation(m_uProgram,keys[i]);
	}*/
	m_uUniforms[kCCUniformOBJMatrix] = glGetUniformLocation(m_uProgram, kCCUniformOBJMatrix_s);
	//m_uUniforms[kCCUniformMVPMatrix] = glGetUniformLocation(m_uProgram, kCCUniformMVPMatrix_s);
	m_uUniforms[kCCUniformTexMatrix] = glGetUniformLocation(m_uProgram, kCCUniformTexMatrix_s);
	m_uUniforms[kCCUniformColorTransform] = glGetUniformLocation(m_uProgram, kCCUniformColorTransform_s);
	m_uUniforms[kCCUniformBMPFunData] = glGetUniformLocation(m_uProgram, kCCUniformBMPFunData_s);
	//m_uUniforms[kCCUniformAliasingColor] = glGetUniformLocation(m_uProgram, kCCUniformAliasingColor_s);

	m_uUniforms[kCCUniformTexture0] = glGetUniformLocation(m_uProgram, kCCUniformTexture0_s);
	m_uUniforms[kCCUniformTexture1] = glGetUniformLocation(m_uProgram, kCCUniformTexture1_s);
	//m_uUniforms[kCCUniformShaderType] = glGetUniformLocation( m_uProgram, kCCUniformShadeType_s );
	//m_uUniforms[kCCUniformLineFlag] = glGetUniformLocation( m_uProgram, kCCUniformLineFlag_s );
	m_uUniforms[kCCUniformClipDraw] = glGetUniformLocation( m_uProgram, kCCUniformClipDraw_s );

	//m_uUniforms[kCCUniformLineWidth] = glGetUniformLocation( m_uProgram,kCCUniformLineWidth_s);
	m_uUniforms[kCCUniformPreAlpha] = glGetUniformLocation( m_uProgram, kCCUniformPreAlpha_s );
	m_uUniforms[kCCUniformColor] = glGetUniformLocation( m_uProgram, kCCUniformColor_s );
	//m_uUniforms[kCCUniformPointSize] = glGetUniformLocation( m_uProgram, kCCUniformPointSize_s );
	m_uUniforms[kCCUniformFilterData] = glGetUniformLocation( m_uProgram, kCCUniformFilterData_s );
	//m_uUniforms[kCCUniformFilterOffset] = glGetUniformLocation( m_uProgram, kCCUniformFilterOffset_s );
	//m_uUniforms[kCCUniformFilterBounds] = glGetUniformLocation( m_uProgram, kCCUniformFilterBounds_s );
	//m_uUniforms[kCCUniformFilterBcolors] = glGetUniformLocation( m_uProgram, kCCUniformFilterBcolors_s );
	//m_uUniforms[kCCUniformFiltercon] = glGetUniformLocation( m_uProgram, kCCUniformFiltercon_s );
	m_uUniforms[kCCUniformFiltercmatrix] = glGetUniformLocation( m_uProgram, kCCUniformFiltercmatrix_s );
//	m_uUniforms[kCCUniformFiltertype] = glGetUniformLocation( m_uProgram, kCCUniformFiltertype_s );
	//m_uUniforms[kCCUniformFilterstrength] = glGetUniformLocation( m_uProgram, kCCUniformFilterstrength_s );
	//m_uUniforms[kCCUniformFilterkonckout] = glGetUniformLocation( m_uProgram, kCCUniformFilterkonckout_s );
    
	use();

	glUniform1i( (GLint)m_uUniforms[kCCUniformTexture0], 0);
	glUniform1i( (GLint)m_uUniforms[kCCUniformTexture1], 1);
	//glUniform1i( (GLint)m_uUniforms[kCCUniformLineFlag], 0);
	//glUniform1i( (GLint)m_uUniforms[kCCUniformPreAlpha], 0);
	//glUniform1i( (GLint)m_uUniforms[kCCUniformClipDraw], 0);
	return;
}


bool GLProgram::link()
{
	glLinkProgram(m_uProgram);

	GLint status = GL_TRUE;
	glGetProgramiv(m_uProgram, GL_LINK_STATUS, &status );
	if ( status == GL_FALSE ) {
		int i = 0;
	}

	if (m_uVertShader)
	{
		glDeleteShader(m_uVertShader);
	}

	if (m_uFragShader)
	{
		glDeleteShader(m_uFragShader);
	}

	m_uVertShader = m_uFragShader = 0;
	return true;
}

void GLProgram::use()
{
	if ( m_uProgram != s_uCurrentShaderProgram )
	{
		s_uCurrentShaderProgram = m_uProgram;
		glUseProgram( m_uProgram );
	}
}

void GLProgram::unuse()
{
	if ( m_uProgram == s_uCurrentShaderProgram )
	{
		s_uCurrentShaderProgram = -1;
	}
	glDeleteProgram( m_uProgram );
}


