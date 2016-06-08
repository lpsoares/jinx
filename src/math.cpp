#include "math.h"

#include <iostream>

using namespace std;

void crossprod(float p1[3],float p2[3],float p3[3]) {
	p3[0] = p1[1]*p2[2] - p1[2]*p2[1];
	p3[1] = p1[2]*p2[0] - p1[0]*p2[2];
	p3[2] = p1[0]*p2[1] - p1[1]*p2[0];
}

void normalize(float p[3]) {
	
	double length;
		
	length = sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]);
	if (length != 0) {
		p[0] /= length;
		p[1] /= length;
		p[2] /= length;
	} 
	else {
		p[0] = 0;
		p[1] = 0;
		p[2] = 0;
	}
}

void crossprod(double p1[3],double p2[3],double p3[3]) {
	p3[0] = p1[1]*p2[2] - p1[2]*p2[1];
	p3[1] = p1[2]*p2[0] - p1[0]*p2[2];
	p3[2] = p1[0]*p2[1] - p1[1]*p2[0];
}

void normalize(double p[3]) {
	
	double length;
		
	length = sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]);
	if (length != 0) {
		p[0] /= length;
		p[1] /= length;
		p[2] /= length;
	} 
	else {
		p[0] = 0;
		p[1] = 0;
		p[2] = 0;
	}
}


void multipleMatrix(float M[3][3],float vec[3]) {
	
	float copy[3];
	
	for(int f=0; f<3; f++) copy[f]=vec[f];
	for(int g=0; g<3; g++) vec[g]=0;
	
	for(int h=0; h<3; h++) {
		for(int i=0; i<3; i++) {
			vec[h] += M[i][h]*copy[i];
		}
	}
}

// Creates the 4x4 identity matrix 
void midentity(float m[4][4]) {
	m[0][0]=1;         m[1][0]=0;         m[2][0]=0;         m[3][0]=0;
	m[0][1]=0;         m[1][1]=1;         m[2][1]=0;         m[3][1]=0;
	m[0][2]=0;         m[1][2]=0;         m[2][2]=1;         m[3][2]=0;
	m[0][3]=0;         m[1][3]=0;         m[2][3]=0;         m[3][3]=1;
}

// Creates the 3x3 identity matrix 
void midentity(float m[3][3]) {
	m[0][0]=1;         m[1][0]=0;         m[2][0]=0;
	m[0][1]=0;         m[1][1]=1;         m[2][1]=0;
	m[0][2]=0;         m[1][2]=0;         m[2][2]=1;
}

// Creates the transform matrix 
void transmatrix(float m[4][4], float x, float y, float z) {
	m[0][0]=1;         m[1][0]=0;         m[2][0]=0;         m[3][0]=x;
	m[0][1]=0;         m[1][1]=1;         m[2][1]=0;         m[3][1]=y;
	m[0][2]=0;         m[1][2]=0;         m[2][2]=1;         m[3][2]=z;
	m[0][3]=0;         m[1][3]=0;         m[2][3]=0;         m[3][3]=1;
}

// Creates the scale matrix 
void scalematrix(float m[4][4], float x, float y, float z) {
	m[0][0]=x;         m[1][0]=0;         m[2][0]=0;         m[3][0]=0;
	m[0][1]=0;         m[1][1]=y;         m[2][1]=0;         m[3][1]=0;
	m[0][2]=0;         m[1][2]=0;         m[2][2]=z;         m[3][2]=0;
	m[0][3]=0;         m[1][3]=0;         m[2][3]=0;         m[3][3]=1;
}

// Creates the rotate matrix in any vector(using opengl specification)
void rotmatrix(float m[4][4],float ang, float x, float y, float z) {
	
	float u[3];
	float mod;
	float s[3][3];
	float sina;
	float t1[3][3];  // u*uT
	float t2[3][3]; // identity
	float t3[3][3];
	float cosa;
	
	if( (x==0) && (y==0) && (z==0) ){
		midentity(m);
		return;
	}
	
	midentity(t2);
	
	mod = sqrt( (x*x) + (y*y) + (z*z) );
	
	u[0]=x/mod;	u[1]=y/mod;	u[2]=z/mod;
	
	//cout << "U0" << u[0] << "  " << u[1] << "  " << u[2] << endl;
	
	t1[0][0]=u[0]*u[0];	t1[0][1]=u[1]*u[0];	t1[0][2]=u[2]*u[0];
	t1[1][0]=u[0]*u[1];	t1[1][1]=u[1]*u[1];	t1[1][2]=u[2]*u[1];
	t1[2][0]=u[0]*u[2];	t1[2][1]=u[1]*u[2];	t1[2][2]=u[2]*u[2];
	
	cosa = cos((PI*ang)/180);
	
	t3[0][0]=cosa*(t2[0][0]-t1[0][0]); t3[0][1]=cosa*(t2[0][1]-t1[0][1]); t3[0][2]=cosa*(t2[0][2]-t1[0][2]);
	t3[1][0]=cosa*(t2[1][0]-t1[1][0]); t3[1][1]=cosa*(t2[1][1]-t1[1][1]); t3[1][2]=cosa*(t2[1][2]-t1[1][2]);
	t3[2][0]=cosa*(t2[2][0]-t1[2][0]); t3[2][1]=cosa*(t2[2][1]-t1[2][1]); t3[2][2]=cosa*(t2[2][2]-t1[2][2]);

	sina = sin((PI*ang)/180);	
	
	s[0][0]=0;     			s[0][1]=sina*(u[2]);	s[0][2]=sina*(-u[1]);
	s[1][0]=sina*(-u[2]); 	s[1][1]=0;     			s[1][2]=sina*(u[0]);  
	s[2][0]=sina*(u[1]);	s[2][1]=sina*(-u[0]);	s[2][2]=0;
	
	m[0][0]= t1[0][0] + t3[0][0] + s[0][0]; m[0][1]= t1[0][1] + t3[0][1] + s[0][1]; m[0][2]= t1[0][2] + t3[0][2] + s[0][2]; m[0][3]=0;
	m[1][0]= t1[1][0] + t3[1][0] + s[1][0]; m[1][1]= t1[1][1] + t3[1][1] + s[1][1]; m[1][2]= t1[1][2] + t3[1][2] + s[1][2]; m[1][3]=0;
	m[2][0]= t1[2][0] + t3[2][0] + s[2][0]; m[2][1]= t1[2][1] + t3[2][1] + s[2][1]; m[2][2]= t1[2][2] + t3[2][2] + s[2][2]; m[2][3]=0;
	m[3][0]=0;  m[3][1]=0;  m[3][2]=0;  m[3][3]=1;
	
}

// Creates the rotate matrix in x
void rotxmatrix(float m[4][4],float ang) {
	
	ang=((PI*ang)/180);
	
	m[0][0]=1;         m[1][0]=0;         m[2][0]=0;         m[3][0]=0;
	m[0][1]=0;         m[1][1]=cos(ang);  m[2][1]=-sin(ang); m[3][1]=0;
	m[0][2]=0;         m[1][2]=sin(ang);  m[2][2]=cos(ang);  m[3][2]=0;
	m[0][3]=0;         m[1][3]=0;         m[2][3]=0;         m[3][3]=1;
}

// Creates the rotate matrix in y
void rotymatrix(float m[4][4],float ang) {
	
	ang=((PI*ang)/180);
	
	m[0][0]=cos(ang);  m[1][0]=0;         m[2][0]=sin(ang);  m[3][0]=0;
	m[0][1]=0;         m[1][1]=1;         m[2][1]=0;         m[3][1]=0;
	m[0][2]=-sin(ang); m[1][2]=0;         m[2][2]=cos(ang);  m[3][2]=0;
	m[0][3]=0;         m[1][3]=0;         m[2][3]=0;         m[3][3]=1;
}

// Creates the rotate matrix in z
void rotzmatrix(float m[4][4],float ang) {
	
	ang=((PI*ang)/180);
	
	m[0][0]=cos(ang);  m[1][0]=-sin(ang); m[2][0]=0;         m[3][0]=0;
	m[0][1]=sin(ang);  m[1][1]=cos(ang);  m[2][1]=0;         m[3][1]=0;
	m[0][2]=0;         m[1][2]=0;         m[2][2]=1;         m[3][2]=0;
	m[0][3]=0;         m[1][3]=0;         m[2][3]=0;         m[3][3]=1;
}

// Multiply matrices
void multipleMatrix(float M1[4][4],float M2[4][4]) {
	float copy[4][4];
	
	int f,g;
	
	for(f=0; f<4; f++) {
		for(g=0; g<4; g++) {
			copy[f][g]=M2[f][g];
		}
	}
	
	
	for(f=0; f<4; f++) {
		for(g=0; g<4; g++) {
			M2[f][g]=0;
		}
	}
		
	for(int h=0; h<4; h++) {
		for(int i=0; i<4; i++) {
			for(f=0; f<4; f++) {
				M2[i][h] += M1[f][h]*copy[i][f];
			}
		}
	}
}

void multipleMatrix(float M[4][4],float vec[4]) {
	
	float copy[4];
	
	for(int f=0; f<4; f++) copy[f]=vec[f];
	for(int g=0; g<4; g++) vec[g]=0;
	
	for(int h=0; h<4; h++) {
		for(int i=0; i<4; i++) {
			vec[h] += M[i][h]*copy[i];
		}
	}
}


void rotate(double angle, float up[3], float vec[3]) {
	double s;
	float v[3];
	float M[3][3];
	
	s = cos( (angle*PI/180) / 2 );
	
	v[0]=up[0]*sin( (angle*PI/180) / 2 );
	v[1]=up[1]*sin( (angle*PI/180) / 2 );
	v[2]=up[2]*sin( (angle*PI/180) / 2 );
	
	M[0][0] = ( 1 ) - ( 2*v[1]*v[1] ) - ( 2*v[2]*v[2] );
	M[0][1] = ( 2*v[0]*v[1] ) - ( 2*s*v[2] );
	M[0][2] = ( 2*v[0]*v[2] ) + ( 2*s*v[1] );
	M[1][0] = ( 2*v[0]*v[1] ) + ( 2*s*v[2] );
	M[1][1] = ( 1 ) - ( 2*v[0]*v[0] ) - ( 2*v[2]*v[2] );
	M[1][2] = ( 2*v[1]*v[2] ) - ( 2*s*v[0] );
	M[2][0] = ( 2*v[0]*v[2] ) - ( 2*s*v[1] );
	M[2][1] = ( 2*v[1]*v[2] ) + ( 2*s*v[0] );
	M[2][2] = ( 1 ) - ( 2*v[0]*v[0] ) - ( 2*v[1]*v[1] );
	
	multipleMatrix(M,vec);
	
}

//http://skal.planet-d.net/demo/matrixfaq.htm#Q24

void m4_submat( float mr[16], float mb[9], int i, int j ) {
      int ti, tj, idst, jdst;

      for ( ti = 0; ti < 4; ti++ ) {
        if ( ti < i )
          idst = ti;
        else
          if ( ti > i )
            idst = ti-1;

        for ( tj = 0; tj < 4; tj++ )
          {
          if ( tj < j )
            jdst = tj;
          else
            if ( tj > j )
              jdst = tj-1;

          if ( ti != i && tj != j )
            mb[idst*3 + jdst] = mr[ti*4 + tj ];
          }
        }
}

    
float m3_det( float mat[9] ) {
      float det;

      det = mat[0] * ( mat[4]*mat[8] - mat[7]*mat[5] )
          - mat[1] * ( mat[3]*mat[8] - mat[6]*mat[5] )
          + mat[2] * ( mat[3]*mat[7] - mat[6]*mat[4] );

      return( det );
}

float m4_det( float mr[16] ) {
      float  det, result = 0, i = 1;
      float msub3[9];
      int     n;

      for ( n = 0; n < 4; n++, i *= -1 )
        {
        m4_submat( mr, msub3, 0, n );

        det     = m3_det( msub3 );
        result += mr[n] * det * i;
        }

      return( result );
}

int m4_inverse( float mr[16], float ma[16] ) {
      float  mdet = m4_det( ma );
      float mtemp[9];
      int     i, j, sign;

      if ( fabs( mdet ) < 0.0005 )
        return( 0 );

      for ( i = 0; i < 4; i++ )
        for ( j = 0; j < 4; j++ )
          {
          sign = 1 - ( (i +j) % 2 ) * 2;

          m4_submat( ma, mtemp, i, j );

          mr[i+j*4] = ( m3_det( mtemp ) * sign ) / mdet;
          }

      return( 1 );
}

void mcopy(float m1[4][4],float m2[4][4]) {
	
	m2[0][0]=m1[0][0];m2[0][1]=m1[0][1];m2[0][2]=m1[0][2];m2[0][3]=m1[0][3];
	m2[1][0]=m1[1][0];m2[1][1]=m1[1][1];m2[1][2]=m1[1][2];m2[1][3]=m1[1][3];
	m2[2][0]=m1[2][0];m2[2][1]=m1[2][1];m2[2][2]=m1[2][2];m2[2][3]=m1[2][3];
	m2[3][0]=m1[3][0];m2[3][1]=m1[3][1];m2[3][2]=m1[3][2];m2[3][3]=m1[3][3];
	
}



