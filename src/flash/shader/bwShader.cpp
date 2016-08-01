#include "bwShader.h"

const char *VerColor_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerColor_vert.h"
#include "mainend.h"
const char *VerColor_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerColor_frag.h"
#include "mainend.h"

const char *VerColor_line_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerColor_vert.h"
#include "line_vert.h"
#include "mainend.h"
const char *VerColor_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "line_frag.h"
#include "VerColor_frag.h"
#include "mainend.h"

///////////////////////////////////////////////////////
const char *VerTexMat_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_vert.h"
#include "mainend.h"
const char *VerTexMat_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "mainend.h"

const char *VerTexMat_mask_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "mask_frag.h"
#include "mainend.h"

const char *VerTexMat_line_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_vert.h"
#include "line_vert.h"
#include "mainend.h"
const char *VerTexMat_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "line_frag.h"
#include "mainend.h"

const char *VerTexMat_transform_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_vert.h"
#include "transform_vert.h"
#include "mainend.h"
const char *VerTexMat_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_line_transform_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_vert.h"
#include "line_vert.h"
#include "transform_vert.h"
#include "mainend.h"
const char *VerTexMat_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"

//////////////////////////////////////////////////////////////////////////
const char *VerTexMat_filter_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_vert.h"
#include "filter_vert.h"
#include "mainend.h"
const char *VerTexMat_filter_bevel_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_bevel_frag.h"
#include "mainend.h"

const char *VerTexMat_filter_blur_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_blur_frag.h"
#include "mainend.h"

const char *VerTexMat_filter_colormatix_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_colormatix_frag.h"
#include "mainend.h"

const char *VerTexMat_filter_convolution_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_convolution_frag.h"
#include "mainend.h"

const char *VerTexMat_filter_glow_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_glow_frag.h"
#include "mainend.h"

const char *VerTexMat_filter_shadow_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_shadow_frag.h"
#include "mainend.h"

//////////////////////////////////////////////////////////////////////////

const char *VerTexMat_filter_transform_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_vert.h"
#include "filter_vert.h"
#include "transform_vert.h"
#include "mainend.h"
const char *VerTexMat_filter_bevel_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_bevel_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_filter_blur_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_blur_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_filter_colormatix_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_colormatix_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_filter_convolution_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_convolution_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_filter_glow_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_glow_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_filter_shadow_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_shadow_frag.h"
#include "transform_frag.h"
#include "mainend.h"

//////////////////////////////////////////////////////////////////////////

const char *VerTexMat_filter_line_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_vert.h"
#include "line_vert.h"
#include "filter_vert.h"
#include "mainend.h"

const char *VerTexMat_filter_bevel_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_bevel_frag.h"
#include "line_frag.h"
#include "mainend.h"
const char *VerTexMat_filter_blur_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_blur_frag.h"
#include "line_frag.h"
#include "mainend.h"
const char *VerTexMat_filter_colormatix_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_colormatix_frag.h"
#include "line_frag.h"
#include "mainend.h"
const char *VerTexMat_filter_convolution_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_convolution_frag.h"
#include "line_frag.h"
#include "mainend.h"
const char *VerTexMat_filter_glow_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_glow_frag.h"
#include "line_frag.h"
#include "mainend.h"
const char *VerTexMat_filter_shadow_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_shadow_frag.h"
#include "line_frag.h"
#include "mainend.h"

//////////////////////////////////////////////////////////////////////////

const char *VerTexMat_filter_line_transform_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_vert.h"
#include "filter_vert.h"
#include "line_vert.h"
#include "transform_vert.h"
#include "mainend.h"
const char *VerTexMat_filter_bevel_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_bevel_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_filter_blur_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_blur_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_filter_colormatix_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_colormatix_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"
const char *VerTexMat_filter_convolution_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_convolution_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"
const char *VerTexMat_filter_glow_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_glow_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"
const char *VerTexMat_filter_shadow_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_frag.h"
#include "filter_shadow_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"
///////////////////////////////////////////////////////
const char *VerTexMat_repeat_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_mask_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "mask_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "line_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"

//////////////////////////////////////////////////////////////////////////
const char *VerTexMat_repeat_filter_bevel_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_bevel_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_filter_blur_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_blur_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_filter_colormatix_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_colormatix_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_filter_convolution_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_convolution_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_filter_glow_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_glow_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_filter_shadow_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_shadow_frag.h"
#include "mainend.h"

//////////////////////////////////////////////////////////////////////////

const char *VerTexMat_repeat_filter_bevel_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_bevel_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_filter_blur_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_blur_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_filter_colormatix_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_colormatix_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_filter_convolution_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_convolution_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_filter_glow_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_glow_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_filter_shadow_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_shadow_frag.h"
#include "transform_frag.h"
#include "mainend.h"

//////////////////////////////////////////////////////////////////////////
const char *VerTexMat_repeat_filter_bevel_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_bevel_frag.h"
#include "line_frag.h"
#include "mainend.h"
const char *VerTexMat_repeat_filter_blur_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_blur_frag.h"
#include "line_frag.h"
#include "mainend.h"
const char *VerTexMat_repeat_filter_colormatix_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_colormatix_frag.h"
#include "line_frag.h"
#include "mainend.h"
const char *VerTexMat_repeat_filter_convolution_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_convolution_frag.h"
#include "line_frag.h"
#include "mainend.h"
const char *VerTexMat_repeat_filter_glow_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_glow_frag.h"
#include "line_frag.h"
#include "mainend.h"
const char *VerTexMat_repeat_filter_shadow_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_shadow_frag.h"
#include "line_frag.h"
#include "mainend.h"

//////////////////////////////////////////////////////////////////////////
const char *VerTexMat_repeat_filter_bevel_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_bevel_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_filter_blur_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_blur_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *VerTexMat_repeat_filter_colormatix_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_colormatix_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"
const char *VerTexMat_repeat_filter_convolution_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_convolution_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"
const char *VerTexMat_repeat_filter_glow_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_glow_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"
const char *VerTexMat_repeat_filter_shadow_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexMat_repeat_frag.h"
#include "filter_shadow_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"
////////////////////////////////////////////////////////
const char *RadialGradient_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "RadialGradient_vert.h"
#include "mainend.h"
const char *RadialGradient_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "RadialGradient_frag.h"
#include "mainend.h"

const char *RadialGradient_line_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "RadialGradient_vert.h"
#include "line_vert.h"
#include "mainend.h"
const char *RadialGradient_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "RadialGradient_frag.h"
#include "line_frag.h"
#include "mainend.h"

const char *RadialGradient_line_transform_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "RadialGradient_vert.h"
#include "line_vert.h"
#include "transform_vert.h"
#include "mainend.h"
const char *RadialGradient_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "RadialGradient_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *RadialGradient_transform_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "RadialGradient_vert.h"
#include "transform_vert.h"
#include "mainend.h"
const char *RadialGradient_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "RadialGradient_frag.h"
#include "transform_frag.h"
#include "mainend.h"
////////////////////////////////////////////////////////
const char *RadialGradient_repeat_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "RadialGradient_repeat_frag.h"
#include "mainend.h"

const char *RadialGradient_repeat_line_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "RadialGradient_repeat_frag.h"
#include "line_frag.h"
#include "mainend.h"

const char *RadialGradient_repeat_line_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "RadialGradient_repeat_frag.h"
#include "line_frag.h"
#include "transform_frag.h"
#include "mainend.h"

const char *RadialGradient_repeat_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "RadialGradient_repeat_frag.h"
#include "transform_frag.h"
#include "mainend.h"
////////////////////////////////////////////////////////
const char *VerTexCoords_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_vert.h"
#include "mainend.h"
const char *VerTexCoords_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "mainend.h"

const char *VerTexCoords_mask_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "mask_frag.h"
#include "mainend.h"

const char *VerTexCoords_transform_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_vert.h"
#include "transform_vert.h"
#include "mainend.h"
const char *VerTexCoords_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "transform_frag.h"
#include "mainend.h"

//////////////////////////////////////////////////////////////////////////
const char *VerTexCoords_filter_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_vert.h"
#include "filter_vert.h"
#include "mainend.h"
const char *VerTexCoords_filter_bevel_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "filter_bevel_frag.h"
#include "mainend.h"
const char *VerTexCoords_filter_blur_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "filter_blur_frag.h"
#include "mainend.h"
const char *VerTexCoords_filter_colormatix_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "filter_colormatix_frag.h"
#include "mainend.h"
const char *VerTexCoords_filter_convolution_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "filter_convolution_frag.h"
#include "mainend.h"
const char *VerTexCoords_filter_glow_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "filter_glow_frag.h"
#include "mainend.h"
const char *VerTexCoords_filter_shadow_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "filter_shadow_frag.h"
#include "mainend.h"

//////////////////////////////////////////////////////////////////////////

const char *VerTexCoords_filter_transform_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_vert.h"
#include "filter_vert.h"
#include "transform_vert.h"
#include "mainend.h"
const char *VerTexCoords_filter_bevel_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "filter_bevel_frag.h"
#include "transform_frag.h"
#include "mainend.h"
const char *VerTexCoords_filter_blur_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "filter_blur_frag.h"
#include "transform_frag.h"
#include "mainend.h"
const char *VerTexCoords_filter_colormatix_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "filter_colormatix_frag.h"
#include "transform_frag.h"
#include "mainend.h"
const char *VerTexCoords_filter_convolution_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "filter_convolution_frag.h"
#include "transform_frag.h"
#include "mainend.h"
const char *VerTexCoords_filter_glow_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "filter_glow_frag.h"
#include "transform_frag.h"
#include "mainend.h"
const char *VerTexCoords_filter_shadow_transform_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "VerTexCoords_frag.h"
#include "filter_shadow_frag.h"
#include "transform_frag.h"
#include "mainend.h"

/////////////////////////////////////////////////////////
const char *Font_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "Font_vert.h"
#include "mainend.h"
const char *Font_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "Font_frag.h"
#include "mainend.h"

const char *Font_mask_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "Font_frag.h"
#include "mask_frag.h"
#include "mainend.h"


//////////////////////////////////////////////////////////////////////////
const char *Font_filter_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "Font_vert.h"
#include "filter_vert.h"
#include "mainend.h"
const char *Font_filter_bevel_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "Font_frag.h"
#include "filter_bevel_frag.h"
#include "mainend.h"
const char *Font_filter_blur_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "Font_frag.h"
#include "filter_blur_frag.h"
#include "mainend.h"
const char *Font_filter_colormatix_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "Font_frag.h"
#include "filter_colormatix_frag.h"
#include "mainend.h"
const char *Font_filter_convolution_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "Font_frag.h"
#include "filter_convolution_frag.h"
#include "mainend.h"
const char *Font_filter_glow_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "Font_frag.h"
#include "filter_glow_frag.h"
#include "mainend.h"
const char *Font_filter_shadow_frag = 
#include "defineVariable.h"
#include "mainbegin.h"
#include "Font_frag.h"
#include "filter_shadow_frag.h"
#include "mainend.h"
/////////////////////////////////////////////////////////
const char *BitmapFunc_vert =
#include "defineAttribute.h"
#include "defineVariable.h"
#include "mainbegin.h"
#include "BitmapFunc_vert.h"
#include "mainend.h"
const char *BitmapFunc_Merga_frag = 
"//Bitmap Frag Shader(Merga)"
#include "defineVariable.h"
#include "mainbegin.h"
#include "BitmapFunc_Merga_frag.h"
#include "mainend.h"

const char *BitmapFunc_Compare_frag = 
"//Bitmap Frag Shader(Compare)"
#include "defineVariable.h"
#include "mainbegin.h"
#include "BitmapFunc_Compare_frag.h"
#include "mainend.h"

const char *BitmapFunc_CopyChannel_frag = 
"//Bitmap Frag Shader(CopyChannel)"
#include "defineVariable.h"
#include "mainbegin.h"
#include "BitmapFunc_CopyChannel_frag.h"
#include "mainend.h"

const char *BitmapFunc_MixWithAlpha_frag = 
"//Bitmap Frag Shader(MixWithAlpha)"
#include "defineVariable.h"
#include "mainbegin.h"
#include "BitmapFunc_MixWithAlpha_frag.h"
#include "mainend.h"

//////////////////////////////////////////////////////////////////////////
//const char *Filter_vert = 
//#include "defineAttribute.h"
//#include "defineVariable.h"
//#include "mainbegin.h"
//#include "filter_vert.h"
//#include "mainend.h"
//const char *Filter_Bevel_frag =
//#include "defineVariable.h"
//#include "mainbegin.h"
//#include "filter_bevel_frag.h"
//#include "mainend.h"
//const char *Filter_Blur_frag =
//#include "defineVariable.h"
//#include "mainbegin.h"
//#include "filter_blur_frag.h"
//#include "mainend.h"
//const char *Filter_Colormatix_frag =
//#include "defineVariable.h"
//#include "mainbegin.h"
//#include "filter_colormatix_frag.h"
//#include "mainend.h"
//const char *Filter_Convolution_frag =
//#include "defineVariable.h"
//#include "mainbegin.h"
//#include "filter_convolution_frag.h"
//#include "mainend.h"
//const char *Filter_Glow_frag =
//#include "defineVariable.h"
//#include "mainbegin.h"
//#include "filter_glow_frag.h"
//#include "mainend.h"
//const char *Filter_Shadow_frag =
//#include "defineVariable.h"
//#include "mainbegin.h"
//#include "filter_shadow_frag.h"
//#include "mainend.h"



//const char *Test_vert = 
//#include "Test_vert.h"
//const char *Test_frag = 
//#include "Test_frag.h"



