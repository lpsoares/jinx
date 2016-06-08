/* Lucas Peetz Dulley @ 2004/01/12
 */

#ifndef __SPHELIB_H__
#define __SPHELIB_H__

#define SP_PI 3.141592653589793238462643
#define RES 19 

#   ifdef __cplusplus
extern "C" {
#   endif

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

int gera_pontos(void);

void sp_Normalize(sp_point *p);

void sp_Set_Offsets(double xOff, double yOff, double zOff);

sp_point sp_Get_Offsets(void);

void sp_Set_Aperture_EC(float aperture, float ecent);

void sp_Set_Aperture(float aperture);

void sp_Set_EC(float ecent);

float sp_Get_Aperture(void);

float sp_Get_EC(void);

void sp_CalcCamera(int mode);

vertex2f sp_Coord(sp_point p);

void sp_Init(void);

void sp_CaptureBegin(int i, double eye);

void sp_CaptureEnd(int i);

void sp_draw(int sp_F_width, int sp_F_height);

#   ifdef __cplusplus
}
#   endif

#endif
