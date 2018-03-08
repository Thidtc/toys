#pragma once

#include <iostream>
#include <Eigen/Dense>
#include <Windows.h>
#include "material.h"

#define OBJECT4D_MAX_VERTICES 16
#define OBJECT4D_MAX_POLYS 16
#define RENDERLIST4D_MAX_POLYS 16

// Attributes of polygons and faces
#define POLY4D_ATTR_2SIDED 0x0001
#define POLY4D_ATTR_TRANSPARENT 0x0002
#define POLY4D_ATTR_8BITCOLOR 0x0004
#define POLY4D_ATTR_RGB16 0x0008
#define POLY4D_ATTR_RGB24 0x0010

#define POLY4D_ATTR_SHADE_MODE_PURE 0x0020
#define POLY4D_ATTR_SHADE_MODE_FLAT 0x0040
#define POLY4D_ATTR_SHADE_MODE_GOURAUD 0x0080
#define POLY4D_ATTR_SHADE_MODE_PHONG 0x0100
#define POLY4D_ATTR_SHADE_MODE_FASTPHONG 0x100
#define POLY4D_ATTR_SHADE_MODE_TEXTURE 0x0200

#define POLY4D_ATTR_ENABLE_MATERIAL 0x0800
#define POLY4D_ATTR_DISABLE_MATERIAL 0x1000


// State of polygons and faces
#define POLY4D_STATE_NULL 0x0000
#define POLY4D_STATE_ACTIVE 0x0001
#define POLY4D_STATE_CLIPPED 0x0002
#define POLY4D_STATE_BACKFACE 0x0004
#define POLY4D_STATE_LIT 0x0008

// Attributes of objects
#define OBJECT4D_STATE_NULL 0x0000
#define OBJECT4D_STATE_ACTIVE 0x0001
#define OBJECT4D_STATE_VISIBLE 0x0002 
#define OBJECT4D_STATE_CULLED 0x0004

#define OBJECT4DV2_ATTR_SINGLE_FRAME 0x0001 // single frame object (emulates ver 1.0)
#define OBJECT4DV2_ATTR_MULTI_FRAME 0x0002 // multi frame object for .md2 support etc.
#define OBJECT4DV2_ATTR_TEXTURES 0x0004	

// Cull flags
#define CULL_OBJECT_X_PLANE           0x0001 // cull on the x clipping planes
#define CULL_OBJECT_Y_PLANE           0x0002 // cull on the y clipping planes
#define CULL_OBJECT_Z_PLANE           0x0004 // cull on the z clipping planes
#define CULL_OBJECT_XYZ_PLANES        (CULL_OBJECT_X_PLANE | CULL_OBJECT_Y_PLANE | CULL_OBJECT_Z_PLANE)

#define VERTEX_FLAGS_OVERRIDE_MASK          0xf000 
#define VERTEX_FLAGS_OVERRIDE_CONSTANT      0x1000
#define VERTEX_FLAGS_OVERRIDE_EMISSIVE      0x1000 
#define VERTEX_FLAGS_OVERRIDE_PURE          0x1000
#define VERTEX_FLAGS_OVERRIDE_FLAT          0x2000
#define VERTEX_FLAGS_OVERRIDE_GOURAUD       0x4000
#define VERTEX_FLAGS_OVERRIDE_TEXTURE       0x8000

#define VERTEX_FLAGS_INVERT_TEXTURE_U       0x0080   
#define VERTEX_FLAGS_INVERT_TEXTURE_V       0x0100   
#define VERTEX_FLAGS_INVERT_SWAP_UV         0x0800

#define VERTEX4DTV1_ATTR_NULL             0x0000 // this vertex is empty
#define VERTEX4DTV1_ATTR_POINT            0x0001
#define VERTEX4DTV1_ATTR_NORMAL           0x0002
#define VERTEX4DTV1_ATTR_TEXTURE          0x0004

using namespace std;
using namespace Eigen;

typedef struct VERTEX4D_TYP {
	union {
		double M[12];
		struct {
			double x, y, z, w;
			double nx, ny, nz, nw;
			float u0, v0;
			float i;
			int attr;
		};
		struct {
			Vector4d v;
			Vector4d n;
			Vector2d t;

			float i;
			int attr;
		};
	};
}VERTEX4D, *VERTEX4D_PTR;



typedef struct PLANE3D_TYP {
	Vector3d p0;
	Vector3d n;
} PLANE3D, *PLANE3D_PTR;


typedef struct POLY4D_TYP {
	int state;
	int attr;
	int color;
	int color_trans;

	Vector4d n;

	int lit_color[3];

	int tex_index;
	TEXTURE_PTR texture;

	int mati;

	Vector4d *vlist;
	int vert[3];
	Vector2d text[3];
	Vector4d world_pos[3];
} POLY4D, *POLY4D_PTR;

typedef struct POLYF4D_TYP {
	int state;
	int attr;
	int color;
	int color_trans;

	Vector4d vlist[3];
	Vector4d tvlist[3];

	Vector2d text[3];
	HBITMAP texture;

	POLYF4D_TYP *next;
	POLYF4D_TYP *prev;
} POLYF4D, *POLYF4D_PTR;

typedef struct OBJECT4D_TYP {
	int id;
	char name[64];
	int state;
	int attr;
	float avg_radius;
	float max_radius;
	Vector4d world_pos;
	Vector4d dir;
	Vector4d ux, uy, uz;
	int num_vertices;

	Vector4d vlist_local[OBJECT4D_MAX_VERTICES];
	Vector4d vlist_trans[OBJECT4D_MAX_VERTICES];

	int num_polys;
	POLY4D plist[OBJECT4D_MAX_POLYS];

} OBJECT4D, *OBJECT4D_PTR;

typedef struct RENDERLIST4D_TYP{
	int state; // State of render list
	int attr; // Attributes of render list

	POLYF4D_PTR poly_ptrs[RENDERLIST4D_MAX_POLYS]; 
	POLYF4D poly_data[RENDERLIST4D_MAX_POLYS];

	int num_polys;
} RENDERLIST4D, *RENDERLIST4D_PTR;



void PLANE3D_Init(PLANE3D_PTR, Vector3d *, Vector3d *, int);

float Compute_OBJECT4D_Radius(OBJECT4D_PTR);

int Insert_POLY4D_RENDERLIST4D(RENDERLIST4D_PTR, POLY4D_PTR);

int Insert_POLYF4D_RENDERLIST4D(RENDERLIST4D_PTR, POLYF4D_PTR);

