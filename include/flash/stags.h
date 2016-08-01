/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright ?Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc. 
****************************************************************************/

#ifndef STAGS_INCLUDED
#define STAGS_INCLUDED

enum {
		sMajorVersion = 3,
		sMinorVersion = 8,
		sVersion = ((sMajorVersion << 16) | sMinorVersion)
};

// Tags - the high 10 bits is the op code, the low 6 bits is the length
// if the length == 0x5f, the length is indicated by the following U32
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
	stagDefineVideo			= 60,	// a reference to an external video stream
	stagVideoFrame			= 61,
	stagDefineFontInfo2		= 62,
	stagFileAttrib			= 69,
	stagDefineFont3			= 75,
	stagDefineShape4		= 83,
	stagDefineFont4			= 91,
	stagDoInitAction	    = 59,
	stagDoABC			    = 82,
	stagDoABC2			    = 72,
	stagDefineMorphShape2	= 84,
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
	//stagDefineFontAlignZones= 73,
	stagSymbolClass			= 76,
	stagMetaData			= 77,
	stagDefineScalingGrid	= 78,
	stagDefineFrameLableData= 86,
	stagCSMTextSettings		= 74,
	stagDefineBinaryData	= 87,

	// NOTE: If tag values exceed 255 we need to expand SCharacter::tagCode from a BYTE to a WORD
	stagDefineBitsPtr		= 1023  // a special tag used only in the editor
};

// Flags for defining a shape character
enum {
		// These flag codes are used for state changes - and as return values from ShapeParser::GetEdge()
		eflagsMoveTo	   = 0x01,
		eflagsFill0	   	   = 0x02,
		eflagsFill1		   = 0x04,
		eflagsLine		   = 0x08,
		eflagsNewStyles	   = 0x10,

		eflagsEnd 	   	   = 0x80  // a state change with no change marks the end
};

// Button State flags
enum { 
	sbtnUpState			= (1<<0), 
	sbtnOverState		= (1<<1), 
	sbtnDownState		= (1<<2), 
	sbtnHitTestState	= (1<<3),
	sbtnFilterList		= 0x10,
	sbtnBlendMode		= 0x20,
};

// Button state transitions
enum { 
	bsIdleToOverUp = 0,		// mouse enter up
	bsOverUpToIdle,			// mouse exit up
	bsOverUpToOverDown,		// mouse down
	bsOverDownToOverUp,		// mouse up in

	// These transitions only apply when tracking "push" buttons
	bsOverDownToOutDown,	// mouse exit down
	bsOutDownToOverDown,	// mouse enter down
	bsOutDownToIdle,		// mouse up out

	// These transitions only apply when tracking "menu" buttons
	bsIdleToOverDown,		// mouse enter down
	bsOverDownToIdle		// mouse exit down
};

// Codes for saving a compact matrix
enum { 
		smatrixASimple = 1,		// a  == fixed1
		smatrixBSimple = 2,		// b  == 0
		smatrixCSimple = 4,		// c  == 0
		smatrixDSimple = 8,		// d  == fixed1
		smatrixTxSimple = 16,	// tx == 0
		smatrixTySimple = 32	// ty == 0
	};

// Flags for text chunks
enum {
		// These flag codes are used for state changes
		tflagsX			= 0x01,
		tflagsY			= 0x02,
		tflagsColor		= 0x04,
		tflagsFont		= 0x08,
		tflagsHeight	= 0x08
};

// Font style flags
enum {
		// Font style options
		tfontWideChars	= 0x01,
		tfontBold		= 0x02,
		tfontItalic		= 0x04,

		// Font style options
		tfontANSI		= 0x10,
		tfontShiftJIS	= 0x20,
		tfontUnicode	= 0x30
};

// Action codes
enum {
	// Flash 1 and 2 actions
	sactionHasLength	= 0x80,
	sactionNone			= 0x00,
	sactionGotoFrame	= 0x81,	// frame num (U16)
	sactionGetURL		= 0x83,	// url (STR), window (STR)
	sactionNextFrame	= 0x04,
	sactionPrevFrame	= 0x05,
	sactionPlay			= 0x06,
	sactionStop			= 0x07,
	sactionToggleQuality= 0x08,
	sactionStopSounds	= 0x09,
	sactionWaitForFrame	= 0x8A,	// frame needed (U16), actions to skip (U8)

	// Flash 3 Actions
	sactionSetTarget	= 0x8B,	// name (STR)
	sactionGotoLabel	= 0x8C,	// name (STR)

	// Flash 4 Actions
	sactionAdd			= 0x0A, // Stack IN: number, number, OUT: number
	sactionSubtract		= 0x0B, // Stack IN: number, number, OUT: number
	sactionMultiply		= 0x0C, // Stack IN: number, number, OUT: number
	sactionDivide		= 0x0D, // Stack IN: dividend, divisor, OUT: number
	sactionEquals		= 0x0E, // Stack IN: number, number, OUT: BOOL
	sactionLess			= 0x0F, // Stack IN: number, number, OUT: BOOL
	sactionAnd			= 0x10, // Stack IN: BOOL, BOOL, OUT: BOOL
	sactionOr			= 0x11, // Stack IN: BOOL, BOOL, OUT: BOOL
	sactionNot			= 0x12, // Stack IN: BOOL, OUT: BOOL
	sactionStringEquals	= 0x13, // Stack IN: string, string, OUT: BOOL
	sactionStringLength	= 0x14, // Stack IN: string, OUT: number
	sactionStringAdd	= 0x21, // Stack IN: string, strng, OUT: string
	sactionStringExtract= 0x15, // Stack IN: string, index, count, OUT: substring
	sactionPush			= 0x96, // type (U8), value (STRING or FLOAT)
	sactionPop			= 0x17, // no arguments
	sactionToInteger	= 0x18, // Stack IN: number, OUT: integer
	sactionJump			= 0x99, // offset (U16)
	sactionIf			= 0x9D, // offset (U16) Stack IN: BOOL
	sactionCall			= 0x9E, // Stack IN: name
	sactionGetVariable	= 0x1C, // Stack IN: name, OUT: value
	sactionSetVariable	= 0x1D, // Stack IN: name, value
	sactionGetURL2		= 0x9A, // method (U8) Stack IN: url, window
	sactionGotoFrame2	= 0x9F, // flags (U8) Stack IN: frame
	sactionSetTarget2	= 0x20, // Stack IN: target
	sactionGetProperty  = 0x22, // Stack IN: target, property, OUT: value
	sactionSetProperty  = 0x23, // Stack IN: target, property, value
    sactionCloneSprite  = 0x24, // Stack IN: source, target, depth
    sactionRemoveSprite = 0x25, // Stack IN: target
	sactionTrace        = 0x26, // Stack IN: message
	sactionStartDrag	= 0x27, // Stack IN: no constraint: 0, center, target
	                            //           constraint: x1, y1, x2, y2, 1, center, target
	sactionEndDrag		= 0x28, // no arguments
	sactionStringLess	= 0x29, // Stack IN: string, string, OUT: BOOL
	sactionWaitForFrame2= 0x8D, // skipCount (U8) Stack IN: frame
    sactionRandomNumber = 0x30, // Stack IN: maximum, OUT: result
	sactionMBStringLength = 0x31, // Stack IN: string, OUT: length
	sactionCharToAscii	= 0x32, // Stack IN: character, OUT: ASCII code
	sactionAsciiToChar	= 0x33, // Stack IN: ASCII code, OUT: character 
	sactionGetTime		= 0x34, // Stack OUT: milliseconds since Player start
	sactionMBStringExtract = 0x35,// Stack IN: string, index, count, OUT: substring
	sactionMBCharToAscii   = 0x36,// Stack IN: character, OUT: ASCII code
	sactionMBAsciiToChar   = 0x37,// Stack IN: ASCII code, OUT: character 

	// Flash 5 up
	sactionCastOp		   = 0x2B,
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

/*enum
{
    kStringType = 0,
    kFloatType = 1
};*/

enum 
{
    kSpritePosX = 0,
    kSpritePosY,
    kSpriteScaleX,
    kSpriteScaleY,
    kSpriteCurFrame,        // (can only get but not set)
    kSpriteTotalframes,     // (can only get but  not set)
    kSpriteAlpha,           // (a value between 0 and 100 %)
    kSpriteVisible,         // (if zero this means we don't hit test the object)
    kSpriteWidth,           // (can only get, but not set)
    kSpriteHeight,          // (can only get, but not set),
	kSpriteRotate,
	kSpriteTarget,
	kSpriteLastFrameLoaded,
	kSpriteName,
	kSpriteDropTarget,
	kSpriteURL,
	kSpriteHighQuality,		// (global)
	kSpriteFocusRect,		// (global)
	kSpriteSoundBufferTime,	// (global)
	kSpriteQulity,
	kSpriteXMouse=20,
	kSpriteYMouse=21
};

// Mouse target conditions
enum {
	stargetMouseEnter	= 1,
	stargetMouseExit	= 2,
	stargetMouseDown	= 3,
	stargetMouseUp		= 4
};

// Start Sound Flags
enum {
	soundHasInPoint		= 0x01,
	soundHasOutPoint	= 0x02,
	soundHasLoops		= 0x04,
	soundHasEnvelope	= 0x08

	// the upper 4 bits are reserved for synchronization flags
};

// Bitmap Alpha types
enum {
	sbitsAlphaFlag	= 0,	// just a flag that the alpha channel is valid
	sbitsAlphaCTab	= 1,	// alpha values for a color table
	sbitsAlphaMask	= 2		// a complete alpha mask for a jpeg image
};


// PlaceObject 2 flags
// format
//   depth (word)
//   character tag (word)
//   matrix
//   color transform w/alpha
//	 blend ratio (word)
//	 name (string)
enum {
	splaceMove			      = 0x01, // this place moves an exisiting object
	splaceCharacter		      = 0x02, // there is a character tag	(if no tag, must be a move)
	splaceMatrix		      = 0x04, // there is a matrix
	splaceColorTransform      = 0x08, // there is a color transform
	splaceRatio			      = 0x10, // there is a blend ratio
	splaceName			      = 0x20, // there is an object name
	splaceDefineClip	      = 0x40, // this shape should open or close a clipping bracket (character != 0 to open, character == 0 to close)
    splaceCloneExternalSprite = 0x80  // cloning a movie which was loaded externally
	// 1 bit left for expansion
};

enum 
{
	extPlaceHasFilterList	=0x01,
	extPlaceHasBlendMode	=0x02,
	extPlaceHasHasCache		=0x04,
	extPlaceHasClassName	=0x08,
	extPlaceHasImage		=0x10,
	extPlaceClipActions		=0x80
};

// Server Packet Flags
enum {
	spktObject		= 0x00,	// packet types
	spktFrame		= 0x01,
	spktMask		= 0x03,

	spktResend		= 0x04,	// flags for object packets

	spktSeekPoint	= 0x04,	// flags for frame packets
	spktKeyFrame	= 0x08

	// Upper 4 bits are reserved for a sequence number
};


// Template Text Flags.
enum {
	stextEnd		= 0x00,	// end of text flag
	stextStyle		= 0x80,	// font style: followed by 8-bit flags (bold, italic, etc...)
	stextFont		= 0x81,	// font identifier: followed by 16-bit font identifier
	stextSize		= 0x82, // font size: followed by 16-bit value in twips
	stextColor		= 0x83, // font color: followed by 32-bit RGB value
	stextPosition	= 0x84, // font position: followed by 8-bit position (normal, super or subscript)
	stextKerning	= 0x85, // font kerning: followed by 16-bit kerning value in twips
	stextReserved1	= 0x86,	// reserved value
	stextReserved2	= 0x87,	// reserved value
	stextAlignment	= 0x88,	// paragraph alignment: followed by 8-bit alignment value
	stextIndent		= 0x89, // paragraph alignment: followed by 16-bit indent value in twips
	stextLMargin	= 0x8a, // paragraph left margin: followed by 16-bit left margin value in twips
	stextRMargin	= 0x8b, // paragraph right margin: followed by 16-bit right margin value in twips
	stextLeading	= 0x8c, // paragraph leading: followed by 16-bit leading value in twips
	stextReserved3	= 0x8d,	// reserved value
	stextReserved4	= 0x8e,	// reserved value
	stextReserved5	= 0x8f	// reserved value
};

// Template Text Style Flags
enum {
	stextStyleNone		= 0x00,
	stextStyleBold		= 0x01,
	stextStyleItalic	= 0x02
	// 6 bits left for expansion
};

// Template Text Position Values
enum {
	stextPosNormal		= 0x00,
	stextPosSuperScript	= 0x01,
	stextPosSubScript	= 0x02
};

// Template Text Alignment Values
enum {
	stextAlignLeft		= 0x00,
	stextAlignRight		= 0x01,
	stextAlignCenter	= 0x02,
	stextAlignJustify	= 0x03,
	stextAlignEnd       = 0x04,
	stextAlignStart     = 0x05
};


// Template Text Flags
enum {
	sfontFlagsBold			= 0x01,
	sfontFlagsItalic		= 0x02,
	sfontFlagsWideCodes		= 0x04,
	sfontFlagsWideOffsets	= 0x08,
	sfontFlagsANSI			= 0x10,
	sfontFlagsUnicode		= 0x20,
	sfontFlagsShiftJIS		= 0x40,
	sfontFlagsHasLayout		= 0x80
//#if (__CORE_VERSION__>=0x02075000)
	,sfontFlagsSystem	    = 0x8000
//#endif
};

// GetURL2 methods
enum {
	kHttpDontSend			= 0,
	kHttpSendUseGet			= 1,
	kHttpSendUsePost		= 2,
	kHttpNoResponse			= 4,
	kHttpLoadSoundStream	= 0x10,
    kHttpLoadTarget         = 0x40,
	kHttpLoadData			= 0x20,
	kHttpLoadVariables		= 0x80,
	kHttpLoadRPC			= 0x100,
	kHttpLoadContext	    = 0xf0
};

// Edit Text Flags
enum {
	seditTextFlagsHasFont		= 0x0001,
	seditTextFlagsHasMaxLength	= 0x0002,
	seditTextFlagsHasTextColor	= 0x0004,
	seditTextFlagsReadOnly		= 0x0008,
	seditTextFlagsPassword		= 0x0010,
	seditTextFlagsMultiline		= 0x0020,
	seditTextFlagsWordWrap		= 0x0040,
	seditTextFlagsHasText		= 0x0080,
	seditTextFlagsUseOutlines	= 0x0100,
	seditTextFlagsHTML			= 0x0200,
	seditTextFlagsWasStatic		= 0x0400,
	seditTextFlagsBorder		= 0x0800,
	seditTextFlagsNoSelect		= 0x1000,
	seditTextFlagsHasLayout		= 0x2000,
	seditTextAutoSize			= 0x4000,
	seditTextHasFontClass		= 0x8000,
	seditTextFlagsFillBackground= 0x00010000,
	seditTextDelInitText		= 0x00020000,
	seditTextNoTabStop			= 0x00040000,
	seditTextDelVarText			= 0x00080000,
	seditTextAlwaysShowSelection= 0x00100000,
	seditTextCondenseWhite		= 0x00200000,
	seditTextLockHTML			= 0x00400000,
	//seditTextNoSelect			= 0x
};
/*enum {
	seditTextFlagsFillBackground=0x0001,
	seditTextDelInitText		=0x0002,
	seditTextNoTabStop			=0x0004,
	seditTextDelVarText			=0x0008,
	seditTextAlwaysShowSelection=0x0010
	//seditTextDel

};*/

// Drag constrants
enum {
	sdragFromPoint				= 0,
	sdragFromCenter				= 1
};
enum {
	sdragNoConstraint			= 0,
	sdragRectConstraint			= 1
};

enum 
{
	drawBlendNormal0 = 0,
	drawBlendNormal1 = 1,
	drawBlendLayer	 = 2,
	drawBlendMulty	 = 3,
	drawBlendScreen	 = 4,
	drawBlendLighten = 5,
	drawBlendDarken	 = 6,
	drawBlendDefrence= 7,
	drawBlendAdd	 = 8,
	drawBlendSubtrace= 9,
	drawBlendInvert	 =10,
	drawBlendAlpha	 =11,
	drawBlendErease  =12,
	drawBlendOverlay =13,
	drawBlendHardLight=14
};

#endif // STAGS_INCLUDED




