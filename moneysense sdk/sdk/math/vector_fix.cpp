#include <cmath>

#include "vector_fix.hpp"

VectorDrop::VectorDrop(void)
{

}

VectorDrop::VectorDrop(vec_t X, vec_t Y)
{
	x = X; y = Y;
}

VectorDrop::VectorDrop(vec_t* clr)
{
	x = clr[0]; y = clr[1];
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

void VectorDrop::Init(vec_t ix, vec_t iy)
{
	x = ix; y = iy;
}

void VectorDrop::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
}

void Vector2DClear(VectorDrop& a)
{
	a.x = a.y = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

VectorDrop& VectorDrop::operator=(const VectorDrop&vOther)
{
	x = vOther.x; y = vOther.y;
	return *this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------

vec_t& VectorDrop::operator[](int i)
{
	return ((vec_t*)this)[i];
}

vec_t VectorDrop::operator[](int i) const
{
	return ((vec_t*)this)[i];
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------

vec_t* VectorDrop::Base()
{
	return (vec_t*)this;
}

vec_t const* VectorDrop::Base() const
{
	return (vec_t const*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

bool VectorDrop::IsValid() const
{
	return !isinf(x) && !isinf(y);
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

bool VectorDrop::operator==(const VectorDrop& src) const
{
	return (src.x == x) && (src.y == y);
}

bool VectorDrop::operator!=(const VectorDrop& src) const
{
	return (src.x != x) || (src.y != y);
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

void VectorDropCopy(const VectorDrop& src, VectorDrop& dst)
{
	dst.x = src.x;
	dst.y = src.y;
}

void VectorDrop::CopyToArray(float* rgfl) const
{
	rgfl[0] = x; rgfl[1] = y;
}

//-----------------------------------------------------------------------------
// standard Math operations
//-----------------------------------------------------------------------------

void VectorDrop::Negate()
{
	x = -x; y = -y;
}

void VectorDropAdd(const VectorDrop& a, const VectorDrop& b, VectorDrop& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
}

void VectorDropSubtract(const VectorDrop& a, const VectorDrop& b, VectorDrop& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
}

void VectorDropMultiply(const VectorDrop& a, vec_t b, VectorDrop& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
}

void VectorDropMultiply(const VectorDrop& a, const VectorDrop& b, VectorDrop& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
}

void VectorDropDivide(const VectorDrop& a, vec_t b, VectorDrop& c)
{
	vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
}

void VectorDropDivide(const VectorDrop& a, const VectorDrop& b, VectorDrop& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
}

void VectorDropMA(const VectorDrop& start, float s, const VectorDrop& dir, VectorDrop& result)
{
	result.x = start.x + s * dir.x;
	result.y = start.y + s * dir.y;
}

// FIXME: Remove
// For backwards compatability
void VectorDrop::MulAdd(const VectorDrop& a, const VectorDrop& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
}

void Vector2DLerp(const VectorDrop& src1, const VectorDrop& src2, vec_t t, VectorDrop& dest)
{
	dest[0] = src1[0] + (src2[0] - src1[0]) * t;
	dest[1] = src1[1] + (src2[1] - src1[1]) * t;
}

//-----------------------------------------------------------------------------
// dot, cross
//-----------------------------------------------------------------------------
vec_t DotProduct2D(const VectorDrop& a, const VectorDrop& b)
{
	return(a.x*b.x + a.y*b.y);
}

// for backwards compatability
vec_t VectorDrop::Dot(const VectorDrop& vOther) const
{
	return DotProduct2D(*this, vOther);
}

vec_t Vector2DNormalize(VectorDrop& v)
{
	vec_t l = v.Length();
	if (l != 0.0f) {
		v /= l;
	}
	else {
		v.x = v.y = 0.0f;
	}
	return l;
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
vec_t Vector2DLength(const VectorDrop& v)
{
	return (vec_t)sqrt(v.x*v.x + v.y*v.y);
}

vec_t Vector2DLengthDrop(const VectorDrop& v)
{
	return (vec_t)sqrt(v.x*v.x + v.y*v.y);
}

vec_t VectorDrop::NormalizeInPlace()
{
	return Vector2DNormalize(*this);
}

bool VectorDrop::IsLengthGreaterThan(float val) const
{
	return LengthSqr() > val*val;
}

bool VectorDrop::IsLengthLessThan(float val) const
{
	return LengthSqr() < val*val;
}

vec_t VectorDrop::Length(void) const
{
	return Vector2DLength(*this);
}

vec_t VectorDrop::LengthDrop(void) const
{
	return Vector2DLengthDrop(*this);
}

void Vector2DMin(const VectorDrop&a, const VectorDrop&b, VectorDrop&result)
{
	result.x = (a.x < b.x) ? a.x : b.x;
	result.y = (a.y < b.y) ? a.y : b.y;
}

void Vector2DMax(const VectorDrop&a, const VectorDrop&b, VectorDrop&result)
{
	result.x = (a.x > b.x) ? a.x : b.x;
	result.y = (a.y > b.y) ? a.y : b.y;
}

//-----------------------------------------------------------------------------
// Computes the closest point to vecTarget no farther than flMaxDist from vecStart
//-----------------------------------------------------------------------------
void ComputeClosestPoint2D(const VectorDrop& vecStart, float flMaxDist, const VectorDrop& vecTarget, VectorDrop*pResult)
{
	VectorDrop vecDelta;
	VectorDropSubtract(vecTarget, vecStart, vecDelta);
	float flDistSqr = vecDelta.LengthSqr();
	if (flDistSqr <= flMaxDist * flMaxDist) {
		*pResult = vecTarget;
	}
	else {
		vecDelta /= sqrt(flDistSqr);
		VectorDropMA(vecStart, flMaxDist, vecDelta, *pResult);
	}
}

//-----------------------------------------------------------------------------
// Returns a Vector2D with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------

VectorDrop VectorDrop::Min(const VectorDrop&vOther) const
{
	return VectorDrop(x < vOther.x ? x : vOther.x, y < vOther.y ? y : vOther.y);
}

VectorDrop VectorDrop::Max(const VectorDrop&vOther) const
{
	return VectorDrop(x > vOther.x ? x : vOther.x, y > vOther.y ? y : vOther.y);
}

//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

VectorDrop VectorDrop::operator-(void) const
{
	return VectorDrop(-x, -y);
}

VectorDrop VectorDrop::operator+(const VectorDrop& v) const
{
	VectorDrop res;
	VectorDropAdd(*this, v, res);
	return res;
}

VectorDrop VectorDrop::operator-(const VectorDrop& v) const
{
	VectorDrop res;
	VectorDropSubtract(*this, v, res);
	return res;
}

VectorDrop VectorDrop::operator*(float fl) const
{
	VectorDrop res;
	VectorDropMultiply(*this, fl, res);
	return res;
}

VectorDrop VectorDrop::operator*(const VectorDrop& v) const
{
	VectorDrop res;
	VectorDropMultiply(*this, v, res);
	return res;
}

VectorDrop VectorDrop::operator/(float fl) const
{
	VectorDrop res;
	VectorDropDivide(*this, fl, res);
	return res;
}

VectorDrop VectorDrop::operator/(const VectorDrop& v) const
{
	VectorDrop res;
	VectorDropDivide(*this, v, res);
	return res;
}

VectorDrop operator*(float fl, const VectorDrop& v)
{
	return v * fl;
}