#pragma once

#define PI         ((float)3.141592654f)
#define PI2        ((float)6.283185307f)
#define PI_DIV_2   ((float)1.570796327f)
#define PI_DIV_4   ((float)0.785398163f) 
#define PI_INV     ((float)0.318309886f) 

#define EPSILON_E3 (float)(1E-3) 
#define EPSILON_E4 (float)(1E-4) 
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)

#define DEG_TO_RAD(ang) ((ang)*PI/180.0)
#define RAD_TO_DEG(rads) ((rads)*180.0/PI)

#define BITMAP_ID 0x4D42

#define MIN(a, b)  (((a) < (b)) ? (a) : (b)) 
#define MAX(a, b)  (((a) > (b)) ? (a) : (b)) 

#define SWAP(a,b,t) {t=a; a=b; b=t;}

void Build_Sin_Cos_Tables();

float Fast_Sin(float);
float Fast_Cos(float);

float Fast_Distance(float, float, float);

float Fast_Distance_3D(Vector3d *);

float Fast_Distance_4D(Vector4d *);

#define  AJUST(a) ((float)(a) / 255)

unsigned char *LoadBitmapFile(char *, BITMAPINFOHEADER *);