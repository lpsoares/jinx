/*	Floating point Vector 3D class

	Last Modified	6/4/00
*/

#ifndef __VECTOR3D_H_
#define __VECTOR3D_H_

////////////////////////////

class Point3D;

class	Vector3D
{
private:
	float	x,y,z;

public:

	Vector3D();									// constructs a zero vector
	Vector3D(float x, float y, float z);

	Vector3D(const Point3D &p, const Point3D &q);	// create vector PQ

//	~Vector3D();

///////////

	void	Round(void);														// round values to within decimal places
	void	Zero_Clamp(void);													// clamp values to 0

	int		IsUnit(void)											const;		// test if this vector is a unit vector
	int		IsZero(void)											const;		// test if this vector is zero
	int		IsParallel(const Vector3D &v)							const;		// true if two vectors are parallel, Vectors CANNOT be zero

	float	GetX()													const;		// returns the value of individual components of this vector
	float	GetY()													const;
	float	GetZ()													const;

	void	GetValues(float &x, float &y, float &z)					const;		// get the values of this vector
	void	SetValues(const float x, const float y, const float z);				// set the values of this vector
	void	CreateVector(const Point3D &p, const Point3D &q);					// create a vector PQ

///////////

	double	DotProduct(const Vector3D &v)							const;		// returns dot product (cos theta) for UNIT VECTORS only. Vectors CANNOT be zero. returns -1 to 1
	void	CrossProduct(const Vector3D &a, const Vector3D &b);					// NORMALIZED cross product and store result in this vector, FOR UNIT VECTORS ONLY. May return (0,0,0) meaning vectors are parallel
	double	GetAngleDeg(const Vector3D &v)							const;		// get angle in degrees between the UNIT VECTORS. returns 0 - 180
	double	GetAngleRad(const Vector3D &v)							const;		// get angle in rads between the UNIT VECTORS.

	double	Length(void)											const;		// length of this vector

///////////

	double	Normalize(void);		// normalizes the vector and return the length of the vector
	void	Scale(float s);			// scale the vector
	void	Negate(void);			// vector go in opposite direction

////////////

	// opengl
	void	glTranslate(void)					const;		// translate the current opengl matrix with this vector
	void	glRotate(const float angle)			const;		// rotate angle around axis specified by this vector in deg

////////////

	// operators
	Vector3D	operator*(const Vector3D &v)	const;		// Unit cross product, for UNIT VECTORS ONLY. May return 0,0,0 meaning vectors are parallel

// dangerous - remove for now
//	Vector3D	operator*(const float s)		const;		// scale vector by s

	Vector3D	operator+(const Vector3D &v)	const;
	Vector3D	operator-(const Vector3D &v)	const;
	Vector3D	operator-()						const;		// negation

	Vector3D&	operator=(const Vector3D &v);				// copy constructor
	Vector3D&	operator+=(const Vector3D &v);
	Vector3D&	operator-=(const Vector3D &v);
	Vector3D&	operator*=(const float s);					// scale vector by s

	int			operator==(const Vector3D &v)	const;		// equality operator
	int			operator!=(const Vector3D &v)	const;

// debug
	void Print();
};	// end class Vector3D

////////////////////////////

#endif	// __VECTOR3D_H_
