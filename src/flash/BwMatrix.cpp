#include "StdAfxflash.h"
#include "BwMatrix.h"

//#include <limits>
//using namespace std;

//#ifdef __APPLE__
#include "math.h"
//#endif

template<typename T>
inline T lerp(T a, T b, T f)
{
	return (b - a) * f + a;
}

XU32 truncateWithFactor(double a)
{ 
	//const double factor = 65536;// static_cast<double>(Factor);
	//static const double upperUnsignedLimit = std::numeric_limits<unsigned int>::max() + 1.0;
	//static const double upperSignedLimit = std::numeric_limits<int>::max() / factor;
	//static const double lowerSignedLimit = std::numeric_limits<int>::min() / factor;

	//if (a >= lowerSignedLimit && a <= upperSignedLimit)
	//{
	//	return a * 65536;
	//}
	//// This slow truncation happens only in very unlikely cases.
	//return a >= 0 ?
	//	(XU32)(std::fmod(a * factor, upperUnsignedLimit))
	//	: 
	//	(XU32)(std::fmod(-a * factor, upperUnsignedLimit));
	return a;
}


inline XU32 multiplyFixed16(XU32 a, XU32 b)
{
	return ((XU64)a *(XU64) b + 0x8000) >> 16;
}

inline XU32 toFixed16(double a)
{
	return truncateWithFactor(a);
}

inline double rotationX(const BwMatrix& m)
{
	const double b = m.b;
	const double a = m.a;
	return atan2(b, a);
}

inline double rotationY(const BwMatrix& m)
{
	const double c = m.c;
	const double d = m.d;
	return atan2(-c, d);
}

//////////////////////////////////////////////////////////////////////////

BwMatrix::BwMatrix() : a(65536), b( 0 ), c(0), d( 65536), tx(0), ty(0)
{

}

BwMatrix::BwMatrix( int a1, int b1, int c1, int d1, int x1, int y1 ) :
	a(a1), b(b1), c( c1), d( d1), tx( x1), ty( y1 )
{

}

BwMatrix::~BwMatrix()
{

}

void BwMatrix::set_identity()
{
	a = d = 65536;
	b = c = tx = ty = 0;
}

void BwMatrix::concatenate( BwMatrix &m )
{
	BwMatrix t;
	t.a = multiplyFixed16( a, m.a ) + multiplyFixed16( c, m.b );
	t.b = multiplyFixed16(b, m.a) + multiplyFixed16(d, m.b);
	t.c = multiplyFixed16(a, m.c) + multiplyFixed16(c, m.d);
	t.d = multiplyFixed16(b, m.c) + multiplyFixed16(d, m.d);
	t.tx = multiplyFixed16(a, m.tx) + multiplyFixed16(c, m.ty) + tx;
	t.ty = multiplyFixed16(b, m.tx) + multiplyFixed16(d, m.ty) + ty;
	*this = t;
}

void BwMatrix::concatenate_translation(int xoffset, int yoffset)
{
	tx += multiplyFixed16(a, xoffset) + multiplyFixed16(c, yoffset);
	ty += multiplyFixed16(b, xoffset) + multiplyFixed16(d, yoffset);
	return;
}

void BwMatrix::concatenate_scale(double xscale, double yscale)
{
	a = multiplyFixed16(a, toFixed16(xscale));
	c = multiplyFixed16(c, toFixed16(yscale));
	b = multiplyFixed16(b, toFixed16(xscale));
	d = multiplyFixed16(d, toFixed16(yscale)); 
}

void BwMatrix::set_lerp(const BwMatrix& m1, const BwMatrix& m2, float t)
{
	a = lerp<float>(m1.a, m2.a, t);
	b = lerp<float>(m1.b, m2.b, t);
	c = lerp<float>(m1.c, m2.c, t);
	d = lerp<float>(m1.d, m2.d, t);
	tx = lerp<float>(m1.tx, m2.tx, t);
	ty = lerp<float>(m1.ty, m2.ty, t);
}

void BwMatrix::set_scale_rotation(double x_scale, double y_scale, double angle)
{
	const double cos_angle = cos(angle);
	const double sin_angle = sin(angle);
	a = toFixed16(x_scale * cos_angle);
	c = toFixed16(y_scale * -sin_angle);
	b = toFixed16(x_scale * sin_angle);
	d = toFixed16(y_scale * cos_angle); 
}

void BwMatrix::set_scale(double x_scale, double y_scale)
{
	const double rotation = get_rotation();
	set_scale_rotation(x_scale, y_scale, rotation); 
}

void BwMatrix::set_x_scale(double xscale)
{
	const double rot_x = rotationX(*this);
	a = toFixed16(xscale * cos(rot_x));
	b = toFixed16(xscale * sin(rot_x));
}

void BwMatrix::set_y_scale(double yscale)
{
	const double rot_y = rotationY(*this);
	c = -toFixed16(yscale * sin(rot_y));
	d = toFixed16(yscale * cos(rot_y));
}

void BwMatrix::set_rotation(double rotation)
{
	const double rot_x = rotationX(*this);
	const double rot_y = rotationY(*this);

	const double scale_x = get_x_scale();
	const double scale_y = get_y_scale();

	a = toFixed16(scale_x * cos(rotation));
	b = toFixed16(scale_x * sin(rotation));
	c = -toFixed16(scale_y * sin(rot_y - rot_x + rotation));
	d = toFixed16(scale_y * cos(rot_y - rot_x + rotation));
	return;
}

void BwMatrix::set_translation(int x, int y) 
{
	tx = x;
	ty = y;
}

void BwMatrix::transform(XPoint& p)
{
	XU32 t0 = multiplyFixed16(a, p.x) + multiplyFixed16(c, p.y) + tx;
	XU32 t1 = multiplyFixed16(b, p.x) + multiplyFixed16(d, p.y) + ty;
	p.x = t0;
	p.y = t1;
}

void BwMatrix::transform( XU32& x, XU32& y)
{
	XU32 t0 = multiplyFixed16(a, x) + multiplyFixed16(c, y) + tx;
	XU32 t1 = multiplyFixed16(b, x) + multiplyFixed16(d,  y) + ty;
	x = t0;
	y = t1;
}

void BwMatrix::transform(XPoint* result, const XPoint& p)
{
	result->x = multiplyFixed16(a, p.x) + multiplyFixed16(c, p.y) + tx;
	result->y = multiplyFixed16(b, p.x) + multiplyFixed16(d, p.y) + ty;
}

BwMatrix& BwMatrix::invert()
{
	XU64 det = determinant();

	if (det == 0) 
	{
		set_identity();
		return *this;
	}

	const double dn = 65536.0 * 65536.0 / det;

	XU32 t0 = (XU32)(d * dn);
	d = (XU32)(a * dn);
	c = (XU32)(-c * dn);
	b = (XU32)(-b * dn);

	XU32 t4 = -(multiplyFixed16(tx, t0) + multiplyFixed16(ty, c));
	ty = -(multiplyFixed16(tx, b) + multiplyFixed16(ty, d));

	a = t0;
	tx = t4;

	return *this;
}

double BwMatrix::get_x_scale()
{
	const double a2 = pow((double)a, 2);
	const double b2 = pow((double)b, 2);
	return sqrt(a2 + b2) / 65536.0;
}

double BwMatrix::get_y_scale()
{
	const double d2 = pow((double)d, 2);
	const double c2 = pow((double)c, 2);
	return sqrt(d2 + c2) / 65536.0;
}

double BwMatrix::get_rotation()
{
	return rotationX(*this);
}

XU64  BwMatrix::determinant()
{
	// | _a	_c	_tx |
	// | _b	_d	_ty |   = T. Using the Leibniz formula:
	// | 0	0	1  |
	//
	// Det(T) = ( (_a * _d * 1 ) + (_c * _ty * 0) + (_tx * _b *  0) ) -
	//          ( (0  * _d * _tx) + (0  * _ty * _a) + (1 * _c * _b) )
	//        = _a * _d - _b * _c
	return (XU64)a * d - (XU64)b * c;
}
