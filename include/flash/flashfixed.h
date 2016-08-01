/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright � Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc. 
****************************************************************************/
// 990325 mnk	Fixed definition collision that was causing a warning.
// 980918 mnk	Include <FixMath.h> on Mac if not already included.


#ifndef FIXED_INCLUDED
#define FIXED_INCLUDED

#include "flashglobal.h"

#ifdef __ANDROID__
#include "GGType.h"
#endif

#define HAVE64BIT
// fixed 1.0 changed from "fixed1" to "fixed_1"
// to prevent collision with "fixed1" declared in "FixMath.h"
#define FIX16OFF		16
#define	fixed_1 		0x00010000L	

// fixed 2.0
#define	fixed2 		0x00020000L	
// fixed 0.5
#define	fixedHalf 	0x00008000L	
#define	infinity_as2	0x7FFFFFFFL
#define	negInfinity 0x80000000L
#define	fixedStdErr 0x0000003FL
// fixed sqrt(2)
#define	fixedSqrt2 	0x00016A0AL	

inline SFIXED FixedMulS(SFIXED a,SFIXED b)
{
	return (a*b+fixedHalf)>>16;
}
inline SFIXED FixedDivS(SFIXED a,SFIXED b)
{
	return ((a<<16)+(b>>1))/b;
}

#define FixedRound(a)		((S16)(((SFIXED)(a) + 0x8000L) >> 16))
#define FixedTrunc(a)		((S16)((SFIXED)(a) >> 16))

#define	FixedCeiling(a)		((S16)(((SFIXED)(a) + 0x8000L) >> 16))
#define	FixedFloor(a)		((S16)((SFIXED)(a) >> 16 ))

#define FixedToInt(a)		((S16)(((SFIXED)(a) + 0x8000L) >> 16))
#define IntToFixed(a)		((SFIXED)(a) << 16)
// Fixed integer constant
#define FC(a)				IntToFixed(a)		

#define FixedToFloat(a)		((float)(a) / fixed_1)
#define FloatToFixed(a) 	((SFIXED)((float)(a) * fixed_1))

#define FixedToDouble(a)	((XFDOUBLE)(a) / fixed_1)
#define DoubleToFixed(a) 	((SFIXED)((XFDOUBLE)(a) * fixed_1))

#define FixedAverage(a, b) (((a) + (b)) >> 1)

#define FixedAbs(x) ((x) < 0 ? -(x) : (x))
#define FixedMin(a, b) ((a) < (b) ? (a) : (b))
#define FixedMax(a, b) ((a) > (b) ? (a) : (b))
#define FixedEqual(a, b, err) (FixedAbs((a) - (b)) <= err)

SFIXED FixedNearestMultiple(SFIXED x, SFIXED factor);

// Note that all angles are handled in Fixed point degrees to simplify rounding issues
// they are kept in the range of 0 to 360 degrees

#ifdef FIXEDUSEFLOAT
// Generic Floating point routines for quick porting not fast enough for shipping code
SFIXED FixedMul(SFIXED, SFIXED);
SFIXED FixedDiv(SFIXED, SFIXED);
SFIXED FixedSin(SFIXED);
SFIXED FixedCos(SFIXED);
SFIXED FixedTan(SFIXED);
SFIXED FixedAtan2(SFIXED dy, SFIXED dx);

S32 _FPMul(S32 a, S32 b, int shift);
S32 _FPDiv(S32 a, S32 b, int rshift);

#else
#if defined(HAVE64BIT)
//#define S64			__int64

inline SFIXED FixedMul(SFIXED a, SFIXED b)
	{
	    return (S32) (((XS64)a * (XS64)b+fixed_1/2) >> 16);
		//XS64 v=(XS64)a * (XS64)b;
		//if(v>0) return (v+fixed_1/2)>>16;
		//else	return (v-fixed_1/2)>>16;//(v-fixed_1/2)>>16;
	}

	inline SFIXED FixedDiv(SFIXED a, SFIXED b)
	{
	  if (b == 0) {
		return 0;
	  }
//#if (__CORE_VERSION__>=0x02074000)
	  return (S32) (((b>>1)+((((XS64)a)) << 16)) / (XS64)b);
//#else
//	  return (S32) ((((((XS64)a)) << 16)) / (XS64)b);
//#endif
	}

	inline S32 _FPMul(S32 a, S32 b, int rshift)
	{
//#if (__CORE_VERSION__>=0x02074000)
	  if(rshift>1)
		 return (S32) ((((XS64)a * (XS64)b)+(1<<(rshift-1))) >> rshift);
	  return (S32) ((((XS64)a * (XS64)b)) >> rshift);
//#else
//	  return (S32) ((((XS64)a * (XS64)b)) >> rshift);
//#endif
	}

	inline S32 _FPDiv(S32 a, S32 b, int rshift)
	{
	  if ( b == 0 ) return a < 0 ? minS32 : maxS32;
//#if (__CORE_VERSION__>=0x02074000)
	  return (S32) (((b>>1)+((((XS64)a) << rshift))) / (XS64) b);
//#else
//	  return (S32) ((((XS64)a) << rshift) / (XS64) b);
//#endif
	}
#elif defined(_WIN32)

	// Routines for Win32

	inline S32 _FPDiv(S32 a, S32 b, int rshift)
	{
		S32 result;
		__asm {
			mov		eax, a			;   eax = a
			mov		ebx, b			;   ebx = b

			mov		ecx, rshift
			mov		ch, 32			;   cl = excess bits on the right
			sub		ch, cl			;   ch = excess bits on the left

			xor		di, di			;   product sign is positive

			cmp		eax, 0			;   if a < 0
			jns		short FPD322

			not		di				;     product sign toggles
			neg		eax				;     a = -a
			jns		FPD322			;     if a is still negative
			dec		eax				;       make it positive dammit

	FPD322:	cmp		ebx, 0			;   if b = 0
			jz		short FPD325	;     overflow

			jns		short FPD323	;   if b < 0

			not		di				;     product sign toggles
			neg		ebx				;     b = -b
			jns		FPD323			;     if b is still negative
			dec		ebx				;       make it positive dammit

	FPD323:

			; Check for overflow

			cdq						;   edx:eax = a

			push	eax
			div		ebx				;   edx = int( a / b )
			mov		edx, eax
			pop		eax

			xchg	cl, ch			;   cl = bits in integer part less sign
			dec		cl
			shr		edx, cl			;   edx = overflow bits
			inc		cl
			xchg	cl, ch

			or		edx, edx		;   If overflow bits contain data
			jnz		short FPD325	;     overflow

			; Perform division

			mov		edx, eax		;   edx:eax = a * 2 ** shift
			sal		eax, cl
			xchg	cl, ch
			sar		edx, cl
			xchg	cl, ch

			div		ebx				;   eax = a / b

			shl		edx, 1			;   edx = remainder * 2
			cmp		edx, ebx		;   if (remainder / b) > 0.5
			jc		short FPD326

			inc		eax				;     Round up

			cmp		eax, 0			;     If overflow
			jns		short FPD326

	FPD325:
			mov		eax, 07fffffffh	;       eax = max positve

			or		di, di			;       if quotient is negative
			jz		short FPD329

			inc		eax				;         eax = max negative

			jmp		short FPD329	;       Exit

	FPD326:	or		di, di			;   if result is negative
			jz		FPD329

			neg		eax				;     negate quotient

	FPD329:	mov		result, eax
		}
		return result;
	}

	inline S32 FixedDiv(S32 a, S32 b)
	{
		S32 result;
		__asm {
			mov		eax, a			;   eax = a
			mov		ebx, b			;   ebx = b

			mov		cx, 16			;	right shift count
			mov		ch, 32			;   cl = excess bits on the right
			sub		ch, cl			;   ch = excess bits on the left

			xor		di, di			;   product sign is positive

			cmp		eax, 0			;   if a < 0
			jns		short FPD322

			not		di				;     product sign toggles
			neg		eax				;     a = -a
			jns		FPD322			;     if a is still negative
			dec		eax				;       make it positive dammit

	FPD322:	cmp		ebx, 0			;   if b = 0
			jz		short FPD325	;     overflow

			jns		short FPD323	;   if b < 0

			not		di				;     product sign toggles
			neg		ebx				;     b = -b
			jns		FPD323			;     if b is still negative
			dec		ebx				;       make it positive dammit

	FPD323:

			; Check for overflow

			cdq						;   edx:eax = a

			push	eax
			div		ebx				;   edx = int( a / b )
			mov		edx, eax
			pop		eax

			xchg	cl, ch			;   cl = bits in integer part less sign
			dec		cl
			shr		edx, cl			;   edx = overflow bits
			inc		cl
			xchg	cl, ch

			or		edx, edx		;   If overflow bits contain data
			jnz		short FPD325	;     overflow

			; Perform division

			mov		edx, eax		;   edx:eax = a * 2 ** shift
			sal		eax, cl
			xchg	cl, ch
			sar		edx, cl
			xchg	cl, ch

			div		ebx				;   eax = a / b

			shl		edx, 1			;   edx = remainder * 2
			cmp		edx, ebx		;   if (remainder / b) > 0.5
			jc		short FPD326

			inc		eax				;     Round up

			cmp		eax, 0			;     If overflow
			jns		short FPD326

	FPD325:
			mov		eax, 07fffffffh	;       eax = max positve

			or		di, di			;       if quotient is negative
			jz		short FPD329

			inc		eax				;         eax = max negative

			jmp		short FPD329	;       Exit

	FPD326:	or		di, di			;   if result is negative
			jz		FPD329

			neg		eax				;     negate quotient

	FPD329:	mov		result, eax
		}
		return result;
	}

	// Use the inline assembly
	inline S32 _FPMul(S32 a, S32 b, int rshift)
	{
		FLASHASSERT(rshift > 0);
		S32 result;
		__asm {
			mov		eax, a			;   eax = a
			mov		ebx, b			;   ebx = b

			imul	ebx				;   edx:eax = a * b

			mov		ecx, rshift		;   cl = shift
			shrd	eax, edx, cl	;     result in eax
			adc		eax, 0			;     round up

			mov		result, eax
		}
		return result;
	}

	inline SFIXED FixedMul(SFIXED a, SFIXED b)
	{
		S32 result;
		__asm {
			mov		eax, a			;   eax = a
			mov		ebx, b			;   ebx = b

			imul	ebx				;   edx:eax = a * b
			shrd	eax, edx, 16	;     result in eax
			adc		eax, 0			;     round up

			mov		result, eax
		}
		return result;
	}
#else
inline SFIXED FixedMul(SFIXED a, SFIXED b)
{
  return (S32) (((double)a * (double)b)*(1.0F/fixed_1));
}

inline SFIXED FixedDiv(SFIXED a, SFIXED b)
{
  if (b == 0) {
	return 0;
  }
  return (S32) (((double)a*fixed_1) / (double)b);
}

inline S32 _FPMul(S32 a, S32 b, int rshift)
{
  return (S32)(((double)a*(double)b)/(1L<<rshift));
}

inline S32 _FPDiv(S32 a, S32 b, int rshift)
{
  if ( b == 0 ) return a < 0 ? minS32 : maxS32;
	return (S32)(((double)a)/((double)b)*(1L<<rshift));
}
#endif
//#include NATIVE_FIXED
inline S32 _FPRound(S32 a, int shift)
{
	if ( shift > 0 )
		return (a+(1<<(shift-1))) >> shift;
	else
		return a << -shift;
}
S32 _FPSin( S32 degrees, int inPrec, int outPrec );
S32 _FPCos( S32 degrees, int inPrec, int outPrec );
S32 _FPATan2( S32 y, S32 x, int inPrec, int outPrec );
//extern void _FPInit( S16 fpuFlag );

#define FixedSin( degree ) ((SFIXED)_FPSin(((S32)(degree)), 16, 16))
#define FixedCos( degree ) ((SFIXED)_FPCos(((S32)(degree)), 16, 16))
SFIXED FixedTan(SFIXED);
#define FixedAtan2( y, x ) ((SFIXED)_FPATan2(((S32)(y)), ((S32)(x)), 16, 16))

#endif
extern void CalcRotate(XFDOUBLE r, int &sinv, int &cosv,XU8 b360);
/*inline SFIXED IShift(SFIXED x, int s)
{
	if ( s >= 0 ) {
		return x << s;
	} else {
		return x >> s;
	}
} */

#endif
