
#include "Vector.h"

Vector::Vector() {    
   this->value[0] = 0.0;
   this->value[1] = 0.0; 
   this->value[2] = 0.0;
}

Vector::Vector(const float x, const float y, const float z) {
   this->value[0] = x;
   this->value[1] = y;
   this->value[2] = z;
}

Vector::Vector(const float d) {
   this->value[0] = d;
   this->value[1] = d;
   this->value[2] = d;
}

Vector::Vector(const Vector& v) {
   this->value[0] = v.value[0];
   this->value[1] = v.value[1];
   this->value[2] = v.value[2];
}

Vector::~Vector() {
}

Vector& Vector::operator = (const Vector& v) {
this->value[0] = v.value[0];
this->value[1] = v.value[1];
this->value[2] = v.value[2];
  return *this;
}

Vector& Vector::operator += (const Vector& v) {
  value[0] += v.value[0];
  value[1] += v.value[1];
  value[2] += v.value[2];
  return *this;
} 
  
Vector& Vector::operator -= (const Vector& v) {
  value[0] -= v.value[0];
  value[1] -= v.value[1];
  value[2] -= v.value[2];
  return *this;
}

Vector& Vector::operator *= (const float f) {
  value[0] *= f;
  value[1] *= f;
  value[2] *= f;
  return *this;
}

Vector& Vector::operator /= (const float f) {
	float d;
	
	d = 1/f;
	value[0] *= d;
	value[1] *= d;
	value[2] *= d;
	return *this;
}

float& Vector::operator [] (int i) {
  if( i < 0 || i > 2) {
    printf("Vector [] operator: illegal access;\n");
    exit(0);
  }
  return value[i];
}

// Special Functions

float Vector::length() {

	return (float)sqrt(length2());
	
}

float Vector::length2() {
  return value[0]*value[0] + value[1]*value[1] + value[2]*value[2];
}

Vector& Vector::normalize() {
  *this /= length();
  return *this;
}

Vector& Vector::apply(V_FCT_PTR fct) {
  value[0] = (*fct)(value[0]);
  value[1] = (*fct)(value[1]);
  value[2] = (*fct)(value[2]);  
  return *this;
}

// Friends

Vector operator - (const Vector& v) {
  return Vector(-v.value[0],-v.value[1],-v.value[2]);
}

Vector operator + (const Vector& v1, const Vector& v2) {
  return Vector(v1.value[0] + v2.value[0],
		v1.value[1] + v2.value[1],
		v1.value[2] + v2.value[2]);
}

Vector operator - (const Vector& v1, const Vector& v2) {
  return Vector(v1.value[0] - v2.value[0],
		v1.value[1] - v2.value[1],
		v1.value[2] - v2.value[2]);
}

Vector operator * (const Vector& v1, const float f) {
  return Vector(v1.value[0] * f,
		v1.value[1] * f,
		v1.value[2] * f);
}

Vector operator * (const float f, const Vector& v2) {
  return v2*f;
}

float operator * (const Vector& v1, const Vector& v2) {
  return (v1.value[0] * v2.value[0] +
	  v1.value[1] * v2.value[1] +
	  v1.value[2] * v2.value[2]);
}

Vector operator / (const Vector& v1, const float f) {
  double d = 1.0/f;
  return Vector(v1.value[0] * d,
		v1.value[1] * d,
		v1.value[2] * d);
}

Vector operator ^ (const Vector& v1, const Vector& v2) {
  return Vector(v1.value[1] * v2.value[2] - v1.value[2] * v2.value[1],
		v1.value[2] * v2.value[0] - v1.value[0] * v2.value[2],
		v1.value[0] * v2.value[1] - v1.value[1] * v2.value[0]);
}

int operator == (const Vector& v1, const Vector& v2) {
  return (v1.value[0] == v2.value[0]) && (v1.value[1] ==v2.value[1])
  && (v1.value[1] ==v2.value[1]);
}

int operator != (const Vector& v1, const Vector& v2) {
  return !( v1 == v2);
}

ostream& operator << (ostream& s, Vector& v) {
  return s << "| " << v.value[0] << ' ' << v.value[1] << ' ' <<  v.value[2] << " |";
}

void swap(Vector& v1, Vector& v2) {
  Vector aux(v1);
  v1 = v2;
  v2 = aux;
}

Vector min(const Vector& v1, const Vector& v2) {
  return Vector(((v1.value[0]<v2.value[0])?v1.value[0]:v2.value[0]),
				((v1.value[1]<v2.value[1])?v1.value[1]:v2.value[1]),
				((v1.value[2]<v2.value[2])?v1.value[2]:v2.value[2])
		 );
}

Vector max(const Vector& v1, const Vector& v2) {
  return Vector(((v1.value[0]>v2.value[0])?v1.value[0]:v2.value[0]),
				((v1.value[1]>v2.value[1])?v1.value[1]:v2.value[1]),
				((v1.value[2]>v2.value[2])?v1.value[2]:v2.value[2])
		 );
}

Vector prod(const Vector& v1, const Vector& v2) {
  return Vector(v1.value[0] * v2.value[0],v1.value[1] * v2.value[1],
		v1.value[2] * v2.value[2]);
}

void Vector::SetVector(float V[3]) {
  value[0] = V[0];
  value[1] = V[1];
  value[2] = V[2];
}

Vector& Vector::GetValue(void) {
  return(*this);
}

float Vector::GetValue(const int i) {
  return(value[i]);
}
