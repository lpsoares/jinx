/* Lucas Peetz Dulley @ 2004/01/12
 */
 
 /*
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define WIDTH 800 
#define HEIGHT 800
#define DELTATIME 20

int height, width, aaa=0,bbb=0,ccc=0;
*/

/* SP_CODE BEGIN */

#define SP_PI 3.141592653589793238462643
#define RES 19 

int gera_pontos(void);

typedef struct {
   double x,y,z;
}sp_point;

typedef struct {
   double x,y;
}vertex2f;

typedef struct {
   unsigned char r,g,b,a;
} PIXELA;

typedef struct {
   sp_point vp;              /* View position           */
   sp_point vd;              /* View direction vector   */
   sp_point vu;              /* View up direction       */
   sp_point pr;              /* Point to rotate about   */
   double focallength;  /* Focal Length along vd   */
   sp_point focus;           /* Focal point             */
   double aperture;     /* camera aperture         */
   double eyesep;       /* Eye separation          */
   double left;
   double right;
   double bottom;
   double top;
   double near;
   double far;
 } sp_CAMERA;

static vertex2f F[RES][RES];
static vertex2f R[(RES+1)/2][RES];
static vertex2f L[(RES+1)/2][RES];
static vertex2f T[RES][(RES+1)/2];
static vertex2f B[RES][(RES+1)/2];

unsigned int textureid[5];

static sp_CAMERA sp_camera;

static sp_point userOffset;
static float lensAperture;
static float ec;

static int texResolution;


void sp_Normalize(sp_point *p){
	double mag;
	mag = sqrt((p->x)*(p->x)+(p->y)*(p->y)+(p->z)*(p->z));
	if (mag == 0.0)
	{
		p->x = 0.0;
		p->y = 0.0;
		p->z = 0.0;
	}
	else
	{
		p->x /= mag;
		p->y /= mag;
		p->z /= mag;
	}
};

void sp_Set_Offsets(double xOff, double yOff, double zOff)
{
	sp_point q;
	q.x=xOff;q.y=yOff;q.z=zOff;
	if (sqrt((q.x)*(q.x)+(q.y)*(q.y)+(q.z)*(q.z)) > 1.0)
		sp_Normalize(&q);

	userOffset.x=q.x;
	userOffset.y=q.y;
	userOffset.z=q.z;
	gera_pontos();
}

void sp_Set_Aperture_EC(float aperture, float ecent)
{
	lensAperture=aperture;
	ec=ecent;
	gera_pontos();
}


void sp_CalcCamera(int mode)
{
	double corr=0.0;
	sp_camera.aperture = 90;
	
	sp_camera.near=0.1;
	sp_camera.far=6500;
	
	sp_camera.left   = -0.1; sp_camera.right =  0.1;
	sp_camera.bottom = -0.1; sp_camera.top   =  0.1;
	
	corr=1.0/(1.0-ec);

	switch (mode) {
	case 0: /* Look front */
		sp_camera.vp.x =  0.0; sp_camera.vp.y =  0.0; sp_camera.vp.z =  0.0;
		sp_camera.vd.x =  0.0; sp_camera.vd.y =  0.0; sp_camera.vd.z = -5.0;
		sp_camera.vu.x =  0.0; sp_camera.vu.y =  1.0; sp_camera.vu.z =  0.0;
	
		sp_camera.left   *= corr;  sp_camera.right *=  corr;
		sp_camera.bottom *= corr; sp_camera.top   *=  corr;

		break;
	case 1: /* Look left */
		sp_camera.vp.x =  0.0; sp_camera.vp.y =  0.0; sp_camera.vp.z =  0.0;
		sp_camera.vd.x = -5.0; sp_camera.vd.y =  0.0; sp_camera.vd.z =  0.0;
		sp_camera.vu.x =  0.0; sp_camera.vu.y =  1.0; sp_camera.vu.z =  0.0;

		sp_camera.left   *= 1.0+ec ; sp_camera.right *=  1.0/corr;

		break;
	case 2: /* Look right */
		sp_camera.vp.x =  0.0; sp_camera.vp.y =  0.0; sp_camera.vp.z =  0.0;
		sp_camera.vd.x =  5.0; sp_camera.vd.y =  0.0; sp_camera.vd.z =  0.0;
		sp_camera.vu.x =  0.0; sp_camera.vu.y =  1.0; sp_camera.vu.z =  0.0;
	sp_camera.left   *= 1.0/corr;; sp_camera.right *=  1.0+ec;

	break;
	case 3: /* Look top */
		sp_camera.vp.x =  0.0; sp_camera.vp.y =  0.0; sp_camera.vp.z =  0.0;
		sp_camera.vd.x =  0.0; sp_camera.vd.y =  1.0; sp_camera.vd.z =  0.0;
		sp_camera.vu.x =  0.0; sp_camera.vu.y =  0.0; sp_camera.vu.z =  1.0;

		sp_camera.bottom *= 1.0/corr; sp_camera.top   *=  1.0+ec;

	break;
	case 4: /* Look bottom */
		sp_camera.vp.x =  0.0; sp_camera.vp.y =  0.0; sp_camera.vp.z =  0.0;
		sp_camera.vd.x =  0.0; sp_camera.vd.y = -5.0; sp_camera.vd.z =  0.0;
		sp_camera.vu.x =  0.0; sp_camera.vu.y =  0.0; sp_camera.vu.z = -1.0;

		sp_camera.bottom *= 1.0+ec ; sp_camera.top   *=  1.0/corr;

	break;
	}
	
	/*
	cout << mode << endl;
	cout << sp_camera.vp.x;
	cout << sp_camera.vp.y;
	cout << sp_camera.vp.z;
	cout << sp_camera.vd.x;
	cout << sp_camera.vd.y;
	cout << sp_camera.vd.z;
	cout << sp_camera.left;
	cout << sp_camera.right;
	cout << sp_camera.bottom;
	cout << sp_camera.top;
	cout << endl;
	*/
	
}



vertex2f sp_Coord(sp_point p){
	double theta,phi,r,plinha,K,L,delta,mag;
	vertex2f v;


	
	/*Inverte o sistema de Coordenadas*/
//	p.x= -p.x;
//	p.y= -p.y;
//	p.z= -p.z;
	
	p.z=p.z+ec;

	sp_Normalize(&p);

	/*Calculo da correção da projeção para usuario fora do eixo principal*/
	K= p.x*userOffset.x + p.y*userOffset.y + p.z*userOffset.z;
	L= userOffset.x*userOffset.x + userOffset.y*userOffset.y + userOffset.z*userOffset.z -1.0;
	delta = K*K - L;	

	plinha= -K +sqrt(delta); 
	//printf(" Alpha: %.3f ", plinha);

	p.y= p.y*plinha+userOffset.y;
	p.x= p.x*plinha+userOffset.x;
	p.z= p.z*plinha+userOffset.z;

	/* Projeção Olho de Peixe fisheye*/


	phi=acos(-p.z);
	theta=atan2(p.y,p.x);

	
	r=2*phi/(SP_PI*lensAperture/180.0);
	v.x=r*cos(theta);	
	v.y=r*sin(theta);	
	//printf("Vertex: ( %.3f, %.3f )\n", v.x, v.y); 
	return v;
};

int gera_pontos(void)
{
	int i,j;
	sp_point front[RES][RES];
	sp_point right[(RES+1)/2][RES];
	sp_point left[(RES+1)/2][RES];
	sp_point top[RES][(RES+1)/2];
	sp_point bottom[RES][(RES+1)/2];
	double step;
	double mag;

	step = 2.0/ (RES-1);
	
	
	/* vertex for the front square */
	for(j=0; j < RES; j++)
		for(i=0; i < RES; i++)
		{
			front[i][j].x=-1.0+i*step;
			front[i][j].y=-1.0+j*step;
			front[i][j].z=-1.0;
			F[i][j]=sp_Coord(front[i][j]);

		}

	/* vertex for the right square */
	for(j=0; j < RES; j++)
		for(i=0; i < (RES+1)/2; i++)
		{
			right[i][j].x=1.0;
			right[i][j].y=-1.0+j*step;
			right[i][j].z=-1.0+i*step;
			R[i][j]=sp_Coord(right[i][j]);
		}

	/* vertex for the left square */
	for(j=0; j < RES; j++)
		for(i=0; i < (RES+1)/2; i++)
		{
			left[i][j].x=-1.0;
			left[i][j].y=-1.0+j*step;
			left[i][j].z=0.0-i*step;
			L[i][j]=sp_Coord(left[i][j]);
		}

	/* vertex for the top square */
	for(j=0; j < (RES+1)/2; j++)
		for(i=0; i < RES; i++)
		{
			top[i][j].x=-1.0+i*step;
			top[i][j].y=1.0;
			top[i][j].z=-1.0+j*step;
			T[i][j]=sp_Coord(top[i][j]);
		}


	/* vertex for the bottom square */
	for(j=0; j < (RES+1)/2; j++)
		for(i=0; i < RES; i++)
		{
			bottom[i][j].x=-1.0+i*step;
			bottom[i][j].y=-1.0;
			bottom[i][j].z=0.0-j*step;
			B[i][j]=sp_Coord(bottom[i][j]);
		}

	return 0;
}
	

void sp_Init(void)
{
	userOffset.x=0.0;
	userOffset.y=0.0;
	userOffset.z=0.0;
	lensAperture=180.0;
	ec=0.0;
	texResolution=512;
	//texResolution=1024;
	glGenTextures(5,textureid);
	gera_pontos();
}

void sp_CaptureBegin(int i, double eye)
{
	glViewport(0,0, texResolution, texResolution);
	sp_CalcCamera(i); /* Choose camera view */
	
	
	
	//glDrawBuffer(GL_BACK_LEFT); //temp
	glDrawBuffer(GL_AUX0);
	
	
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(sp_camera.left,sp_camera.right,sp_camera.bottom,sp_camera.top,sp_camera.near,sp_camera.far);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(	sp_camera.vp.x+eye,sp_camera.vp.y,sp_camera.vp.z,
			sp_camera.vd.x,sp_camera.vd.y,sp_camera.vd.z, 
			sp_camera.vu.x,sp_camera.vu.y,sp_camera.vu.z);

	/*	
	cout << sp_camera.vp.x;
	cout << sp_camera.vp.y;
	cout << sp_camera.vp.z;
	cout << sp_camera.vd.x;
	cout << sp_camera.vd.y;
	cout << sp_camera.vd.z;
	cout << sp_camera.left;
	cout << sp_camera.right;
	cout << sp_camera.bottom;
	cout << sp_camera.top;
	cout << endl;
	*/

	
}

void sp_CaptureEnd(int i)
{

    glBindTexture(GL_TEXTURE_2D,textureid[i]);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB, 0,0, texResolution, texResolution, 0);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

	//glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
}

void sp_draw(int sp_F_width, int sp_F_height)
{
	int i,j;
	double delta;
	
	delta= 1.0/ (RES-1); 

 	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	/* BEGIN: Draw The Fisheye View */

	glViewport(0,0, sp_F_width, sp_F_height);
	glDisable(GL_LIGHTING);
	glShadeModel(GL_FLAT);

	//glDrawBuffer(GL_BACK);
	
	//glClearColor(0.5,0.5,0.5,0.0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -0.5, 1.0,-100,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();

	glDisable(GL_TEXTURE_2D);

	// Front
	glColor3f(0.5,0.5,0.5);

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(-1,-1,-0.5);
	glVertex3d( 1,-1,-0.5);
	glVertex3d(-1, 1,-0.5);
	glVertex3d( 1, 1,-0.5);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	for(j=0; j < RES-1 ; j++)
	{
		glBindTexture(GL_TEXTURE_2D, textureid[0]);
		glBegin(GL_TRIANGLE_STRIP);
		for(i=0; i < RES-1; i++)
			{
				glTexCoord2d((i)*delta,(j+1)*delta);				
				glVertex2d(F[i][j+1].x, F[i][j+1].y);
				glTexCoord2d((i)*delta,(j)*delta);				
				glVertex2d(F[i][j].x, F[i][j].y);
				glTexCoord2d((i+1)*delta,(j+1)*delta);				
				glVertex2d(F[i+1][j+1].x, F[i+1][j+1].y);
				glTexCoord2d((i+1)*delta,(j+1)*delta);				
				glVertex2d(F[i+1][j+1].x, F[i+1][j+1].y);
				glTexCoord2d((i)*delta,(j)*delta);				
				glVertex2d(F[i][j].x, F[i][j].y);
				glTexCoord2d((i+1)*delta,(j)*delta);				
				glVertex2d(F[i+1][j].x, F[i+1][j].y);

			}
		glEnd();
		

	}

	// Left 
	glColor3f(0.0,1.0,1.0);
	for(j=0; j < RES-1; j++)
	{
		glBindTexture(GL_TEXTURE_2D, textureid[1]);
		glBegin(GL_TRIANGLE_STRIP);
		for(i=0; i < ((RES+1)/2)-1; i++)
			{
				glTexCoord2d(0.5+(i)*delta,(j+1)*delta);
				glVertex2d(L[i][j+1].x, L[i][j+1].y);
				glTexCoord2d(0.5+(i)*delta,(j)*delta);
				glVertex2d(L[i][j].x, L[i][j].y);
				glTexCoord2d(0.5+(i+1)*delta,(j+1)*delta);
				glVertex2d(L[i+1][j+1].x, L[i+1][j+1].y);
				glTexCoord2d(0.5+(i+1)*delta,(j+1)*delta);
				glVertex2d(L[i+1][j+1].x, L[i+1][j+1].y);
				glTexCoord2d(0.5+(i)*delta,(j)*delta);
				glVertex2d(L[i][j].x, L[i][j].y);
				glTexCoord2d(0.5+(i+1)*delta,(j)*delta);
				glVertex2d(L[i+1][j].x, L[i+1][j].y);
			}
		glEnd();
	}

	// Right
	glColor3f(0.5,0.0,1.0);
	for(j=0; j < RES-1; j++)
	{
		glBindTexture(GL_TEXTURE_2D, textureid[2]);
		glBegin(GL_TRIANGLE_STRIP);
		for(i=0; i < ((RES+1)/2)-1; i++)
			{
				glTexCoord2d((i)*delta,(j+1)*delta);
				glVertex2d(R[i][j+1].x, R[i][j+1].y);
				glTexCoord2d((i)*delta,(j)*delta);
				glVertex2d(R[i][j].x, R[i][j].y);
				glTexCoord2d((i+1)*delta,(j+1)*delta);
				glVertex2d(R[i+1][j+1].x, R[i+1][j+1].y);
				glTexCoord2d((i+1)*delta,(j+1)*delta);
				glVertex2d(R[i+1][j+1].x, R[i+1][j+1].y);
				glTexCoord2d((i)*delta,(j)*delta);
				glVertex2d(R[i][j].x, R[i][j].y);
				glTexCoord2d((i+1)*delta,(j)*delta);
				glVertex2d(R[i+1][j].x, R[i+1][j].y);
			}
		glEnd();
	}

	// Top 
	glColor3f(0.0,1.0,0.0);
	for(j=0; j < ((RES+1)/2)-1; j++)
	{
		glBindTexture(GL_TEXTURE_2D, textureid[3]);
		glBegin(GL_TRIANGLE_STRIP);
		for(i=0; i < RES-1; i++)
			{
				glTexCoord2d((i)*delta,(j+1)*delta);
				glVertex2d(T[i][j+1].x, T[i][j+1].y);
				glTexCoord2d((i)*delta,(j)*delta);
				glVertex2d(T[i][j].x, T[i][j].y);
				glTexCoord2d((i+1)*delta,(j+1)*delta);
				glVertex2d(T[i+1][j+1].x, T[i+1][j+1].y);
				glTexCoord2d((i+1)*delta,(j+1)*delta);
				glVertex2d(T[i+1][j+1].x, T[i+1][j+1].y);
				glTexCoord2d((i)*delta,(j)*delta);
				glVertex2d(T[i][j].x, T[i][j].y);
				glTexCoord2d((i+1)*delta,(j)*delta);
				glVertex2d(T[i+1][j].x, T[i+1][j].y);

			}
		glEnd();
	}

	// Bottom 
	glColor3f(1.0,1.0,0.0);
	for(j=0; j < ((RES+1)/2)-1; j++)
	{
		glBindTexture(GL_TEXTURE_2D, textureid[4]);
		glBegin(GL_TRIANGLE_STRIP);
		for(i=0; i < RES-1; i++)
			{
				glTexCoord2d((i)*delta,0.5+(j+1)*delta);
				glVertex2d(B[i][j+1].x, B[i][j+1].y);
				glTexCoord2d((i)*delta,0.5+(j)*delta);
				glVertex2d(B[i][j].x, B[i][j].y);
				glTexCoord2d((i+1)*delta,0.5+(j+1)*delta);
				glVertex2d(B[i+1][j+1].x, B[i+1][j+1].y);
				glTexCoord2d((i+1)*delta,0.5+(j+1)*delta);
				glVertex2d(B[i+1][j+1].x, B[i+1][j+1].y);
				glTexCoord2d((i)*delta,0.5+(j)*delta);
				glVertex2d(B[i][j].x, B[i][j].y);
				glTexCoord2d((i+1)*delta,0.5+(j)*delta);
				glVertex2d(B[i+1][j].x, B[i+1][j].y);

			}
		glEnd();
	
	}

	glFlush();
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();

	glPopAttrib();

/* END: Draw The Fisheye View */

}



/*
void init(void)
{
	sp_Init(); 

	sp_Set_Offsets(0.0,0.42,0.0);
	sp_Set_Aperture_EC(135.0,-0.38);

	glEnable(GL_DEPTH_TEST);
}

*/
