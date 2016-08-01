/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright ?Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc. 
****************************************************************************/

#ifndef GEOM_INCLUDED
#define GEOM_INCLUDED

#ifndef FIXED_INCLUDED
#include "flashfixed.h"
#endif

#ifdef __ANDROID__
#include "GGType.h"
#include "XData.h"
#endif

//
// TYPES
//

#define XBITMAPOFF 16

#define BITTOPIXEL(VV) ((VV+fixed_1)>>XBITMAPOFF)

#define oneInch 1440L
#define onePoint 20L
#define oneMillimeter 57L
#define oneCentimeter 570L

typedef S32 SCOORD, *P_SCOORD;

#define SPOINT		_XPOINT
typedef _XPOINT*	P_SPOINT;
#define rectEmptyFlag 0x80000000L

//#define xmin  left
//#define xmax  right
//#define ymin  top
//#define ymax  bottom
typedef struct _XSRECT {	// Note that a rectangle is considered empty if xmin == rectEmptyFlag
	SCOORD xmin;
	SCOORD xmax;
	SCOORD ymin;
	SCOORD ymax;
} SRECT, *P_SRECT;

enum { // reference points on a rectangle
		rectTopLeft = 0, rectTopRight, rectBottomRight, rectBottomLeft, 
		rectTopCenter, rectCenterRight, rectBottomCenter, rectCenterLeft,
		rectCenter
	};

typedef struct _XMATRIX {
	SFIXED a;
	SFIXED b;
	SFIXED c;
	SFIXED d;
	SCOORD tx;
	SCOORD ty;
} MATRIX, *P_MATRIX;

#define ANTIZOOM 4
#define ANTISTEP 16
#define ANTIADD	 8
////////////////////////////////////////////////////////////
// ���ζ�����
class XSWFRect:public XRect
{
public:
	//�����ж�ȡ���
	void  Center(XPOINT&pt)
	{
		pt.x=(left+right+1)>>1;
		pt.y=(top+bottom+1)>>1;
	}
	int CenterX(){return (left+right+1)>>1;};
	int CenterY(){return (top+bottom+1)>>1;};
	//XBOOL ReadFromStream(XStream* stream);
	//SCOORD Width()		{ return right - left + 1; }
	//SCOORD Height()		{ return bottom - top + 1; }
	void operator=(XRect&r)
	{
		left=r.left;
		right=r.right;
		top=r.top;
		bottom=r.bottom;
	}
	void ToZoom()
	{
		left<<=ANTIZOOM;
		right<<=ANTIZOOM;
		top<<=ANTIZOOM;
		bottom<<=ANTIZOOM;
	}
/*	void ToZoomPoly()
	{
		left<<=ANTIZOOMPOLY;
		right<<=ANTIZOOMPOLY;
		top<<=ANTIZOOMPOLY;
		bottom<<=ANTIZOOMPOLY;
	}
	void ToPolyRound()
	{
		left>>=ANTIZOOMPOLY;
		right>>=ANTIZOOMPOLY;
		top>>=ANTIZOOMPOLY;
		bottom>>=ANTIZOOMPOLY;
	}*/
	void ToPixel()
	{
		left=left>>ANTIZOOM;
		top=top>>ANTIZOOM;
		right=(right+ANTIADD)>>ANTIZOOM;
		bottom=(bottom+ANTIADD)>>ANTIZOOM;
	}
	void AddRect(XRect&rect,XU8 bFirst)
	{
		if(bFirst)
			*this=rect;
		else
			*this|=rect;
	}
	void AddPoint(int x,int y,XU8 bFirst)
	{
		if(bFirst)//IsRectEmpty())
		{
			left=x;
			right=x;
			top=y;
			bottom=y;
		}
		else
		{
			if(left>x) left=x;
			if(top>y) top=y;
			if(right<x) right=x;
			if(bottom<y) bottom=y;
		}
	}
	void GetBoundRect(XRect&rect,MATRIX*pMat);
};
//
// SPOINT FUNCTIONS
//

void PointScale(P_SPOINT pt, SFIXED factor, P_SPOINT dst);
void PointShift(P_SPOINT src, int shift, P_SPOINT dst);
void PointAverage(P_SPOINT pt1, P_SPOINT pt2, SFIXED f, P_SPOINT dst);

//
// Inline Functions
//

inline void PointAdd(P_SPOINT pt1, P_SPOINT pt2, P_SPOINT dst)
{
	dst->x = pt1->x + pt2->x;
	dst->y = pt1->y + pt2->y;
}

inline void PointSub(P_SPOINT pt1, P_SPOINT pt2, P_SPOINT dst)
{
	dst->x = pt1->x - pt2->x;
	dst->y = pt1->y - pt2->y;
}

inline void PointAverage(P_SPOINT pt1, P_SPOINT pt2, P_SPOINT dst)
{
	dst->x = (pt1->x + pt2->x)>>1;
	dst->y = (pt1->y + pt2->y)>>1;
}

BOOL PointEqual(P_SPOINT pt1, P_SPOINT pt2, SCOORD err);

inline BOOL PointEqual(P_SPOINT pt1, P_SPOINT pt2) 
{ 
	return pt1->x == pt2->x && pt1->y == pt2->y; 
}

SFIXED PointAngle(P_SPOINT pt, P_SPOINT origin);
// Return the angle of the vector from origin to pt
// Origin can be nil as a shorthand for (0,0)

void PointPolar(SCOORD length, SFIXED angle, P_SPOINT origin, P_SPOINT dst);
// Generate a polar from the polar representaion

SCOORD PointNearestLine(P_SPOINT l1, P_SPOINT l2, P_SPOINT pt, P_SPOINT nearestPt);
// Find the point nearest to pt in line (l1, l2)
// If nearestPt is not null, return the
// Returns the distance from pt to nearestPt

BOOL LineIntersection(P_SPOINT a1, P_SPOINT a2, P_SPOINT b1, P_SPOINT b2, SFIXED* uA, SFIXED* uB);
// Calculate the intersection of line (a1,a2) and line (b1,b2)
// If both uA and uB are nil, simply tests to see if the lines intersect

SCOORD PointFastLength(P_SPOINT pt);
SCOORD PointFastDistance(P_SPOINT pt1, P_SPOINT pt2);
// Calculate a very fast approximation of the distance between pt1 amd pt2

SCOORD PointLength(P_SPOINT pt);
SCOORD PointDistance(P_SPOINT pt1, P_SPOINT pt2);
// Calculate the distance between pt1 and pt2

void PointNormalize(SPOINT* v, SCOORD thickness);


//
// SRECT FUNCTIONS
//

void RectSet(SCOORD xmin, SCOORD ymin, SCOORD xmax, SCOORD ymax, P_SRECT dst);
void RectSetPoint(P_SPOINT pt, P_SRECT dst);
void RectSetPoint2(P_SPOINT, P_SPOINT, P_SRECT dst);
void RectSetEmpty(P_SRECT dst);
void RectSetHuge(P_SRECT dst);
void RectCenter(P_SRECT r, P_SPOINT pt);
void RectValidate(P_SRECT dst);
void RectOffset(SCOORD dx, SCOORD dy, P_SRECT dst);
void RectInset(SCOORD dist, P_SRECT dst);
void RectShift(P_SRECT src, int shift, P_SRECT dst);

#define RectIsEmpty(r) ((r)->xmin == rectEmptyFlag)
void RectGetPoint(P_SRECT r, int ref, P_SPOINT pt);
int RectOppositePoint(int);
void RectUnion(P_SRECT r1, P_SRECT r2, P_SRECT dst);
void RectUnionPoint(P_SPOINT pt, P_SRECT dst);
void RectIntersect(P_SRECT r1, P_SRECT r2, P_SRECT dst);
BOOL RectTestIntersect(P_SRECT r1, P_SRECT r2);
BOOL RectEqual(P_SRECT r1, P_SRECT r2, SCOORD err);
BOOL RectPointIn(P_SRECT r, P_SPOINT pt);
//BOOL RectContains(P_SRECT big, P_SRECT small);
SCOORD RectSize(P_SRECT);
void RectSizePoint(P_SRECT, P_SPOINT);
SCOORD RectNearDistance(P_SRECT, P_SPOINT);
SCOORD RectDistance(P_SRECT, P_SRECT);
#define RectWidth(r) ((r)->xmax - (r)->xmin)
#define RectHeight(r) ((r)->ymax - (r)->ymin)


//
// MATRIX FUNCTIONS
//

void MatrixTransformPoint(P_MATRIX m, P_SPOINT p, P_SPOINT dst);
void MatrixDeltaTransformPoint(P_MATRIX m, P_SPOINT p, P_SPOINT dst);
void MatrixTransformRect(P_MATRIX m, P_SRECT r, P_SRECT dst);
SCOORD MatrixTransformThickness(P_MATRIX m, SCOORD);

void MatrixConcat(P_MATRIX m1, P_MATRIX m2, P_MATRIX dst);
// Given m1 that maps from A to B and m2 that maps from B to C
// Generates dst that maps from A to C

void MatrixInvert(P_MATRIX m, P_MATRIX dst);
// Given m that maps from A to B
// Generates dst that maps from B to A

BOOL MatrixEqual(P_MATRIX m1, P_MATRIX m2, SFIXED err);

void MatrixIdentity(P_MATRIX m);
void MatrixRotate(SFIXED degrees, P_MATRIX m);
void MatrixRotate(XFDOUBLE r, P_MATRIX m);
void MatrixTranslate(SCOORD dx, SCOORD dy, P_MATRIX m);
void MatrixScale(SCOORD sx, SCOORD sy, P_MATRIX m);

// #ifdef SPRITE_XTRA
// void MatrixFlipX(P_MATRIX m);
// void MatrixFlipY(P_MATRIX m);
// void MatrixSkew(SFIXED degrees, P_MATRIX m);
// #endif // SPRITE_XTRA

void MatrixMap(P_SRECT src, P_SRECT dst, P_MATRIX m);
// Generates m that will map any point in src to the coresponding point in dst

void MatrixMapAspect(P_SRECT src, P_SRECT dst, P_MATRIX m);	
// Map rects while maintaining a constant aspect ratio


void MatrixDecompose(MATRIX* mat, SPOINT* scale, SFIXED* rot);
BOOL MatrixIsScaleOnly(P_MATRIX m);
BOOL MatrixIsIdentity(P_MATRIX m);

#define MIND 12

BOOL MatrixIsNoTrans(P_MATRIX m);

const XPCTSTR _strEmpty=("");
const XPCTSTR _strInstance=("instance");

enum {	XSYSID__Infinity,
	XSYSID___proto__,
	XSYSID___resolve,
	XSYSID__accProps,
	XSYSID__alpha,
	XSYSID__currentframe,
	XSYSID__droptarget,
	XSYSID__focusrect,
	XSYSID__framesloaded,
	XSYSID__global,
	XSYSID__height,
	XSYSID__highquality,
	XSYSID__level,
	XSYSID__listeners,
	XSYSID__lockroot,
	XSYSID__name,
	XSYSID__parent,
	XSYSID__quality,
	XSYSID__root,
	XSYSID__rotation,
	XSYSID__soundbuftime,
	XSYSID__target,
	XSYSID__totalframes,
	XSYSID__url,
	XSYSID__visible,
	XSYSID__width,
	XSYSID__x,
	XSYSID__xmouse,
	XSYSID__xscale,
	XSYSID__y,
	XSYSID__ymouse,
	XSYSID__yscale,
	XSYSID_a,
	XSYSID_abs,
	XSYSID_accept,
	XSYSID_Accessibility,
	XSYSID_acos,
	XSYSID_activityLevel,
	XSYSID_add,
	XSYSID_addCallback,
	XSYSID_addDelayedInstance,
	XSYSID_addListener,
	XSYSID_addPage,
	XSYSID_addProperty,
	XSYSID_addRequestHeader,
	XSYSID_addXMLPath,
	XSYSID_align,
	XSYSID_allowDomain,
	XSYSID_allowInsecureDomain,
	XSYSID_alpha,
	XSYSID_alphaMultiplier,
	XSYSID_ALPHANUMERIC_FULL,
	XSYSID_ALPHANUMERIC_HALF,
	XSYSID_alphaOffset,
	XSYSID_alphas,
	XSYSID_angle,
	XSYSID_antiAliasType,
	XSYSID_apply,
	XSYSID_applyFilter,
	XSYSID_arguments,
	XSYSID_Array,
	XSYSID_AsBroadcaster,
	XSYSID_asfunction,
	XSYSID_asin,
	XSYSID_atan,
	XSYSID_atan2,
	XSYSID_attachAudio,
	XSYSID_attachBitmap,
	XSYSID_attachMovie,
	XSYSID_attachSound,
	XSYSID_attachVideo,
	XSYSID_attributes,
	XSYSID_autoReplace,
	XSYSID_autoSize,
	XSYSID_available,
	XSYSID_avHardwareDisable,
	XSYSID_b,
	XSYSID_background,
	XSYSID_backgroundColor,
	XSYSID_BACKSPACE,
	XSYSID_bandwidth,
	XSYSID_beginBitmapFill,
	XSYSID_beginFill,
	XSYSID_beginGradientFill,
	XSYSID_BevelFilter,
	XSYSID_bias,
	XSYSID_BitmapData,
	XSYSID_BitmapFilter,
	XSYSID_blendMode,
	XSYSID_blockIndent,
	XSYSID_blueMultiplier,
	XSYSID_blueOffset,
	XSYSID_BlurFilter,
	XSYSID_blurX,
	XSYSID_blurY,
	XSYSID_bold,
	XSYSID_Boolean,
	XSYSID_border,
	XSYSID_borderColor,
	XSYSID_bottom,
	XSYSID_bottomRight,
	XSYSID_bottomScroll,
	XSYSID_broadcastMessage,
	XSYSID_browse,
	XSYSID_bufferLength,
	XSYSID_bufferTime,
	XSYSID_builtInItems,
	XSYSID_bullet,
	XSYSID_Button,
	XSYSID_bytesLoaded,
	XSYSID_bytesTotal,
	XSYSID_c,
	XSYSID_cacheAsBitmap,
	XSYSID_call,
	XSYSID_callee,
	XSYSID_caller,
	XSYSID_Camera,
	XSYSID_cancel,
	XSYSID_capabilities,
	XSYSID_CAPSLOCK,
	XSYSID_caption,
	XSYSID_CASEINSENSITIVE,
	XSYSID_ceil,
	XSYSID_charAt,
	XSYSID_charCodeAt,
	XSYSID_checkXMLStatus,
	XSYSID_childNodes,
	XSYSID_CHINESE,
	XSYSID_chr,
	XSYSID_clamp,
	XSYSID_clear,
	XSYSID_clearInterval,
	XSYSID_clone,
	XSYSID_close,
	XSYSID_Color,
	XSYSID_ColorMatrixFilter,
	XSYSID_colors,
	XSYSID_ColorTransform,
	XSYSID_colorTransform,
	XSYSID_componentX,
	XSYSID_componentY,
	XSYSID_concat,
	XSYSID_concatenatedColorTransform,
	XSYSID_concatenatedMatrix,
	XSYSID_condenseWhite,
	XSYSID_connect,
	XSYSID_constructor,
	XSYSID_contains,
	XSYSID_containsPoint,
	XSYSID_containsRectangle,
	XSYSID_contentType,
	XSYSID_ContextMenu,
	XSYSID_ContextMenuItem,
	XSYSID_CONTROL,
	XSYSID_ConvolutionFilter,
	XSYSID_copy,
	XSYSID_copyChannel,
	XSYSID_copyPixels,
	XSYSID_cos,
	XSYSID_createBox,
	XSYSID_createEmptyMovieClip,
	XSYSID_createGradientBox,
	XSYSID_createTextField,
	XSYSID_creationDate,
	XSYSID_creator,
	XSYSID_currentFps,
	XSYSID_curveTo,
	XSYSID_CustomActions,
	XSYSID_customItems,
	XSYSID_d,
	XSYSID_data,
	XSYSID_Date,
	XSYSID_deblocking,
	XSYSID_decode,
	XSYSID_DELETEKEY,
	XSYSID_deltaTransformPoint,
	XSYSID_DESCENDING,
	XSYSID_DisplacementMapFilter,
	XSYSID_display,
	XSYSID_dispose,
	XSYSID_distance,
	XSYSID_divisor,
	XSYSID_docTypeDecl,
	XSYSID_domain,
	XSYSID_DOWN,
	XSYSID_download,
	XSYSID_draw,
	XSYSID_DropShadowFilter,
	XSYSID_duplicateMovieClip,
	XSYSID_duration,
	XSYSID_E,
	XSYSID_embedFonts,
	XSYSID_enabled,
	XSYSID_END,
	XSYSID_endFill,
	XSYSID_ENTER,
	XSYSID_equals,
	XSYSID_Error,
	XSYSID_escape,
	XSYSID_ESCAPE,
	XSYSID_SHIFT,
	XSYSID_eval,
	XSYSID_exactSettings,
	XSYSID_exp,
	XSYSID_ExternalInterface,
	XSYSID_false,
	XSYSID_fileList,
	XSYSID_FileReference,
	XSYSID_FileReferenceList,
	XSYSID_fillRect,
	XSYSID_filters,
	XSYSID_findText,
	XSYSID_firstChild,
	XSYSID_floodFill,
	XSYSID_floor,
	XSYSID_flush,
	XSYSID_focusEnabled,
	XSYSID_font,
	XSYSID_fontFamily,
	XSYSID_fontSize,
	XSYSID_fontStyle,
	XSYSID_fontWeight,
	XSYSID_fps,
	XSYSID_fromCharCode,
	XSYSID_fscommand,
	XSYSID_Function,
	XSYSID_gain,
	XSYSID_generateFilterRect,
	XSYSID_geom,
	XSYSID_get,
	XSYSID_getAscii,
	XSYSID_getBeginIndex,
	XSYSID_getBounds,
	XSYSID_getBytesLoaded,
	XSYSID_getBytesTotal,
	XSYSID_getCaretIndex,
	XSYSID_getCode,
	XSYSID_getColorBoundsRect,
	XSYSID_getCount,
	XSYSID_getDate,
	XSYSID_getDay,
	XSYSID_getDefaultLang,
	XSYSID_getDepth,
	XSYSID_getEndIndex,
	XSYSID_getFocus,
	XSYSID_getFontList,
	XSYSID_getFullYear,
	XSYSID_getHours,
	XSYSID_getInstanceAtDepth,
	XSYSID_getLocal,
	XSYSID_getMilliseconds,
	XSYSID_getMinutes,
	XSYSID_getMonth,
	XSYSID_getNewTextFormat,
	XSYSID_getNextHighestDepth,
	XSYSID_getPan,
	XSYSID_getPixel,
	XSYSID_getPixel32,
	XSYSID_getProgress,
	XSYSID_getProperty,
	XSYSID_getRect,
	XSYSID_getRGB,
	XSYSID_getSeconds,
	XSYSID_getSelected,
	XSYSID_getSelectedText,
	XSYSID_getSize,
	XSYSID_getStyle,
	XSYSID_getStyleNames,
	XSYSID_getSWFVersion,
	XSYSID_getText,
	XSYSID_getTextFormat,
	XSYSID_getTextRunInfo,
	XSYSID_getTextSnapshot,
	XSYSID_getTime,
	XSYSID_getTimer,
	XSYSID_getTimezoneOffset,
	XSYSID_getTransform,
	XSYSID_getURL,
	XSYSID_getUTCDate,
	XSYSID_getUTCDay,
	XSYSID_getUTCFullYear,
	XSYSID_getUTCHours,
	XSYSID_getUTCMilliseconds,
	XSYSID_getUTCMinutes,
	XSYSID_getUTCMonth,
	XSYSID_getUTCSeconds,
	XSYSID_getUTCYear,
	XSYSID_getVersion,
	XSYSID_getVolume,
	XSYSID_getYear,
	XSYSID_globalToLocal,
	XSYSID_GlowFilter,
	XSYSID_gotoAndPlay,
	XSYSID_gotoAndStop,
	XSYSID_GradientBevelFilter,
	XSYSID_GradientGlowFilter,
	XSYSID_greenMultiplier,
	XSYSID_greenOffset,
	XSYSID_gridFitType,
	XSYSID_hasAccessibility,
	XSYSID_hasAudio,
	XSYSID_hasAudioEncoder,
	XSYSID_hasEmbeddedVideo,
	XSYSID_hasIME,
	XSYSID_hasMP3,
	XSYSID_hasOwnProperty,
	XSYSID_hasPrinting,
	XSYSID_hasScreenBroadcast,
	XSYSID_hasScreenPlayback,
	XSYSID_hasStreamingAudio,
	XSYSID_hasStreamingVideo,
	XSYSID_hasVideoEncoder,
	XSYSID_height,
	XSYSID_hide,
	XSYSID_hideBuiltInItems,
	XSYSID_hideObject,
	XSYSID_highlightAlpha,
	XSYSID_highlightColor,
	XSYSID_hitArea,
	XSYSID_hitTest,
	XSYSID_hitTestTextNearPos,
	XSYSID_HOME,
	XSYSID_hscroll,
	XSYSID_html,
	XSYSID_htmlText,
	XSYSID_id3,
	XSYSID_identity,
	XSYSID_idMap,
	XSYSID_ifFrameLoaded,
	XSYSID_ignoreWhite,
	XSYSID_IME,
	XSYSID_indent,
	XSYSID_index,
	XSYSID_indexOf,
	XSYSID_Infinity,
	XSYSID_inflate,
	XSYSID_inflatePoint,
	XSYSID_initialize,
	XSYSID_inner,
	XSYSID_INSERT,
	XSYSID_install,
	XSYSID_int,
	XSYSID_interpolate,
	XSYSID_intersection,
	XSYSID_intersects,
	XSYSID_invert,
	XSYSID_isAccessible,
	XSYSID_isActive,
	XSYSID_isDebugger,
	XSYSID_isDown,
	XSYSID_isEmpty,
	XSYSID_isFinite,
	XSYSID_isNaN,
	XSYSID_isPropertyEnumerable,
	XSYSID_isPrototypeOf,
	XSYSID_isToggled,
	XSYSID_italic,
	XSYSID_JAPANESE_HIRAGANA,
	XSYSID_JAPANESE_KATAKANA_FULL,
	XSYSID_JAPANESE_KATAKANA_HALF,
	XSYSID_join,
	XSYSID_kerning,
	XSYSID_Key,
	XSYSID_knockout,
	XSYSID_KOREAN,
	XSYSID_language,
	XSYSID_languageCodeArray,
	XSYSID_lastChild,
	XSYSID_lastIndexOf,
	XSYSID_leading,
	XSYSID_LEFT,
	XSYSID_left,
	XSYSID_leftMargin,
	XSYSID_length,
	XSYSID_letterSpacing,
	XSYSID_lineGradientStyle,
	XSYSID_lineStyle,
	XSYSID_lineTo,
	XSYSID_list,
	XSYSID_listeners,
	XSYSID_LN10,
	XSYSID_LN2,
	XSYSID_load,
	XSYSID_loadBitmap,
	XSYSID_loadClip,
	XSYSID_loaded,
	XSYSID_loadLanguageXML,
	XSYSID_loadMovie,
	XSYSID_loadMovieNum,
	XSYSID_loadPolicyFile,
	XSYSID_loadSound,
	XSYSID_loadString,
	XSYSID_loadStringEx,
	XSYSID_loadVariables,
	XSYSID_loadVariablesNum,
	XSYSID_LoadVars,
	XSYSID_LocalConnection,
	XSYSID_Locale,
	XSYSID_localFileReadDisable,
	XSYSID_localName,
	XSYSID_localToGlobal,
	XSYSID_log,
	XSYSID_LOG10E,
	XSYSID_LOG2E,
	XSYSID_manufacturer,
	XSYSID_mapBitmap,
	XSYSID_mapPoint,
	XSYSID_marginLeft,
	XSYSID_marginRight,
	XSYSID_Math,
	XSYSID_Matrix,
	XSYSID_matrix,
	XSYSID_matrixX,
	XSYSID_matrixY,
	XSYSID_max,
	XSYSID_MAX_VALUE,
	XSYSID_maxChars,
	XSYSID_maxhscroll,
	XSYSID_maxLevel,
	XSYSID_maxscroll,
	XSYSID_mbchr,
	XSYSID_mblength,
	XSYSID_mbord,
	XSYSID_mbsubstring,
	XSYSID_menu,
	XSYSID_merge,
	XSYSID_message,
	XSYSID_methodName,
	XSYSID_Microphone,
	XSYSID_min,
	XSYSID_MIN_VALUE,
	XSYSID_MMExecute,
	XSYSID_mode,
	XSYSID_modificationDate,
	XSYSID_motionLevel,
	XSYSID_motionTimeOut,
	XSYSID_Mouse,
	XSYSID_mouseWheelEnabled,
	XSYSID_moveTo,
	XSYSID_MovieClip,
	XSYSID_MovieClipLoader,
	XSYSID_multiline,
	XSYSID_muted,
	XSYSID_name,
	XSYSID_names,
	XSYSID_namespaceURI,
	XSYSID_NaN,
	XSYSID_NEGATIVE_INFINITY,
	XSYSID_NetConnection,
	XSYSID_NetStream,
	XSYSID_newline,
	XSYSID_nextFrame,
	XSYSID_nextScene,
	XSYSID_nextSibling,
	XSYSID_nodeName,
	XSYSID_nodeType,
	XSYSID_nodeValue,
	XSYSID_noise,
	XSYSID_normalize,
	XSYSID_null,
	XSYSID_Number,
	XSYSID_NUMERIC,
	XSYSID_Object,
	XSYSID_offset,
	XSYSID_offsetPoint,
	XSYSID_on,
	XSYSID_onActivity,
	XSYSID_onCancel,
	XSYSID_onChanged,
	XSYSID_onClipEvent,
	XSYSID_onClose,
	XSYSID_onComplete,
	XSYSID_onConnect,
	XSYSID_onData,
	XSYSID_onDragOut,
	XSYSID_onDragOver,
	XSYSID_onEnterFrame,
	XSYSID_onHTTPError,
	XSYSID_onHTTPStatus,
	XSYSID_onID3,
	XSYSID_onIOError,
	XSYSID_onKeyDown,
	XSYSID_onKeyUp,
	XSYSID_onKillFocus,
	XSYSID_onLoad,
	XSYSID_onLoadComplete,
	XSYSID_onLoadError,
	XSYSID_onLoadInit,
	XSYSID_onLoadProgress,
	XSYSID_onLoadStart,
	XSYSID_onMetaData,
	XSYSID_onMouseDown,
	XSYSID_onMouseMove,
	XSYSID_onMouseUp,
	XSYSID_onMouseWheel,
	XSYSID_onOpen,
	XSYSID_onPress,
	XSYSID_onProgress,
	XSYSID_onRelease,
	XSYSID_onReleaseOutside,
	XSYSID_onResize,
	XSYSID_onRollOut,
	XSYSID_onRollOver,
	XSYSID_onScroller,
	XSYSID_onSecurityError,
	XSYSID_onSelect,
	XSYSID_onSetFocus,
	XSYSID_onSoundComplete,
	XSYSID_onStatus,
	XSYSID_onUnload,
	XSYSID_onXML,
	XSYSID_opaqueBackground,
	XSYSID_ord,
	XSYSID_orientation,
	XSYSID_os,
	XSYSID_pageHeight,
	XSYSID_pageWidth,
	XSYSID_paletteMap,
	XSYSID_paperHeight,
	XSYSID_paperWidth,
	XSYSID_parentNode,
	XSYSID_parseCSS,
	XSYSID_parseFloat,
	XSYSID_parseInt,
	XSYSID_password,
	XSYSID_pause,
	XSYSID_perlinNoise,
	XSYSID_PGDN,
	XSYSID_PGUP,
	XSYSID_PI,
	XSYSID_pixelAspectRatio,
	XSYSID_pixelBounds,
	XSYSID_pixelDissolve,
	XSYSID_play,
	XSYSID_playerType,
	XSYSID_Point,
	XSYSID_polar,
	XSYSID_pop,
	XSYSID_position,
	XSYSID_POSITIVE_INFINITY,
	XSYSID_pow,
	XSYSID_prefix,
	XSYSID_preserveAlpha,
	XSYSID_prevFrame,
	XSYSID_previousSibling,
	XSYSID_prevScene,
	XSYSID_print,
	XSYSID_printAsBitmap,
	XSYSID_printAsBitmapNum,
	XSYSID_PrintJob,
	XSYSID_printNum,
	XSYSID_prototype,
	XSYSID_push,
	XSYSID_quality,
	XSYSID_random,
	XSYSID_rate,
	XSYSID_ratios,
	XSYSID_rectangle,
	XSYSID_Rectangle,
	XSYSID_redMultiplier,
	XSYSID_redOffset,
	XSYSID_registerClass,
	XSYSID_removeListener,
	XSYSID_removeMovieClip,
	XSYSID_removeTextField,
	XSYSID_replaceSel,
	XSYSID_replaceText,
	XSYSID_restrict,
	XSYSID_RETURNINDEXEDARRAY,
	XSYSID_reverse,
	XSYSID_rgb,
	XSYSID_RIGHT,
	XSYSID_right,
	XSYSID_rightMargin,
	XSYSID_rotate,
	XSYSID_round,
	XSYSID_sandboxType,
	XSYSID_scale,
	XSYSID_scale9Grid,
	XSYSID_scaleMode,
	XSYSID_scaleX,
	XSYSID_scaleY,
	XSYSID_screenColor,
	XSYSID_screenDPI,
	XSYSID_screenResolutionX,
	XSYSID_screenResolutionY,
	XSYSID_scroll,
	XSYSID_scrollRect,
	XSYSID_security,
	XSYSID_seek,
	XSYSID_selectable,
	XSYSID_Selection,
	XSYSID_send,
	XSYSID_sendAndLoad,
	XSYSID_separatorBefore,
	XSYSID_serverString,
	XSYSID_set,
	XSYSID_setAdvancedAntialiasingTable,
	XSYSID_setBufferTime,
	XSYSID_setClipboard,
	XSYSID_setDate,
	XSYSID_setDefaultLang,
	XSYSID_setEmpty,
	XSYSID_setFocus,
	XSYSID_setFullYear,
	XSYSID_setGain,
	XSYSID_setHours,
	XSYSID_setInterval,
	XSYSID_setLoadCallback,
	XSYSID_setMask,
	XSYSID_setMilliseconds,
	XSYSID_setMinutes,
	XSYSID_setMode,
	XSYSID_setMonth,
	XSYSID_setMotionLevel,
	XSYSID_setNewTextFormat,
	XSYSID_setPan,
	XSYSID_setPixel,
	XSYSID_setPixel32,
	XSYSID_setProperty,
	XSYSID_setQuality,
	XSYSID_setRate,
	XSYSID_setRGB,
	XSYSID_setSeconds,
	XSYSID_setSelectColor,
	XSYSID_setSelected,
	XSYSID_setSelection,
	XSYSID_setSilenceLevel,
	XSYSID_setString,
	XSYSID_setStyle,
	XSYSID_setTextFormat,
	XSYSID_setTime,
	XSYSID_setTimeout,
	XSYSID_setTransform,
	XSYSID_setUseEchoSuppression,
	XSYSID_setUTCDate,
	XSYSID_setUTCFullYear,
	XSYSID_setUTCHours,
	XSYSID_setUTCMilliseconds,
	XSYSID_setUTCMinutes,
	XSYSID_setUTCMonth,
	XSYSID_setUTCSeconds,
	XSYSID_setVolume,
	XSYSID_setYear,
	XSYSID_shadowAlpha,
	XSYSID_shadowColor,
	XSYSID_SharedObject,
	XSYSID_sharpness,
	XSYSID_shift,
	XSYSID_show,
	XSYSID_showMenu,
	XSYSID_showRedrawRegions,
	XSYSID_showSettings,
	XSYSID_silenceLevel,
	XSYSID_silenceTimeOut,
	XSYSID_sin,
	XSYSID_size,
	XSYSID_slice,
	XSYSID_smoothing,
	XSYSID_sort,
	XSYSID_sortOn,
	XSYSID_Sound,
	XSYSID_SPACE,
	XSYSID_splice,
	XSYSID_split,
	XSYSID_sqrt,
	XSYSID_SQRT1_2,
	XSYSID_SQRT2,
	XSYSID_Stage,
	XSYSID_start,
	XSYSID_startDrag,
	XSYSID_status,
	XSYSID_stop,
	XSYSID_stopAllSounds,
	XSYSID_stopDrag,
	XSYSID_strength,
	XSYSID_String,
	XSYSID_stringIDArray,
	XSYSID_StyleSheet,
	XSYSID_styleSheet,
	XSYSID_substr,
	XSYSID_substring,
	XSYSID_subtract,
	XSYSID_swapDepths,
	XSYSID_System,
	XSYSID_TAB,
	XSYSID_tabChildren,
	XSYSID_tabEnabled,
	XSYSID_tabIndex,
	XSYSID_tabStops,
	XSYSID_tan,
	XSYSID_target,
	XSYSID_targetPath,
	XSYSID_tellTarget,
	XSYSID_text,
	XSYSID_textAlign,
	XSYSID_textColor,
	XSYSID_textDecoration,
	XSYSID_TextField,
	XSYSID_TextFormat,
	XSYSID_textHeight,
	XSYSID_textIndent,
	XSYSID_TextRenderer,
	XSYSID_TextSnapshot,
	XSYSID_textWidth,
	XSYSID_thickness,
	XSYSID_this,
	XSYSID_threshold,
	XSYSID_time,
	XSYSID_toggleHighQuality,
	XSYSID_toLowerCase,
	XSYSID_top,
	XSYSID_topLeft,
	XSYSID_toString,
	XSYSID_toUpperCase,
	XSYSID_trace,
	XSYSID_trackAsMenu,
	XSYSID_Transform,
	XSYSID_transform,
	XSYSID_transformPoint,
	XSYSID_translate,
	XSYSID_transparent,
	XSYSID_true,
	XSYSID_tx,
	XSYSID_ty,
	XSYSID_type,
	XSYSID_undefined,
	XSYSID_underline,
	XSYSID_unescape,
	XSYSID_uninstall,
	XSYSID_union,
	XSYSID_UNIQUESORT,
	XSYSID_UNKNOWN,
	XSYSID_unloadClip,
	XSYSID_unloadMovie,
	XSYSID_unloadMovieNum,
	XSYSID_unshift,
	XSYSID_unwatch,
	XSYSID_UP,
	XSYSID_updateAfterEvent,
	XSYSID_updateProperties,
	XSYSID_upload,
	XSYSID_url,
	XSYSID_useCodepage,
	XSYSID_useEchoSuppression,
	XSYSID_useHandCursor,
	XSYSID_UTC,
	XSYSID_valueOf,
	XSYSID_variable,
	XSYSID_version,
	XSYSID_Video,
	XSYSID_visible,
	XSYSID_watch,
	XSYSID_width,
	XSYSID_wordWrap,
	XSYSID_x,
	XSYSID_XML,
	XSYSID_xmlDecl,
	XSYSID_XMLNode,
	XSYSID_XMLSocket,
	XSYSID_XMLUI,
	XSYSID_y,
	XSYSID__version,
};
const XPCTSTR _strSys_Infinity="-Infinity";
const XPCTSTR _strSys__proto__="__proto__";
const XPCTSTR _strSys__resolve="__resolve";
const XPCTSTR _strSys_accProps="_accProps";
const XPCTSTR _strSys_alpha="_alpha";
const XPCTSTR _strSys_currentframe="_currentframe";
const XPCTSTR _strSys_droptarget="_droptarget";
const XPCTSTR _strSys_focusrect="_focusrect";
const XPCTSTR _strSys_framesloaded="_framesloaded";
const XPCTSTR _strSys_global="_global";
const XPCTSTR _strSys_height="_height";
const XPCTSTR _strSys_highquality="_highquality";
const XPCTSTR _strSys_level="_level";
const XPCTSTR _strSys_listeners="_listeners";
const XPCTSTR _strSys_lockroot="_lockroot";
const XPCTSTR _strSys_name="_name";
const XPCTSTR _strSys_parent="_parent";
const XPCTSTR _strSys_quality="_quality";
const XPCTSTR _strSys_root="_root";
const XPCTSTR _strSys_rotation="_rotation";
const XPCTSTR _strSys_soundbuftime="_soundbuftime";
const XPCTSTR _strSys_target="_target";
const XPCTSTR _strSys_totalframes="_totalframes";
const XPCTSTR _strSys_url="_url";
const XPCTSTR _strSys_visible="_visible";
const XPCTSTR _strSys_width="_width";
const XPCTSTR _strSys_x="_x";
const XPCTSTR _strSys_xmouse="_xmouse";
const XPCTSTR _strSys_xscale="_xscale";
const XPCTSTR _strSys_y="_y";
const XPCTSTR _strSys_ymouse="_ymouse";
const XPCTSTR _strSys_yscale="_yscale";
const XPCTSTR _strSysa="a";
const XPCTSTR _strSysabs="abs";
const XPCTSTR _strSysaccept="accept";
const XPCTSTR _strSysAccessibility="Accessibility";
const XPCTSTR _strSysacos="acos";
const XPCTSTR _strSysactivityLevel="activityLevel";
const XPCTSTR _strSysadd="add";
const XPCTSTR _strSysaddCallback="addCallback";
const XPCTSTR _strSysaddDelayedInstance="addDelayedInstance";
const XPCTSTR _strSysaddListener="addListener";
const XPCTSTR _strSysaddPage="addPage";
const XPCTSTR _strSysaddProperty="addProperty";
const XPCTSTR _strSysaddRequestHeader="addRequestHeader";
const XPCTSTR _strSysaddXMLPath="addXMLPath";
const XPCTSTR _strSysalign="align";
const XPCTSTR _strSysallowDomain="allowDomain";
const XPCTSTR _strSysallowInsecureDomain="allowInsecureDomain";
const XPCTSTR _strSysalpha="alpha";
const XPCTSTR _strSysalphaMultiplier="alphaMultiplier";
const XPCTSTR _strSysALPHANUMERIC_FULL="ALPHANUMERIC_FULL";
const XPCTSTR _strSysALPHANUMERIC_HALF="ALPHANUMERIC_HALF";
const XPCTSTR _strSysalphaOffset="alphaOffset";
const XPCTSTR _strSysalphas="alphas";
const XPCTSTR _strSysangle="angle";
const XPCTSTR _strSysantiAliasType="antiAliasType";
const XPCTSTR _strSysapply="apply";
const XPCTSTR _strSysapplyFilter="applyFilter";
const XPCTSTR _strSysarguments="arguments";
const XPCTSTR _strSysArray="Array";
const XPCTSTR _strSysAsBroadcaster="AsBroadcaster";
const XPCTSTR _strSysasfunction="asfunction";
const XPCTSTR _strSysasin="asin";
const XPCTSTR _strSysatan="atan";
const XPCTSTR _strSysatan2="atan2";
const XPCTSTR _strSysattachAudio="attachAudio";
const XPCTSTR _strSysattachBitmap="attachBitmap";
const XPCTSTR _strSysattachMovie="attachMovie";
const XPCTSTR _strSysattachSound="attachSound";
const XPCTSTR _strSysattachVideo="attachVideo";
const XPCTSTR _strSysattributes="attributes";
const XPCTSTR _strSysautoReplace="autoReplace";
const XPCTSTR _strSysautoSize="autoSize";
const XPCTSTR _strSysavailable="available";
const XPCTSTR _strSysavHardwareDisable="avHardwareDisable";
const XPCTSTR _strSysb="b";
const XPCTSTR _strSysbackground="background";
const XPCTSTR _strSysbackgroundColor="backgroundColor";
const XPCTSTR _strSysBACKSPACE="BACKSPACE";
const XPCTSTR _strSysbandwidth="bandwidth";
const XPCTSTR _strSysbeginBitmapFill="beginBitmapFill";
const XPCTSTR _strSysbeginFill="beginFill";
const XPCTSTR _strSysbeginGradientFill="beginGradientFill";
const XPCTSTR _strSysBevelFilter="BevelFilter";
const XPCTSTR _strSysbias="bias";
const XPCTSTR _strSysBitmapData="BitmapData";
const XPCTSTR _strSysBitmapFilter="BitmapFilter";
const XPCTSTR _strSysblendMode="blendMode";
const XPCTSTR _strSysblockIndent="blockIndent";
const XPCTSTR _strSysblueMultiplier="blueMultiplier";
const XPCTSTR _strSysblueOffset="blueOffset";
const XPCTSTR _strSysBlurFilter="BlurFilter";
const XPCTSTR _strSysblurX="blurX";
const XPCTSTR _strSysblurY="blurY";
const XPCTSTR _strSysbold="bold";
const XPCTSTR _strSysBoolean="Boolean";
const XPCTSTR _strSysborder="border";
const XPCTSTR _strSysborderColor="borderColor";
const XPCTSTR _strSysbottom="bottom";
const XPCTSTR _strSysbottomRight="bottomRight";
const XPCTSTR _strSysbottomScroll="bottomScroll";
const XPCTSTR _strSysbroadcastMessage="broadcastMessage";
const XPCTSTR _strSysbrowse="browse";
const XPCTSTR _strSysbufferLength="bufferLength";
const XPCTSTR _strSysbufferTime="bufferTime";
const XPCTSTR _strSysbuiltInItems="builtInItems";
const XPCTSTR _strSysbullet="bullet";
const XPCTSTR _strSysButton="Button";
const XPCTSTR _strSysbytesLoaded="bytesLoaded";
const XPCTSTR _strSysbytesTotal="bytesTotal";
const XPCTSTR _strSysc="c";
const XPCTSTR _strSyscacheAsBitmap="cacheAsBitmap";
const XPCTSTR _strSyscall="call";
const XPCTSTR _strSyscallee="callee";
const XPCTSTR _strSyscaller="caller";
const XPCTSTR _strSysCamera="Camera";
const XPCTSTR _strSyscancel="cancel";
const XPCTSTR _strSyscapabilities="capabilities";
const XPCTSTR _strSysCAPSLOCK="CAPSLOCK";
const XPCTSTR _strSyscaption="caption";
const XPCTSTR _strSysCASEINSENSITIVE="CASEINSENSITIVE";
const XPCTSTR _strSysceil="ceil";
const XPCTSTR _strSyscharAt="charAt";
const XPCTSTR _strSyscharCodeAt="charCodeAt";
const XPCTSTR _strSyscheckXMLStatus="checkXMLStatus";
const XPCTSTR _strSyschildNodes="childNodes";
const XPCTSTR _strSysCHINESE="CHINESE";
const XPCTSTR _strSyschr="chr";
const XPCTSTR _strSysclamp="clamp";
const XPCTSTR _strSysclear="clear";
const XPCTSTR _strSysclearInterval="clearInterval";
const XPCTSTR _strSysclone="clone";
const XPCTSTR _strSysclose="close";
const XPCTSTR _strSysColor="Color";
const XPCTSTR _strSysColorMatrixFilter="ColorMatrixFilter";
const XPCTSTR _strSyscolors="colors";
const XPCTSTR _strSysColorTransform="ColorTransform";
const XPCTSTR _strSyscolorTransform="colorTransform";
const XPCTSTR _strSyscomponentX="componentX";
const XPCTSTR _strSyscomponentY="componentY";
const XPCTSTR _strSysconcat="concat";
const XPCTSTR _strSysconcatenatedColorTransform="concatenatedColorTransform";
const XPCTSTR _strSysconcatenatedMatrix="concatenatedMatrix";
const XPCTSTR _strSyscondenseWhite="condenseWhite";
const XPCTSTR _strSysconnect="connect";
const XPCTSTR _strSysconstructor="constructor";
const XPCTSTR _strSyscontains="contains";
const XPCTSTR _strSyscontainsPoint="containsPoint";
const XPCTSTR _strSyscontainsRectangle="containsRectangle";
const XPCTSTR _strSyscontentType="contentType";
const XPCTSTR _strSysContextMenu="ContextMenu";
const XPCTSTR _strSysContextMenuItem="ContextMenuItem";
const XPCTSTR _strSysCONTROL="CONTROL";
const XPCTSTR _strSysConvolutionFilter="ConvolutionFilter";
const XPCTSTR _strSyscopy="copy";
const XPCTSTR _strSyscopyChannel="copyChannel";
const XPCTSTR _strSyscopyPixels="copyPixels";
const XPCTSTR _strSyscos="cos";
const XPCTSTR _strSyscreateBox="createBox";
const XPCTSTR _strSyscreateEmptyMovieClip="createEmptyMovieClip";
const XPCTSTR _strSyscreateGradientBox="createGradientBox";
const XPCTSTR _strSyscreateTextField="createTextField";
const XPCTSTR _strSyscreationDate="creationDate";
const XPCTSTR _strSyscreator="creator";
const XPCTSTR _strSyscurrentFps="currentFps";
const XPCTSTR _strSyscurveTo="curveTo";
const XPCTSTR _strSysCustomActions="CustomActions";
const XPCTSTR _strSyscustomItems="customItems";
const XPCTSTR _strSysd="d";
const XPCTSTR _strSysdata="data";
const XPCTSTR _strSysDate="Date";
const XPCTSTR _strSysdeblocking="deblocking";
const XPCTSTR _strSysdecode="decode";
const XPCTSTR _strSysDELETEKEY="DELETEKEY";
const XPCTSTR _strSysdeltaTransformPoint="deltaTransformPoint";
const XPCTSTR _strSysDESCENDING="DESCENDING";
const XPCTSTR _strSysDisplacementMapFilter="DisplacementMapFilter";
const XPCTSTR _strSysdisplay="display";
const XPCTSTR _strSysdispose="dispose";
const XPCTSTR _strSysdistance="distance";
const XPCTSTR _strSysdivisor="divisor";
const XPCTSTR _strSysdocTypeDecl="docTypeDecl";
const XPCTSTR _strSysdomain="domain";
const XPCTSTR _strSysDOWN="DOWN";
const XPCTSTR _strSysdownload="download";
const XPCTSTR _strSysdraw="draw";
const XPCTSTR _strSysDropShadowFilter="DropShadowFilter";
const XPCTSTR _strSysduplicateMovieClip="duplicateMovieClip";
const XPCTSTR _strSysduration="duration";
const XPCTSTR _strSysE="E";
const XPCTSTR _strSysembedFonts="embedFonts";
const XPCTSTR _strSysenabled="enabled";
const XPCTSTR _strSysEND="END";
const XPCTSTR _strSysendFill="endFill";
const XPCTSTR _strSysENTER="ENTER";
const XPCTSTR _strSysequals="equals";
const XPCTSTR _strSysError="Error";
const XPCTSTR _strSysescape="escape";
const XPCTSTR _strSysESCAPE="ESCAPE";
const XPCTSTR _strSysSHIFT="SHIFT";
const XPCTSTR _strSyseval="eval";
const XPCTSTR _strSysexactSettings="exactSettings";
const XPCTSTR _strSysexp="exp";
const XPCTSTR _strSysExternalInterface="ExternalInterface";
const XPCTSTR _strSysfalse="false";
const XPCTSTR _strSysfileList="fileList";
const XPCTSTR _strSysFileReference="FileReference";
const XPCTSTR _strSysFileReferenceList="FileReferenceList";
const XPCTSTR _strSysfillRect="fillRect";
const XPCTSTR _strSysfilters="filters";
const XPCTSTR _strSysfindText="findText";
const XPCTSTR _strSysfirstChild="firstChild";
const XPCTSTR _strSysfloodFill="floodFill";
const XPCTSTR _strSysfloor="floor";
const XPCTSTR _strSysflush="flush";
const XPCTSTR _strSysfocusEnabled="focusEnabled";
const XPCTSTR _strSysfont="font";
const XPCTSTR _strSysfontFamily="fontFamily";
const XPCTSTR _strSysfontSize="fontSize";
const XPCTSTR _strSysfontStyle="fontStyle";
const XPCTSTR _strSysfontWeight="fontWeight";
const XPCTSTR _strSysfps="fps";
const XPCTSTR _strSysfromCharCode="fromCharCode";
const XPCTSTR _strSysfscommand="fscommand";
const XPCTSTR _strSysFunction="Function";
const XPCTSTR _strSysgain="gain";
const XPCTSTR _strSysgenerateFilterRect="generateFilterRect";
const XPCTSTR _strSysgeom="geom";
const XPCTSTR _strSysget="get";
const XPCTSTR _strSysgetAscii="getAscii";
const XPCTSTR _strSysgetBeginIndex="getBeginIndex";
const XPCTSTR _strSysgetBounds="getBounds";
const XPCTSTR _strSysgetBytesLoaded="getBytesLoaded";
const XPCTSTR _strSysgetBytesTotal="getBytesTotal";
const XPCTSTR _strSysgetCaretIndex="getCaretIndex";
const XPCTSTR _strSysgetCode="getCode";
const XPCTSTR _strSysgetColorBoundsRect="getColorBoundsRect";
const XPCTSTR _strSysgetCount="getCount";
const XPCTSTR _strSysgetDate="getDate";
const XPCTSTR _strSysgetDay="getDay";
const XPCTSTR _strSysgetDefaultLang="getDefaultLang";
const XPCTSTR _strSysgetDepth="getDepth";
const XPCTSTR _strSysgetEndIndex="getEndIndex";
const XPCTSTR _strSysgetFocus="getFocus";
const XPCTSTR _strSysgetFontList="getFontList";
const XPCTSTR _strSysgetFullYear="getFullYear";
const XPCTSTR _strSysgetHours="getHours";
const XPCTSTR _strSysgetInstanceAtDepth="getInstanceAtDepth";
const XPCTSTR _strSysgetLocal="getLocal";
const XPCTSTR _strSysgetMilliseconds="getMilliseconds";
const XPCTSTR _strSysgetMinutes="getMinutes";
const XPCTSTR _strSysgetMonth="getMonth";
const XPCTSTR _strSysgetNewTextFormat="getNewTextFormat";
const XPCTSTR _strSysgetNextHighestDepth="getNextHighestDepth";
const XPCTSTR _strSysgetPan="getPan";
const XPCTSTR _strSysgetPixel="getPixel";
const XPCTSTR _strSysgetPixel32="getPixel32";
const XPCTSTR _strSysgetProgress="getProgress";
const XPCTSTR _strSysgetProperty="getProperty";
const XPCTSTR _strSysgetRect="getRect";
const XPCTSTR _strSysgetRGB="getRGB";
const XPCTSTR _strSysgetSeconds="getSeconds";
const XPCTSTR _strSysgetSelected="getSelected";
const XPCTSTR _strSysgetSelectedText="getSelectedText";
const XPCTSTR _strSysgetSize="getSize";
const XPCTSTR _strSysgetStyle="getStyle";
const XPCTSTR _strSysgetStyleNames="getStyleNames";
const XPCTSTR _strSysgetSWFVersion="getSWFVersion";
const XPCTSTR _strSysgetText="getText";
const XPCTSTR _strSysgetTextFormat="getTextFormat";
const XPCTSTR _strSysgetTextRunInfo="getTextRunInfo";
const XPCTSTR _strSysgetTextSnapshot="getTextSnapshot";
const XPCTSTR _strSysgetTime="getTime";
const XPCTSTR _strSysgetTimer="getTimer";
const XPCTSTR _strSysgetTimezoneOffset="getTimezoneOffset";
const XPCTSTR _strSysgetTransform="getTransform";
const XPCTSTR _strSysgetURL="getURL";
const XPCTSTR _strSysgetUTCDate="getUTCDate";
const XPCTSTR _strSysgetUTCDay="getUTCDay";
const XPCTSTR _strSysgetUTCFullYear="getUTCFullYear";
const XPCTSTR _strSysgetUTCHours="getUTCHours";
const XPCTSTR _strSysgetUTCMilliseconds="getUTCMilliseconds";
const XPCTSTR _strSysgetUTCMinutes="getUTCMinutes";
const XPCTSTR _strSysgetUTCMonth="getUTCMonth";
const XPCTSTR _strSysgetUTCSeconds="getUTCSeconds";
const XPCTSTR _strSysgetUTCYear="getUTCYear";
const XPCTSTR _strSysgetVersion="getVersion";
const XPCTSTR _strSysgetVolume="getVolume";
const XPCTSTR _strSysgetYear="getYear";
const XPCTSTR _strSysglobalToLocal="globalToLocal";
const XPCTSTR _strSysGlowFilter="GlowFilter";
const XPCTSTR _strSysgotoAndPlay="gotoAndPlay";
const XPCTSTR _strSysgotoAndStop="gotoAndStop";
const XPCTSTR _strSysGradientBevelFilter="GradientBevelFilter";
const XPCTSTR _strSysGradientGlowFilter="GradientGlowFilter";
const XPCTSTR _strSysgreenMultiplier="greenMultiplier";
const XPCTSTR _strSysgreenOffset="greenOffset";
const XPCTSTR _strSysgridFitType="gridFitType";
const XPCTSTR _strSyshasAccessibility="hasAccessibility";
const XPCTSTR _strSyshasAudio="hasAudio";
const XPCTSTR _strSyshasAudioEncoder="hasAudioEncoder";
const XPCTSTR _strSyshasEmbeddedVideo="hasEmbeddedVideo";
const XPCTSTR _strSyshasIME="hasIME";
const XPCTSTR _strSyshasMP3="hasMP3";
const XPCTSTR _strSyshasOwnProperty="hasOwnProperty";
const XPCTSTR _strSyshasPrinting="hasPrinting";
const XPCTSTR _strSyshasScreenBroadcast="hasScreenBroadcast";
const XPCTSTR _strSyshasScreenPlayback="hasScreenPlayback";
const XPCTSTR _strSyshasStreamingAudio="hasStreamingAudio";
const XPCTSTR _strSyshasStreamingVideo="hasStreamingVideo";
const XPCTSTR _strSyshasVideoEncoder="hasVideoEncoder";
const XPCTSTR _strSysheight="height";
const XPCTSTR _strSyshide="hide";
const XPCTSTR _strSyshideBuiltInItems="hideBuiltInItems";
const XPCTSTR _strSyshideObject="hideObject";
const XPCTSTR _strSyshighlightAlpha="highlightAlpha";
const XPCTSTR _strSyshighlightColor="highlightColor";
const XPCTSTR _strSyshitArea="hitArea";
const XPCTSTR _strSyshitTest="hitTest";
const XPCTSTR _strSyshitTestTextNearPos="hitTestTextNearPos";
const XPCTSTR _strSysHOME="HOME";
const XPCTSTR _strSyshscroll="hscroll";
const XPCTSTR _strSyshtml="html";
const XPCTSTR _strSyshtmlText="htmlText";
const XPCTSTR _strSysid3="id3";
const XPCTSTR _strSysidentity="identity";
const XPCTSTR _strSysidMap="idMap";
const XPCTSTR _strSysifFrameLoaded="ifFrameLoaded";
const XPCTSTR _strSysignoreWhite="ignoreWhite";
const XPCTSTR _strSysIME="IME";
const XPCTSTR _strSysindent="indent";
const XPCTSTR _strSysindex="index";
const XPCTSTR _strSysindexOf="indexOf";
const XPCTSTR _strSysInfinity="Infinity";
const XPCTSTR _strSysinflate="inflate";
const XPCTSTR _strSysinflatePoint="inflatePoint";
const XPCTSTR _strSysinitialize="initialize";
const XPCTSTR _strSysinner="inner";
const XPCTSTR _strSysINSERT="INSERT";
const XPCTSTR _strSysinstall="install";
const XPCTSTR _strSysint="int";
const XPCTSTR _strSysinterpolate="interpolate";
const XPCTSTR _strSysintersection="intersection";
const XPCTSTR _strSysintersects="intersects";
const XPCTSTR _strSysinvert="invert";
const XPCTSTR _strSysisAccessible="isAccessible";
const XPCTSTR _strSysisActive="isActive";
const XPCTSTR _strSysisDebugger="isDebugger";
const XPCTSTR _strSysisDown="isDown";
const XPCTSTR _strSysisEmpty="isEmpty";
const XPCTSTR _strSysisFinite="isFinite";
const XPCTSTR _strSysisNaN="isNaN";
const XPCTSTR _strSysisPropertyEnumerable="isPropertyEnumerable";
const XPCTSTR _strSysisPrototypeOf="isPrototypeOf";
const XPCTSTR _strSysisToggled="isToggled";
const XPCTSTR _strSysitalic="italic";
const XPCTSTR _strSysJAPANESE_HIRAGANA="JAPANESE_HIRAGANA";
const XPCTSTR _strSysJAPANESE_KATAKANA_FULL="JAPANESE_KATAKANA_FULL";
const XPCTSTR _strSysJAPANESE_KATAKANA_HALF="JAPANESE_KATAKANA_HALF";
const XPCTSTR _strSysjoin="join";
const XPCTSTR _strSyskerning="kerning";
const XPCTSTR _strSysKey="Key";
const XPCTSTR _strSysknockout="knockout";
const XPCTSTR _strSysKOREAN="KOREAN";
const XPCTSTR _strSyslanguage="language";
const XPCTSTR _strSyslanguageCodeArray="languageCodeArray";
const XPCTSTR _strSyslastChild="lastChild";
const XPCTSTR _strSyslastIndexOf="lastIndexOf";
const XPCTSTR _strSysleading="leading";
const XPCTSTR _strSysLEFT="LEFT";
const XPCTSTR _strSysleft="left";
const XPCTSTR _strSysleftMargin="leftMargin";
const XPCTSTR _strSyslength="length";
const XPCTSTR _strSysletterSpacing="letterSpacing";
const XPCTSTR _strSyslineGradientStyle="lineGradientStyle";
const XPCTSTR _strSyslineStyle="lineStyle";
const XPCTSTR _strSyslineTo="lineTo";
const XPCTSTR _strSyslist="list";
const XPCTSTR _strSyslisteners="listeners";
const XPCTSTR _strSysLN10="LN10";
const XPCTSTR _strSysLN2="LN2";
const XPCTSTR _strSysload="load";
const XPCTSTR _strSysloadBitmap="loadBitmap";
const XPCTSTR _strSysloadClip="loadClip";
const XPCTSTR _strSysloaded="loaded";
const XPCTSTR _strSysloadLanguageXML="loadLanguageXML";
const XPCTSTR _strSysloadMovie="loadMovie";
const XPCTSTR _strSysloadMovieNum="loadMovieNum";
const XPCTSTR _strSysloadPolicyFile="loadPolicyFile";
const XPCTSTR _strSysloadSound="loadSound";
const XPCTSTR _strSysloadString="loadString";
const XPCTSTR _strSysloadStringEx="loadStringEx";
const XPCTSTR _strSysloadVariables="loadVariables";
const XPCTSTR _strSysloadVariablesNum="loadVariablesNum";
const XPCTSTR _strSysLoadVars="LoadVars";
const XPCTSTR _strSysLocalConnection="LocalConnection";
const XPCTSTR _strSysLocale="Locale";
const XPCTSTR _strSyslocalFileReadDisable="localFileReadDisable";
const XPCTSTR _strSyslocalName="localName";
const XPCTSTR _strSyslocalToGlobal="localToGlobal";
const XPCTSTR _strSyslog="log";
const XPCTSTR _strSysLOG10E="LOG10E";
const XPCTSTR _strSysLOG2E="LOG2E";
const XPCTSTR _strSysmanufacturer="manufacturer";
const XPCTSTR _strSysmapBitmap="mapBitmap";
const XPCTSTR _strSysmapPoint="mapPoint";
const XPCTSTR _strSysmarginLeft="marginLeft";
const XPCTSTR _strSysmarginRight="marginRight";
const XPCTSTR _strSysMath="Math";
const XPCTSTR _strSysMatrix="Matrix";
const XPCTSTR _strSysmatrix="matrix";
const XPCTSTR _strSysmatrixX="matrixX";
const XPCTSTR _strSysmatrixY="matrixY";
const XPCTSTR _strSysmax="max";
const XPCTSTR _strSysMAX_VALUE="MAX_VALUE";
const XPCTSTR _strSysmaxChars="maxChars";
const XPCTSTR _strSysmaxhscroll="maxhscroll";
const XPCTSTR _strSysmaxLevel="maxLevel";
const XPCTSTR _strSysmaxscroll="maxscroll";
const XPCTSTR _strSysmbchr="mbchr";
const XPCTSTR _strSysmblength="mblength";
const XPCTSTR _strSysmbord="mbord";
const XPCTSTR _strSysmbsubstring="mbsubstring";
const XPCTSTR _strSysmenu="menu";
const XPCTSTR _strSysmerge="merge";
const XPCTSTR _strSysmessage="message";
const XPCTSTR _strSysmethodName="methodName";
const XPCTSTR _strSysMicrophone="Microphone";
const XPCTSTR _strSysmin="min";
const XPCTSTR _strSysMIN_VALUE="MIN_VALUE";
const XPCTSTR _strSysMMExecute="MMExecute";
const XPCTSTR _strSysmode="mode";
const XPCTSTR _strSysmodificationDate="modificationDate";
const XPCTSTR _strSysmotionLevel="motionLevel";
const XPCTSTR _strSysmotionTimeOut="motionTimeOut";
const XPCTSTR _strSysMouse="Mouse";
const XPCTSTR _strSysmouseWheelEnabled="mouseWheelEnabled";
const XPCTSTR _strSysmoveTo="moveTo";
const XPCTSTR _strSysMovieClip="MovieClip";
const XPCTSTR _strSysMovieClipLoader="MovieClipLoader";
const XPCTSTR _strSysmultiline="multiline";
const XPCTSTR _strSysmuted="muted";
const XPCTSTR _strSysname="name";
const XPCTSTR _strSysnames="names";
const XPCTSTR _strSysnamespaceURI="namespaceURI";
const XPCTSTR _strSysNaN="NaN";
const XPCTSTR _strSysNEGATIVE_INFINITY="NEGATIVE_INFINITY";
const XPCTSTR _strSysNetConnection="NetConnection";
const XPCTSTR _strSysNetStream="NetStream";
const XPCTSTR _strSysnewline="newline";
const XPCTSTR _strSysnextFrame="nextFrame";
const XPCTSTR _strSysnextScene="nextScene";
const XPCTSTR _strSysnextSibling="nextSibling";
const XPCTSTR _strSysnodeName="nodeName";
const XPCTSTR _strSysnodeType="nodeType";
const XPCTSTR _strSysnodeValue="nodeValue";
const XPCTSTR _strSysnoise="noise";
const XPCTSTR _strSysnormalize="normalize";
const XPCTSTR _strSysnull="null";
const XPCTSTR _strSysNumber="Number";
const XPCTSTR _strSysNUMERIC="NUMERIC";
const XPCTSTR _strSysObject="Object";
const XPCTSTR _strSysoffset="offset";
const XPCTSTR _strSysoffsetPoint="offsetPoint";
const XPCTSTR _strSyson="on";
const XPCTSTR _strSysonActivity="onActivity";
const XPCTSTR _strSysonCancel="onCancel";
const XPCTSTR _strSysonChanged="onChanged";
const XPCTSTR _strSysonClipEvent="onClipEvent";
const XPCTSTR _strSysonClose="onClose";
const XPCTSTR _strSysonComplete="onComplete";
const XPCTSTR _strSysonConnect="onConnect";
const XPCTSTR _strSysonData="onData";
const XPCTSTR _strSysonDragOut="onDragOut";
const XPCTSTR _strSysonDragOver="onDragOver";
const XPCTSTR _strSysonEnterFrame="onEnterFrame";
const XPCTSTR _strSysonHTTPError="onHTTPError";
const XPCTSTR _strSysonHTTPStatus="onHTTPStatus";
const XPCTSTR _strSysonID3="onID3";
const XPCTSTR _strSysonIOError="onIOError";
const XPCTSTR _strSysonKeyDown="onKeyDown";
const XPCTSTR _strSysonKeyUp="onKeyUp";
const XPCTSTR _strSysonKillFocus="onKillFocus";
const XPCTSTR _strSysonLoad="onLoad";
const XPCTSTR _strSysonLoadComplete="onLoadComplete";
const XPCTSTR _strSysonLoadError="onLoadError";
const XPCTSTR _strSysonLoadInit="onLoadInit";
const XPCTSTR _strSysonLoadProgress="onLoadProgress";
const XPCTSTR _strSysonLoadStart="onLoadStart";
const XPCTSTR _strSysonMetaData="onMetaData";
const XPCTSTR _strSysonMouseDown="onMouseDown";
const XPCTSTR _strSysonMouseMove="onMouseMove";
const XPCTSTR _strSysonMouseUp="onMouseUp";
const XPCTSTR _strSysonMouseWheel="onMouseWheel";
const XPCTSTR _strSysonOpen="onOpen";
const XPCTSTR _strSysonPress="onPress";
const XPCTSTR _strSysonProgress="onProgress";
const XPCTSTR _strSysonRelease="onRelease";
const XPCTSTR _strSysonReleaseOutside="onReleaseOutside";
const XPCTSTR _strSysonResize="onResize";
const XPCTSTR _strSysonRollOut="onRollOut";
const XPCTSTR _strSysonRollOver="onRollOver";
const XPCTSTR _strSysonScroller="onScroller";
const XPCTSTR _strSysonSecurityError="onSecurityError";
const XPCTSTR _strSysonSelect="onSelect";
const XPCTSTR _strSysonSetFocus="onSetFocus";
const XPCTSTR _strSysonSoundComplete="onSoundComplete";
const XPCTSTR _strSysonStatus="onStatus";
const XPCTSTR _strSysonUnload="onUnload";
const XPCTSTR _strSysonXML="onXML";
const XPCTSTR _strSysopaqueBackground="opaqueBackground";
const XPCTSTR _strSysord="ord";
const XPCTSTR _strSysorientation="orientation";
const XPCTSTR _strSysos="os";
const XPCTSTR _strSyspageHeight="pageHeight";
const XPCTSTR _strSyspageWidth="pageWidth";
const XPCTSTR _strSyspaletteMap="paletteMap";
const XPCTSTR _strSyspaperHeight="paperHeight";
const XPCTSTR _strSyspaperWidth="paperWidth";
const XPCTSTR _strSysparentNode="parentNode";
const XPCTSTR _strSysparseCSS="parseCSS";
const XPCTSTR _strSysparseFloat="parseFloat";
const XPCTSTR _strSysparseInt="parseInt";
const XPCTSTR _strSyspassword="password";
const XPCTSTR _strSyspause="pause";
const XPCTSTR _strSysperlinNoise="perlinNoise";
const XPCTSTR _strSysPGDN="PGDN";
const XPCTSTR _strSysPGUP="PGUP";
const XPCTSTR _strSysPI="PI";
const XPCTSTR _strSyspixelAspectRatio="pixelAspectRatio";
const XPCTSTR _strSyspixelBounds="pixelBounds";
const XPCTSTR _strSyspixelDissolve="pixelDissolve";
const XPCTSTR _strSysplay="play";
const XPCTSTR _strSysplayerType="playerType";
const XPCTSTR _strSysPoint="Point";
const XPCTSTR _strSyspolar="polar";
const XPCTSTR _strSyspop="pop";
const XPCTSTR _strSysposition="position";
const XPCTSTR _strSysPOSITIVE_INFINITY="POSITIVE_INFINITY";
const XPCTSTR _strSyspow="pow";
const XPCTSTR _strSysprefix="prefix";
const XPCTSTR _strSyspreserveAlpha="preserveAlpha";
const XPCTSTR _strSysprevFrame="prevFrame";
const XPCTSTR _strSyspreviousSibling="previousSibling";
const XPCTSTR _strSysprevScene="prevScene";
const XPCTSTR _strSysprint="print";
const XPCTSTR _strSysprintAsBitmap="printAsBitmap";
const XPCTSTR _strSysprintAsBitmapNum="printAsBitmapNum";
const XPCTSTR _strSysPrintJob="PrintJob";
const XPCTSTR _strSysprintNum="printNum";
const XPCTSTR _strSysprototype="prototype";
const XPCTSTR _strSyspush="push";
const XPCTSTR _strSysquality="quality";
const XPCTSTR _strSysrandom="random";
const XPCTSTR _strSysrate="rate";
const XPCTSTR _strSysratios="ratios";
const XPCTSTR _strSysRectangle="Rectangle";
const XPCTSTR _strSysrectangle="rectangle";
const XPCTSTR _strSysredMultiplier="redMultiplier";
const XPCTSTR _strSysredOffset="redOffset";
const XPCTSTR _strSysregisterClass="registerClass";
const XPCTSTR _strSysremoveListener="removeListener";
const XPCTSTR _strSysremoveMovieClip="removeMovieClip";
const XPCTSTR _strSysremoveTextField="removeTextField";
const XPCTSTR _strSysreplaceSel="replaceSel";
const XPCTSTR _strSysreplaceText="replaceText";
const XPCTSTR _strSysrestrict="restrict";
const XPCTSTR _strSysRETURNINDEXEDARRAY="RETURNINDEXEDARRAY";
const XPCTSTR _strSysreverse="reverse";
const XPCTSTR _strSysrgb="rgb";
const XPCTSTR _strSysRIGHT="RIGHT";
const XPCTSTR _strSysright="right";
const XPCTSTR _strSysrightMargin="rightMargin";
const XPCTSTR _strSysrotate="rotate";
const XPCTSTR _strSysround="round";
const XPCTSTR _strSyssandboxType="sandboxType";
const XPCTSTR _strSysscale="scale";
const XPCTSTR _strSysscale9Grid="scale9Grid";
const XPCTSTR _strSysscaleMode="scaleMode";
const XPCTSTR _strSysscaleX="scaleX";
const XPCTSTR _strSysscaleY="scaleY";
const XPCTSTR _strSysscreenColor="screenColor";
const XPCTSTR _strSysscreenDPI="screenDPI";
const XPCTSTR _strSysscreenResolutionX="screenResolutionX";
const XPCTSTR _strSysscreenResolutionY="screenResolutionY";
const XPCTSTR _strSysscroll="scroll";
const XPCTSTR _strSysscrollRect="scrollRect";
const XPCTSTR _strSyssecurity="security";
const XPCTSTR _strSysseek="seek";
const XPCTSTR _strSysselectable="selectable";
const XPCTSTR _strSysSelection="Selection";
const XPCTSTR _strSyssend="send";
const XPCTSTR _strSyssendAndLoad="sendAndLoad";
const XPCTSTR _strSysseparatorBefore="separatorBefore";
const XPCTSTR _strSysserverString="serverString";
const XPCTSTR _strSysset="set";
const XPCTSTR _strSyssetAdvancedAntialiasingTable="setAdvancedAntialiasingTable";
const XPCTSTR _strSyssetBufferTime="setBufferTime";
const XPCTSTR _strSyssetClipboard="setClipboard";
const XPCTSTR _strSyssetDate="setDate";
const XPCTSTR _strSyssetDefaultLang="setDefaultLang";
const XPCTSTR _strSyssetEmpty="setEmpty";
const XPCTSTR _strSyssetFocus="setFocus";
const XPCTSTR _strSyssetFullYear="setFullYear";
const XPCTSTR _strSyssetGain="setGain";
const XPCTSTR _strSyssetHours="setHours";
const XPCTSTR _strSyssetInterval="setInterval";
const XPCTSTR _strSyssetLoadCallback="setLoadCallback";
const XPCTSTR _strSyssetMask="setMask";
const XPCTSTR _strSyssetMilliseconds="setMilliseconds";
const XPCTSTR _strSyssetMinutes="setMinutes";
const XPCTSTR _strSyssetMode="setMode";
const XPCTSTR _strSyssetMonth="setMonth";
const XPCTSTR _strSyssetMotionLevel="setMotionLevel";
const XPCTSTR _strSyssetNewTextFormat="setNewTextFormat";
const XPCTSTR _strSyssetPan="setPan";
const XPCTSTR _strSyssetPixel="setPixel";
const XPCTSTR _strSyssetPixel32="setPixel32";
const XPCTSTR _strSyssetProperty="setProperty";
const XPCTSTR _strSyssetQuality="setQuality";
const XPCTSTR _strSyssetRate="setRate";
const XPCTSTR _strSyssetRGB="setRGB";
const XPCTSTR _strSyssetSeconds="setSeconds";
const XPCTSTR _strSyssetSelectColor="setSelectColor";
const XPCTSTR _strSyssetSelected="setSelected";
const XPCTSTR _strSyssetSelection="setSelection";
const XPCTSTR _strSyssetSilenceLevel="setSilenceLevel";
const XPCTSTR _strSyssetString="setString";
const XPCTSTR _strSyssetStyle="setStyle";
const XPCTSTR _strSyssetTextFormat="setTextFormat";
const XPCTSTR _strSyssetTime="setTime";
const XPCTSTR _strSyssetTransform="setTransform";
const XPCTSTR _strSyssetUseEchoSuppression="setUseEchoSuppression";
const XPCTSTR _strSyssetUTCDate="setUTCDate";
const XPCTSTR _strSyssetUTCFullYear="setUTCFullYear";
const XPCTSTR _strSyssetUTCHours="setUTCHours";
const XPCTSTR _strSyssetUTCMilliseconds="setUTCMilliseconds";
const XPCTSTR _strSyssetUTCMinutes="setUTCMinutes";
const XPCTSTR _strSyssetUTCMonth="setUTCMonth";
const XPCTSTR _strSyssetUTCSeconds="setUTCSeconds";
const XPCTSTR _strSyssetVolume="setVolume";
const XPCTSTR _strSyssetYear="setYear";
const XPCTSTR _strSysshadowAlpha="shadowAlpha";
const XPCTSTR _strSysshadowColor="shadowColor";
const XPCTSTR _strSysSharedObject="SharedObject";
const XPCTSTR _strSyssharpness="sharpness";
const XPCTSTR _strSysshift="shift";
const XPCTSTR _strSysshow="show";
const XPCTSTR _strSysshowMenu="showMenu";
const XPCTSTR _strSysshowRedrawRegions="showRedrawRegions";
const XPCTSTR _strSysshowSettings="showSettings";
const XPCTSTR _strSyssilenceLevel="silenceLevel";
const XPCTSTR _strSyssilenceTimeOut="silenceTimeOut";
const XPCTSTR _strSyssin="sin";
const XPCTSTR _strSyssize="size";
const XPCTSTR _strSysslice="slice";
const XPCTSTR _strSyssmoothing="smoothing";
const XPCTSTR _strSyssort="sort";
const XPCTSTR _strSyssortOn="sortOn";
const XPCTSTR _strSysSound="Sound";
const XPCTSTR _strSysSPACE="SPACE";
const XPCTSTR _strSyssplice="splice";
const XPCTSTR _strSyssplit="split";
const XPCTSTR _strSyssqrt="sqrt";
const XPCTSTR _strSysSQRT1_2="SQRT1_2";
const XPCTSTR _strSysSQRT2="SQRT2";
const XPCTSTR _strSysStage="Stage";
const XPCTSTR _strSysstart="start";
const XPCTSTR _strSysstartDrag="startDrag";
const XPCTSTR _strSysstatus="status";
const XPCTSTR _strSysstop="stop";
const XPCTSTR _strSysstopAllSounds="stopAllSounds";
const XPCTSTR _strSysstopDrag="stopDrag";
const XPCTSTR _strSysstrength="strength";
const XPCTSTR _strSysString="String";
const XPCTSTR _strSysstringIDArray="stringIDArray";
const XPCTSTR _strSysStyleSheet="StyleSheet";
const XPCTSTR _strSysstyleSheet="styleSheet";
const XPCTSTR _strSyssubstr="substr";
const XPCTSTR _strSyssubstring="substring";
const XPCTSTR _strSyssubtract="subtract";
const XPCTSTR _strSysswapDepths="swapDepths";
const XPCTSTR _strSysSystem="System";
const XPCTSTR _strSysTAB="TAB";
const XPCTSTR _strSystabChildren="tabChildren";
const XPCTSTR _strSystabEnabled="tabEnabled";
const XPCTSTR _strSystabIndex="tabIndex";
const XPCTSTR _strSystabStops="tabStops";
const XPCTSTR _strSystan="tan";
const XPCTSTR _strSystarget="target";
const XPCTSTR _strSystargetPath="targetPath";
const XPCTSTR _strSystellTarget="tellTarget";
const XPCTSTR _strSystext="text";
const XPCTSTR _strSystextAlign="textAlign";
const XPCTSTR _strSystextColor="textColor";
const XPCTSTR _strSystextDecoration="textDecoration";
const XPCTSTR _strSysTextField="TextField";
const XPCTSTR _strSysTextFormat="TextFormat";
const XPCTSTR _strSystextHeight="textHeight";
const XPCTSTR _strSystextIndent="textIndent";
const XPCTSTR _strSysTextRenderer="TextRenderer";
const XPCTSTR _strSysTextSnapshot="TextSnapshot";
const XPCTSTR _strSystextWidth="textWidth";
const XPCTSTR _strSysthickness="thickness";
const XPCTSTR _strSysthis="this";
const XPCTSTR _strSysthreshold="threshold";
const XPCTSTR _strSystime="time";
const XPCTSTR _strSystoggleHighQuality="toggleHighQuality";
const XPCTSTR _strSystoLowerCase="toLowerCase";
const XPCTSTR _strSystop="top";
const XPCTSTR _strSystopLeft="topLeft";
const XPCTSTR _strSystoString="toString";
const XPCTSTR _strSystoUpperCase="toUpperCase";
const XPCTSTR _strSystrace="trace";
const XPCTSTR _strSystrackAsMenu="trackAsMenu";
const XPCTSTR _strSysTransform="Transform";
const XPCTSTR _strSystransform="transform";
const XPCTSTR _strSystransformPoint="transformPoint";
const XPCTSTR _strSystranslate="translate";
const XPCTSTR _strSystransparent="transparent";
const XPCTSTR _strSystrue="true";
const XPCTSTR _strSystx="tx";
const XPCTSTR _strSysty="ty";
const XPCTSTR _strSystype="type";
const XPCTSTR _strSysundefined="undefined";
const XPCTSTR _strSysunderline="underline";
const XPCTSTR _strSysunescape="unescape";
const XPCTSTR _strSysuninstall="uninstall";
const XPCTSTR _strSysunion="union";
const XPCTSTR _strSysUNIQUESORT="UNIQUESORT";
const XPCTSTR _strSysUNKNOWN="UNKNOWN";
const XPCTSTR _strSysunloadClip="unloadClip";
const XPCTSTR _strSysunloadMovie="unloadMovie";
const XPCTSTR _strSysunloadMovieNum="unloadMovieNum";
const XPCTSTR _strSysunshift="unshift";
const XPCTSTR _strSysunwatch="unwatch";
const XPCTSTR _strSysUP="UP";
const XPCTSTR _strSysupdateAfterEvent="updateAfterEvent";
const XPCTSTR _strSysupdateProperties="updateProperties";
const XPCTSTR _strSysupload="upload";
const XPCTSTR _strSysurl="url";
const XPCTSTR _strSysuseCodepage="useCodepage";
const XPCTSTR _strSysuseEchoSuppression="useEchoSuppression";
const XPCTSTR _strSysuseHandCursor="useHandCursor";
const XPCTSTR _strSysUTC="UTC";
const XPCTSTR _strSysvalueOf="valueOf";
const XPCTSTR _strSysvariable="variable";
const XPCTSTR _strSysversion="version";
const XPCTSTR _strSysVideo="Video";
const XPCTSTR _strSysvisible="visible";
const XPCTSTR _strSyswatch="watch";
const XPCTSTR _strSyswidth="width";
const XPCTSTR _strSyswordWrap="wordWrap";
const XPCTSTR _strSysx="x";
const XPCTSTR _strSysXML="XML";
const XPCTSTR _strSysxmlDecl="xmlDecl";
const XPCTSTR _strSysXMLNode="XMLNode";
const XPCTSTR _strSysXMLSocket="XMLSocket";
const XPCTSTR _strSysXMLUI="XMLUI";
const XPCTSTR _strSysy="y";
const XPCTSTR _strSysimplements="implements";
const XPCTSTR _strSys_version="$version";
const XPCTSTR _strSyssetTimeout="setTimeout";

const XPCTSTR _strPlayerVersion="JQB 10.0";
#define _SYSID(NAME) XSYSID_##NAME
#define _SYSNAME(NAME) _strSys##NAME
#define _SYSVAR(NAME) XXVar(_strSys##NAME)
#define _SYSCALL(NAME) AddPropertySystem(XXVar(_strSys##NAME),XSYSID_##NAME)
#endif
