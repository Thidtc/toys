#pragma once
#include "primitive.h"

#define CAM_ROT_SEQ_XYZ  0
#define CAM_ROT_SEQ_YXZ  1
#define CAM_ROT_SEQ_XZY  2
#define CAM_ROT_SEQ_YZX  3
#define CAM_ROT_SEQ_ZYX  4
#define CAM_ROT_SEQ_ZXY  5

#define CAM_MODEL_EULER            0x0008
#define CAM_MODEL_UVN              0x0010

#define UVN_MODE_SIMPLE            0 
#define UVN_MODE_SPHERICAL         1

typedef struct CAM4D_TYP {
	int state;
	int attr;

	Vector4d pos;
	Vector4d dir;

	Vector4d u;
	Vector4d v;
	Vector4d n;
	Vector4d target;

	//float view_dist_h;	// horizantal view distance
	//float view_dist_v;	// vertical view distance
	float view_dist;		//view distance


	float fov;	// view field

	// 3D clipping planes
	float near_clip_z; // near clipping plane
	float far_clip_z;	//far clipping plane

	PLANE3D rt_clip_plane;	// right clipping plane
	PLANE3D lt_clip_plane;	// left clipping plane
	PLANE3D tp_clip_plane;	// top clipping plane
	PLANE3D bt_clip_plane;	// bottom clipping plane

	float viewplane_width;	// view plane width
	float viewplane_height;	// view plane height

	float viewport_width;	// view port width
	float viewport_height;	// view port height
	float viewport_center_x;	// view port center
	float viewport_center_y;

	float aspect_ratio;	// width:height

	Matrix4d mcam;		// world to camera
	Matrix4d mper;		//camera to perspective
	Matrix4d mscr;		//perspective to screen
} CAM4D, *CAM4D_PTR;

void Init_CAM4D(CAM4D_PTR, int, Vector4d *, Vector4d *, Vector4d *, float, float, float, float, float);

void Build_CAM4D_Matrix_Euler(CAM4D_PTR, int);

void Build_CAM4D_Matrix_UVN(CAM4D_PTR, int);

int Cull_OBJECT4D(OBJECT4D_PTR, CAM4D_PTR, int);

void Reset_OBJECT4D(OBJECT4D_PTR);

void Reset_RENDERLIST4D(RENDERLIST4D_PTR);

void Remove_Backfaces_OBJECT4D(OBJECT4D_PTR, CAM4D_PTR);

void Remove_Backfaces_RENDERLIST4D(RENDERLIST4D_PTR, CAM4D_PTR);