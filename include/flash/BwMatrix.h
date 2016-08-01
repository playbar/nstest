#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "XData.h"

/// The SWF SWFMatrix record.
/// 
/// Conceptually, it represents a 3*3 linear transformation SWFMatrix like this:
/// 
///   | scale_x       rotateSkew_y  translate_x |
///   | rotateSkey_x  scale_y       traslate_y  |
///   | 0             0             1           |
/// 

class BwMatrix
{
public:
	BwMatrix();
	BwMatrix( int a1, int b1, int c1, int d1, int x1, int y1 );
	~BwMatrix();

	void set_identity();

	void concatenate( BwMatrix &m );

	 void concatenate_translation(int xoffset, int yoffset);

	 void concatenate_scale(double xscale, double yscale);

	 /// Set this SWFMatrix to a blend of m1 and m2, parameterized by t.
	 void set_lerp(const BwMatrix& m1, const BwMatrix& m2, float t);

	 /// Set the scale & rotation part of the SWFMatrix. angle in radians.
	 void set_scale_rotation(double x_scale, double y_scale, double angle);

	 /// Set x and y scales, rotation is unchanged.
	 void set_scale(double x_scale, double y_scale);

	 /// Set x scale, rotation any y scale are unchanged.
	 void set_x_scale(double scale);

	 /// Set y scale, rotation and x scale are unchanged.
	 void set_y_scale(double scale);

	 /// Set rotation in radians, scales component are unchanged.
	 void set_rotation(double rotation);

	 /// Set x translation in TWIPS
	 void set_x_translation(int x) { tx = x; }

	 /// Set y translation in TWIPS.
	 void set_y_translation(int y) {  ty = y; }

	 /// Set x and y translation in TWIPS.
	 void set_translation(int x, int y);

	 void transform(XPoint& p);

	 void transform( XU32& x, XU32& y);

	 void transform(XPoint* result, const XPoint& p);

	 /// Invert this SWFMatrix and return the result.
	 BwMatrix& invert();

	 /// return the magnitude scale of our x coord output
	 double get_x_scale();

	 /// return the magnitude scale of our y coord output
	 double get_y_scale();

	 /// return x translation n TWIPS unit.
	 int get_x_translation() const {  return tx; }

	 /// return y translation in TWIPS unit.
	 int get_y_translation() const { return ty; }

	 /// return rotation component in radians.
	 double get_rotation();

	 /// Allow direct access to values for equality
	 friend bool operator==(const BwMatrix& a, const BwMatrix& b);

private:
	 XU64  determinant();

public:
    XU32 a;
	XU32 b;
	XU32 c;
	XU32 d;
	XU32 tx;
	XU32 ty;
};

inline bool
operator==(const BwMatrix& mata, const BwMatrix& matb)
{
	return  
		mata.a == matb.a &&
		mata.b == matb.b &&
		mata.d == matb.d &&
		mata.c == matb.c &&
		mata.tx == matb.tx &&
		mata.ty == matb.ty;
}

#endif

