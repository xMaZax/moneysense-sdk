#pragma once

typedef float vec_t;
// 2D Vector
class VectorDrop
{
public:
	// Members
	vec_t x, y, z;
	//float xe, ye, ze;
	// Construction/destruction:
	VectorDrop(void);
	VectorDrop(vec_t X, vec_t Y);
	VectorDrop(vec_t* clr);

	VectorDrop(const VectorDrop&vOther)
	{
		x = vOther.x; y = vOther.y;
	}

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f);
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	// Base address...
	vec_t* Base();
	vec_t const* Base() const;

	// Initialization methods
	void Random(vec_t minVal, vec_t maxVal);
	void Zero(); ///< zero out a vector

				 // equality
	bool operator==(const VectorDrop& v) const;
	bool operator!=(const VectorDrop& v) const;

	// arithmetic operations
	VectorDrop& operator+=(const VectorDrop& v)
	{
		x += v.x; y += v.y;
		return *this;
	}

	VectorDrop& operator-=(const VectorDrop& v)
	{
		x -= v.x; y -= v.y;
		return *this;
	}

	VectorDrop& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		return *this;
	}

	VectorDrop& operator*=(const VectorDrop& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	VectorDrop& operator/=(const VectorDrop& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	// this ought to be an opcode.
	VectorDrop& operator+=(float fl)
	{
		x += fl;
		y += fl;
		return *this;
	}

	// this ought to be an opcode.
	VectorDrop& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		return *this;
	}
	VectorDrop& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		return *this;
	}

	// negate the vector components
	void Negate();

	// Get the vector's magnitude.
	vec_t Length() const;
	vec_t LengthDrop() const;

	// Get the vector's magnitude squared.
	vec_t LengthSqr(void) const
	{
		return (x*x + y * y);
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance);
	}

	vec_t NormalizeInPlace();

	VectorDrop NormalizedDrop() const {
		VectorDrop reses = *this;
		float l = reses.LengthDrop();
		if (l != 0.0f) {
			reses /= l;
		}
		else {
			reses.x = reses.y = reses.z = 0.0f;
		}
		return reses;
	}

	VectorDrop Normalized() const
	{
		return *this / Length();
	}

	bool IsLengthGreaterThan(float val) const;
	bool IsLengthLessThan(float val) const;

	// check if a vector is within the box defined by two other vectors
	bool WithinAABox(VectorDrop const &boxmin, VectorDrop const &boxmax);

	// Get the distance from this vector to the other one.
	//vec_t DistTo(const Vector2D &vOther) const 
	//{
	//	Vector delta;

	//	delta.x = x - vOther.x;
	//	delta.y = y - vOther.y;
	//	delta.z = z - vOther.z;

	//	return delta.Length();
	//}

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .
	// may be able to tidy this up after switching to VC7
	vec_t DistToDrop(const VectorDrop&vOther) const
	{
		VectorDrop deltar;

		deltar.x = x - vOther.x;
		deltar.y = y - vOther.y;

		return deltar.LengthDrop();
	}

	vec_t DistToSqr(const VectorDrop&vOther) const
	{
		VectorDrop delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;

		return delta.LengthSqr();
	}

	// Copy
	void CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void MulAdd(const VectorDrop& a, const VectorDrop& b, float scalar);

	// Dot product.
	vec_t Dot(const VectorDrop& vOther) const;

	// assignment
	VectorDrop& operator=(const VectorDrop&vOther);

	// 2d
	vec_t Length2D(void) const;
	vec_t Length2DSqr(void) const;

	/// Get the component of this vector parallel to some other given vector
	VectorDrop  ProjectOnto(const VectorDrop& onto);

	// copy constructors
	// Vector2D(const Vector2D &vOther);

	// arithmetic operations
	VectorDrop operator-(void) const;

	VectorDrop operator+(const VectorDrop& v) const;
	VectorDrop operator-(const VectorDrop& v) const;
	VectorDrop operator*(const VectorDrop& v) const;
	VectorDrop operator/(const VectorDrop& v) const;
	VectorDrop operator*(float fl) const;
	VectorDrop operator/(float fl) const;

	// Cross product between two vectors.
	VectorDrop Cross(const VectorDrop&vOther) const;

	// Returns a vector with the min or max in X, Y, and Z.
	VectorDrop Min(const VectorDrop&vOther) const;
	VectorDrop Max(const VectorDrop&vOther) const;
};