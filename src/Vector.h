#ifndef __vector_h
#define __vector_h

#include <iostream>

#include <string>

#include <math.h>

#include "math.h"

using namespace std;

typedef float (*V_FCT_PTR)(float);

class Vector {
	
	public:

	
	Vector(); 							// construtor
	Vector(float x,float y, float z);	// construtor
	Vector(const float d);				// construtor
	Vector(const Vector& v); 			// construtor
	
	~Vector(); 							// destrutor

	float value[3];
		
	Vector& operator = (const Vector& v);  	// assignment of a Vector
	Vector& operator += (const Vector& v); 	// incrementation by a Vector
	Vector& operator -= (const Vector& v); 	// decrementation by a Vector
	Vector& operator *= (const float f);	// multiplication by a constant
	Vector& operator /= (const float f); 	// division by a constant
	float& operator [] (int i);				// indexing

	float length();							// length of a Vector
	float length2();						// squared length of a Vector
	Vector& normalize();					// normalize a Vector
	Vector& apply(V_FCT_PTR fct);			// apply a func. to each component

	friend Vector operator - (const Vector& v);						// -v1
	friend Vector operator + (const Vector& v1, const Vector& v2);	// v1 + v2
	friend Vector operator - (const Vector& v1, const Vector& v2);	// v1 - v2
	friend Vector operator * (const Vector& v1, const float f);		// v1 * 2.0
	friend Vector operator * (const float f, const Vector& v2);		// 2.0 + v1
	friend float operator * (const Vector& v1, const Vector& v2);	// dot product
	friend Vector operator / (const Vector& v1, const float f);		// v1 / 2.0
	friend Vector operator ^ (const Vector& v1, const Vector& v2);	// Cross Product   
	friend int operator == (const Vector& v1, const Vector& v2);	// v1 == v2 ?
	friend int operator != (const Vector& v1, const Vector& v2);	// v1 != v2 ?
	friend void swap(Vector& v1, Vector& v2);						// swap v1 & v2
	friend Vector min(const Vector& v1, const Vector& v2);			// min(v1, v2)
	friend Vector max(const Vector& v1, const Vector& v2);			// max(v1, v2)
	friend Vector prod(const Vector& v1, const Vector& v2);			// term by term *

	friend ostream& operator << (ostream& s, Vector& v); // output to stream
	
	void SetVector(float Vctr[3]);
	Vector& GetValue(void);
	float GetValue(const int i);

};

#endif
