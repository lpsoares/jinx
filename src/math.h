
#ifndef __math_h
#define __math_h

#include <math.h>

#define PI 3.141592653589793238462643f

#define SMALL 0.01

typedef struct {
	double x,y,z;
} XYZ;

void crossprod(float p1[3],float p2[3],float p3[3]); 
void crossprod(double p1[3],double p2[3],double p3[3]); 

int m4_inverse( float mr[16], float ma[16] );
float m3_det( float mat[9] );
float m4_det( float mr[16] );
void m4_submat( float mr[16], float mb[9], int i, int j );

void normalize(float p[3]);
void normalize(double p[3]);

void multipleMatrix(float M[3][3],float vec[3]);
void multipleMatrix(float M[4][4],float vec[4]);
void multipleMatrix(float M1[4][4],float M2[4][4]);

void scalematrix(float m[4][4], float x, float y, float z);
void transmatrix(float m[4][4], float x, float y, float z);

void rotmatrix(float m[4][4],float ang, float x, float y, float z);
void rotxmatrix(float m[4][4],float ang);
void rotymatrix(float m[4][4],float ang);
void rotzmatrix(float m[4][4],float ang);

void rotate(double angle, float up[3], float vec[3]);

void midentity(float m[4][4]);
void midentity(float m[3][3]);
void mcopy(float m1[4][4],float m2[4][4]);

#endif
