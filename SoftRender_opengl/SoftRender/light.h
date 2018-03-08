#pragma once
#include "material.h"
#include "primitive.h"
#include "camera.h"

#define LIGHT_ATTR_AMBIENT 0x0001    // basic ambient light
#define LIGHT_ATTR_INFINITE 0x0002    // infinite light source
#define LIGHT_ATTR_DIRECTIONAL 0x0002    // infinite light source (alias)
#define LIGHT_ATTR_POINT 0x0004    // point light source
#define LIGHT_ATTR_SPOTLIGHT1 0x0008    // spotlight type 1 (simple)
#define LIGHT_ATTR_SPOTLIGHT2 0x0010    // spotlight type 2 (complex)

#define LIGHT_STATE_ON 1
#define LIGHT_STATE_OFF 0         

#define MAX_LIGHTS       8    

typedef struct LIGHT_TYP {
	int state;
	int id;
	int attr;

	int c_ambient;
	int c_diffuse;
	int c_specular;
	Vector4d pos;
	Vector4d dir;

	float kc, kl, kq; // attenuation coeffu=icients
	float spot_inner; // inner angle 
	float spot_outer; //outer angle

	float pf; // power coeffient
} LIGHT, *LIGHT_PTR;

int Init_Light_LIGHT(int, int, int, int, int, int, Vector4d *, Vector4d *, float, float, float, float, float, float);

int Light_OBJECT4D_World(OBJECT4D_PTR, CAM4D_PTR, LIGHT_PTR, int);