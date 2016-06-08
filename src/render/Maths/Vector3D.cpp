/*	Vector 3D class

	Last Modified	6/4/00
*/

#ifndef __VECTOR3D_CPP_
#define	__VECTOR3D_CPP_

#include <math.h>
#include <assert.h>

#include <GL/gl.h>

#include "MathsStruct.h"

#include "Point3D.h"
#include "Vector3D.h"

////////////////////////////////

Vector3D::Vector3D()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;

	return;
}	// end constructor

///////

Vector3D::Vector3D(float x, float y, float z)
{
	SetValues(x, y, z);

	return;
}	// end constructor

///////

Vector3D::Vector3D(const Point3D &p, const Point3D &q)
{
	CreateVector(p, q);

	return;
}	// end constructor

///////

void Vector3D::Round(void)
{
	x	= ROUND(x, FLOAT_DECIMAL_TOLERANCE);			// clamp the vector
	y	= ROUND(y, FLOAT_DECIMAL_TOLERANCE);
	z	= ROUND(z, FLOAT_DECIMAL_TOLERANCE);

	return;
}	// end void Round()

////////

void Vector3D::Zero_Clamp(void)
{
	x	= ZERO_CLAMP(x);
	y	= ZERO_CLAMP(y);
	z	= ZERO_CLAMP(z);

}	// end void Zero_Clamp()

///////

int Vector3D::IsUnit(void) const
{
	return( FLOAT_EQ(1.0f, Length() ) );
}	// end int IsUnit()

///////

int Vector3D::IsZero(void) const
{
	return( FLOAT_EQ(0.0f, Length()) );
}	// end int IsZero()

////////

int Vector3D::IsParallel(const Vector3D &v) const
{
	assert(!IsZero() && !v.IsZero());		// both not zero vectors

	Vector3D v1, v2;			// temp vectors
	double angle;				// temp angle

	v1.SetValues(x, y, z);		// copy to temp vectors
	v2 = v;

	v1.Normalize();				// normalize both of them
	v2.Normalize();

	angle = v1.DotProduct(v2);	// get value between them

	return(FLOAT_EQ(-1.0f, angle) || FLOAT_EQ(1.0f, angle));		// if cos angle is -1 or 1, the vectors are parallel
}	// end int IsParallel(..)

///////

float Vector3D::GetX() const
{
	return(x);
}	// end float GetX()

///////

float Vector3D::GetY() const
{
	return(y);
}	// end float GetY()

///////

float Vector3D::GetZ() const
{
	return(z);
}	// end float GetZ()

///////

void Vector3D::GetValues(float &x, float &y, float &z) const
{
	x = this->x;
	y = this->y;
	z = this->z;

	return;
}	// end void GetValues(..)

/////////

void Vector3D::SetValues(const float x, const float y, const float z)
{
	this->x = x;
	this->y = y;
	this->z = z;

	return;
}	// end void SetValues(..)

////////

void Vector3D::CreateVector(const Point3D &p, const Point3D &q)
{
	float	x1, y1, z1,
			x2, y2, z2;			// temp vars

	p.GetValues(x1, y1, z1);	// get values from points since no friends allowed
	q.GetValues(x2, y2, z2);

	x = x2 - x1;
	y = y2 - y1;
	z = z2 - z1;

	return;
}	// end void CreateVector(..)

////////

double Vector3D::DotProduct(const Vector3D &v) const
{
	assert(IsUnit() && v.IsUnit());			// defined only for unit vectors

	float dot = x*v.x + y*v.y + z*v.z;

	LIMIT_RANGE(-1.0f, dot, 1.0f);	// dot product must be between -1 and 1

	return(dot);
}	// end double DotProduct(..)

//////

void Vector3D::CrossProduct(const Vector3D &a, const Vector3D &b)
{
	assert(a.IsUnit() && b.IsUnit());		// both are unit vectors

	x = a.y*b.z - a.z*b.y;
	y = a.z*b.x - a.x*b.z;
	z = a.x*b.y - a.y*b.x;

	Normalize();

	return;
}	// end void CrossProduct(..)

/////

double Vector3D::GetAngleDeg(const Vector3D &v) const
{
	assert(!IsZero() && !v.IsZero());		// only defined for unit vectors
	assert(IsUnit() && v.IsUnit());

	double angle;

	angle = (float)RADTODEG(acos(DotProduct(v)));	// get angle between them and convert to degrees

	LIMIT_RANGE(0.0f, angle, 180.f);		// angle must be between 0 - 180

	return(angle);

}	// end double GetAngleDeg(..)

///////

double Vector3D::GetAngleRad(const Vector3D &v) const
{
	assert(!IsZero() && !v.IsZero());		// only defined for unit vectors
	assert(IsUnit() && v.IsUnit());

	double rad_angle;

	rad_angle = (float)acos(DotProduct(v));

	LIMIT_RANGE(0.0f, rad_angle, PI_AQUI);		// angle must be between 0 and PI

	return(rad_angle);
}	// end double GetAngleRad(..)

///////

double Vector3D::Length(void) const
{
	return( sqrt(SQR(x) + SQR(y) + SQR(z))  );
}	// end double Length()

//////

double Vector3D::Normalize(void)
{
	double len = Length();

	if (FLOAT_EQ(0.0f,len))	// if length is zero
	{
		x =	0.0f;
		y =	0.0f;
		z =	0.0f;
	}
	else		// normalize
	{
		x = (float)(x / len);
		y = (float)(y / len);
		z = (float)(z / len);

		Zero_Clamp();
	}

	return(len);
}	// end double Normalize()

////////

void Vector3D::Scale(float s)
{
	x *= s;
	y *= s;
	z *= s;

	return;
}	// end void Scale(..)

////////

void Vector3D::Negate(void)
{
	x = -(x);
	y = -(y);
	z = -(z);

	return;
}	// end void Negate()

////////

void Vector3D::glTranslate(void) const
{
	glTranslatef(x, y, z);

	return;
}	// end void glTranslate()

////////

void Vector3D::glRotate(const float angle) const
{
	glRotatef(angle, x, y, z);

	return;
}	// end void glRotate(..)

////////

Vector3D Vector3D::operator*(const Vector3D &v) const
{
	assert(IsUnit() && v.IsUnit());			// both are unit vectors

/*	Leave checking up to user
	assert(*this != v);						// vector are different

	Vector3D temp = v;						// test the vectors are not parallel
	temp.Negate();
	assert(*this != temp);
*/
	Vector3D	temp;		// temp vector

	temp.CrossProduct(*this, v);

	temp.Normalize();

	return(temp);
}	// end operator *

///////////
/* dangerous - remove for now
Vector3D Vector3D::operator*(const float s) const
{
	float x, y, z;

	x = (*this).x * s;
	y = (*this).y * s;
	z = (*this).z * s;

	return(Vector3D(x, y, z));
}	// end operator *
*/
////////////

Vector3D Vector3D::operator+(const Vector3D &v) const
{
	float x,y,z;

	x = this->x + v.x;
	y = this->y + v.y;
	z = this->z + v.z;

	return(Vector3D(x, y, z));
}	// end operator +

///////

Vector3D Vector3D::operator-(const Vector3D &v) const
{
	float x, y, z;

	x = this->x - v.x;
	y = this->y - v.y;
	z = this->z - v.z;

	return(Vector3D(x, y, z));
}	// end operator -

/////////

Vector3D Vector3D::operator-() const
{
	return(Vector3D(-x, -y, -z));
}	// end operator -

///////

Vector3D& Vector3D::operator=(const Vector3D &v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return(*this);
}	// end operator = 

////////

Vector3D& Vector3D::operator*=(const float s)
{
	x *= s;
	y *= s;
	z *= s;

	return(*this);
}	// end void operator *=

//////

Vector3D& Vector3D::operator+=(const Vector3D &v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return(*this);
}	// end void operator +=

///////

Vector3D& Vector3D::operator-=(const Vector3D &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return(*this);
}	// end operator -=

////////

int Vector3D::operator==(const Vector3D &v) const
{
	return( FLOAT_EQ(x, v.x) && FLOAT_EQ(y, v.y) && FLOAT_EQ(z, v.z) );
}	// end operator ==

////////

int Vector3D::operator!=(const Vector3D &v) const
{
	return( !(*this == v) );
}	// end operator !=

// ----------------------------------------- Debug

#include <iostream.h>

void Vector3D::Print()
{
	cout << x << " " << y << " " << z << endl;
}

//////////////////////////////////

#endif	// __VECTOR3D_CPP_
