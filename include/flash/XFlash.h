// Copyright © 1999 Middlesoft, Inc. All rights reserved.
// First Created By Lee Thomason.
// First Created On 09/08/1999.
// Last Modified On 11/09/1999.

/****************************************************************************************

				File Summary: Macromedia.h

	This header file defines various structs and enums used by Flash File Format SDK
	low-level manager.

****************************************************************************************/

#ifndef _XFLASH_H_
#define _XFLASH_H_

#pragma warning (disable:4786)

// Global Types
/*typedef unsigned long U32, *P_U32, **PP_U32;
typedef signed long S32, *P_S32, **PP_S32;
typedef unsigned short U16, *P_U16, **PP_U16;
typedef signed short S16, *P_S16, **PP_S16;
typedef unsigned char U8, *P_U8, **PP_U8;
typedef signed char S8, *P_S8, **PP_S8;
typedef signed long SFIXED, *P_SFIXED;
typedef signed long SCOORD, *P_SCOORD;

#define maxU8			((U8)0xFF)
#define minS8			((S8)0x80)
#define maxS8			((S8)0x7F)
#define maxU16			((U16)0xFFFF)
#define minS16			((S16)0x8000)
#define maxS16			((S16)0x7FFF)
#define maxU32			((U32)0xFFFFFFFF)
#define minS32			((S32)0x80000000)
#define maxS32			((S32)0x7FFFFFFF)


#define Abs(v)			((v)<0?(-(v)):(v))
#define Max(a,b)		((a)>(b)?(a):(b))
#define Min(a,b)		((a)<(b)?(a):(b))
   
#define LowU16(dw)		((U16)(U32)(dw))
#define HighU16(dw)		((U16)((U32)(dw)>>16))
#define LowU8(w)		((U8)(w))
#define HighU8(w)		((U8)((U16)(w)>>8))
#define MakeU16(lb,hb)	(((U16)(hb)<<8)|(U16)(lb))
#define MakeU32(lw,hw)	(((U32)(hw)<<16)|(U32)(lw))*/

// Tag values that represent actions or data in a Flash script.
enum
{ 
    stagEnd 				= 0,
    stagShowFrame 			= 1,
    stagDefineShape		 	= 2,
    stagFreeCharacter 		= 3,
    stagPlaceObject 		= 4,
    stagRemoveObject 		= 5,
    stagDefineBits 			= 6,
    stagDefineButton 		= 7,
    stagJPEGTables 			= 8,
    stagSetBackgroundColor	= 9,
    stagDefineFont			= 10,
    stagDefineText			= 11,
    stagDoAction			= 12,
    stagDefineFontInfo		= 13,
    stagDefineSound			= 14,	// Event sound tags.
    stagStartSound			= 15,
    stagDefineButtonSound	= 17,
    stagSoundStreamHead		= 18,
    stagSoundStreamBlock	= 19,
    stagDefineBitsLossless	= 20,	// A bitmap using lossless zlib compression.
    stagDefineBitsJPEG2		= 21,	// A bitmap using an internal JPEG compression table.
    stagDefineShape2		= 22,
    stagDefineButtonCxform	= 23,
    stagProtect				= 24,	// This file should not be importable for editing.

	stagPathsArePostScript	= 25,	// assume shapes are filled as PostScript style paths

    // These are the new tags for Flash 3.
    stagPlaceObject2		= 26,	// The new style place w/ alpha color transform and name.
	stagPlaceObject3		= 70,
    stagRemoveObject2		= 28,	// A more compact remove object that omits the character tag (just depth).

	// This tag is used for RealMedia only
	stagSyncFrame			= 29, // Handle a synchronization of the display list

	stagFreeAll				= 31, // Free all of the characters


    stagDefineShape3		= 32,	// A shape V3 includes alpha values.
    stagDefineText2			= 33,	// A text V2 includes alpha values.
    stagDefineButton2		= 34,	// A button V2 includes color transform, alpha and multiple actions
    stagDefineBitsJPEG3		= 35,	// A JPEG bitmap with alpha info.
    stagDefineBitsLossless2 = 36,	// A lossless bitmap with alpha info.
    stagDefineSprite		= 39,	// Define a sequence of tags that describe the behavior of a sprite.
    stagNameCharacter		= 40,	// Name a character definition, character id and a string, (used for buttons, bitmaps, sprites and sounds).
    
	stagSerialNumber		= 41,	// a tag command for the Flash Generator customer serial id and cpu information
	stagDefineTextFormat	= 42,	// define the contents of a text block with formating information
	
	stagFrameLabel			= 43,	// A string label for the current frame.
    stagSoundStreamHead2	= 45,	// For lossless streaming sound, should not have needed this...
    stagDefineMorphShape	= 46,	// A morph shape definition
    
	stagFrameTag			= 47,	// a tag command for the Flash Generator (WORD duration, STRING label)
	stagDefineFont2			= 48,	// a tag command for the Flash Generator Font information
	stagGenCommand			= 49,	// a tag command for the Flash Generator intrinsic
	stagDefineCommandObj	= 50,	// a tag command for the Flash Generator intrinsic Command
	stagCharacterSet		= 51,	// defines the character set used to store strings
	stagFontRef				= 52,   // defines a reference to an external font source
	stagDefineFontAlignZones= 73,
	// Flash 4 tags
	stagDefineEditText		= 37,	// an edit text object (bounds, width, font, variable name)
	stagDefineVideo			= 38,	// a reference to an external video stream
	stagDefineFontInfo2		= 62,
	stagFileAttrib			= 69,
	stagDefineFont3			= 75,
	stagDefineShape4		= 83,
	stagDefineFont4			= 91,
	stagDoInitAction	    = 59,
	stagDoABC			    = 82,
	stagDefineFontName		= 88,
	stagStartSound2			= 89,
	stagDefineBitsJPEG4		= 90,

	stagExportAssets		= 56,
	stagImportAssets		= 57,
	stagEnableDebug			= 58,
	stagEnableDebug2		= 64,
	stagScriptLimits		= 65,
	stagSetTableIndex		= 66,
	stagImportAssets2		= 71,
	stagSymbolClass			= 76,
	stagMetaData			= 77,
	stagDefineScalingGrid	= 78,
	stagDefineFrameLableData= 86,


	// NOTE: If tag values exceed 255 we need to expand SCharacter::tagCode from a BYTE to a WORD
	stagDefineBitsPtr		= 1023  // a special tag used only in the editor
};

// Action codes
enum {
	// Flash 1 and 2 actions
	sactionHasLength	= 0x80,
	sactionNone			= 0x00,
	sactionGotoFrame	= 0x81,	// frame num (WORD)
	sactionGetURL		= 0x83,	// url (STR), window (STR)
	sactionNextFrame	= 0x04,
	sactionPrevFrame	= 0x05,
	sactionPlay			= 0x06,
	sactionStop			= 0x07,
	sactionToggleQuality= 0x08,
	sactionStopSounds	= 0x09,
	sactionWaitForFrame	= 0x8A,	// frame needed (WORD), actions to skip (BYTE)

	// Flash 3 Actions
	sactionSetTarget	= 0x8B,	// name (STR)
	sactionGotoLabel	= 0x8C,	// name (STR)

	// Flash 4 Actions
	sactionAdd			= 0x0A, // Stack IN: number, number, OUT: number
	sactionSubtract		= 0x0B, // Stack IN: number, number, OUT: number
	sactionMultiply		= 0x0C, // Stack IN: number, number, OUT: number
	sactionDivide		= 0x0D, // Stack IN: dividend, divisor, OUT: number
	sactionEquals		= 0x0E, // Stack IN: number, number, OUT: bool
	sactionLess			= 0x0F, // Stack IN: number, number, OUT: bool
	sactionAnd			= 0x10, // Stack IN: bool, bool, OUT: bool
	sactionOr			= 0x11, // Stack IN: bool, bool, OUT: bool
	sactionNot			= 0x12, // Stack IN: bool, OUT: bool
	sactionStringEquals	= 0x13, // Stack IN: string, string, OUT: bool
	sactionStringLength	= 0x14, // Stack IN: string, OUT: number
	sactionStringAdd	= 0x21, // Stack IN: string, strng, OUT: string
	sactionStringExtract= 0x15, // Stack IN: string, index, count, OUT: substring
	sactionPush			= 0x96, // type (BYTE), value (STRING or FLOAT)
	sactionPop			= 0x17, // no arguments
	sactionToInteger	= 0x18, // Stack IN: number, OUT: integer
	sactionJump			= 0x99, // offset (WORD)
	sactionIf			= 0x9D, // offset (WORD) Stack IN: bool
	sactionCall			= 0x9E, // Stack IN: name
	sactionGetVariable	= 0x1C, // Stack IN: name, OUT: value
	sactionSetVariable	= 0x1D, // Stack IN: name, value
	sactionGetURL2		= 0x9A, // method (BYTE) Stack IN: url, window
	sactionGotoFrame2	= 0x9F, // flags (BYTE) Stack IN: frame
	sactionSetTarget2	= 0x20, // Stack IN: target
	sactionGetProperty  = 0x22, // Stack IN: target, property, OUT: value
	sactionSetProperty  = 0x23, // Stack IN: target, property, value
    sactionCloneSprite  = 0x24, // Stack IN: source, target, depth
    sactionRemoveSprite = 0x25, // Stack IN: target
	sactionTrace        = 0x26, // Stack IN: message
	sactionStartDrag	= 0x27, // Stack IN: no constraint: 0, center, target
	                            //           constraint: x1, y1, x2, y2, 1, center, target
	sactionEndDrag		= 0x28, // no arguments
	sactionStringLess	= 0x29, // Stack IN: string, string, OUT: bool
	sactionWaitForFrame2= 0x8D, // skipCount (BYTE) Stack IN: frame
    sactionRandomNumber = 0x30, // Stack IN: maximum, OUT: result
	sactionMBStringLength = 0x31, // Stack IN: string, OUT: length
	sactionCharToAscii	= 0x32, // Stack IN: character, OUT: ASCII code
	sactionAsciiToChar	= 0x33, // Stack IN: ASCII code, OUT: character 
	sactionGetTime		= 0x34, // Stack OUT: milliseconds since Player start
	sactionMBStringExtract = 0x35,// Stack IN: string, index, count, OUT: substring
	sactionMBCharToAscii   = 0x36,// Stack IN: character, OUT: ASCII code
	sactionMBAsciiToChar   = 0x37,// Stack IN: ASCII code, OUT: character 

	sactionCallFunction	   = 0x3D,
	sactionCallMethod	   = 0x52,
	sactionConstantPool	   = 0x88,
	sactionDefineFunction  = 0x9B,
	sactionDefineLocal	   = 0x3C,
	sactionDefineLocal2	   = 0x41,
	sactionDelete		   = 0x3A,
	sactionDelete2		   = 0x3B,
	sactionEnumerate       = 0x46,
	sactionEquals2		   = 0x49,
	sactionGetMember	   = 0x4E,
	sactionInitArray	   = 0x42,
	sactionInitObject      = 0x43,
	sactionNewMethod	   = 0x53,
	sactionNewObject       = 0x40,
	sactionSetMember       = 0x4F,
	sactionTargetPath	   = 0x45,
	sactionWith			   = 0x94,
	sactionToNumber		   = 0x4A,
	sactionToString		   = 0x4B,
	sactionTypeOf		   = 0x44,
	sactionAdd2			   = 0x47,
	sactionLess2		   = 0x48,
	sactionModulo		   = 0x3F,
	sactionBitAnd		   = 0x60,
	sactionBitLShift	   = 0x63,
	sactionBitOr		   = 0x61,
	sactionBitRShift	   = 0x64,
	sactionBitURShift	   = 0x65,
	sactionBitXor		   = 0x62,
	sactionDecrement	   = 0x51,
	sactionIncrement	   = 0x50,
	sactionPushDulicate	   = 0x4C,
	sactionReturn		   = 0x3E,
	sactionStackSwap	   = 0x4D,
	sactionStoreRegister   = 0x87,
	sactionInstanceOf	   = 0x54,
	sactionEnumerate2	   = 0x55,
	sactionStrictEquals	   = 0x66,
	sactionGreater		   = 0x67,
	sactionStringGreater   = 0x68,
	sactionDefineFunction2 = 0x8E,
	sactionExtends		   = 0x69,
	sactionImplementsOp    = 0x2C,
	sactionTry			   = 0x8F,
	sactionThrow		   = 0x2A,

	// Reserved for Quicktime
	sactionQuickTime	= 0xAA	// I think this is what they are using...
};

#define ANTIZOOM 2
#define ANTIADD	 3
#define ANTISTEP 4
#define XSWFTRIP 20
#define XFIXED1	 0x10000
#define XFixedToFloat(VV)  (float)VV/XFIXED1
#define XFloatToFixed(VV)  (XS32)((float)VV*XFIXED1)
#define XFixedToDouble(VV) (double)VV/XFIXED1
#define XDoubleToFixed(VV) (XS32)((double)VV*XFIXED1)
#define XIntRound(VV)	  (int)(VV>=0?VV+0.5F:VV-0.5F)

#define XRatioI(V1,V2,RATIO) (V1+(((V2-V1)*RATIO)>>16))
#define XRatioFixed(V1,V2,RATIO) (V1+FixedMul(V2-V1,RATIO))
#define XRatioF(V1,V2,RATIO) (V1+((V2-V1)*RATIO/XFIXED1))
#define PI				3.1415926F
#define PI2				1.5707963F

#define XCMD_CREATEFLASHEDIT 900
#define XCMD_DESTORYFLASHEDIT 901
#define XCMD_PROCFLASHTEXT	  902

//#define FLASHASSERT(VV)

#endif