#pragma once
#include "primitive.h"
#include "camera.h"

#define TRANSFORM_LOCAL_ONLY       0  
#define TRANSFORM_TRANS_ONLY       1  
#define TRANSFORM_LOCAL_TO_TRANS   2  

void Transform_RENDERLIST4D(RENDERLIST4D_PTR, Matrix4d *, int);

void Transform_OBJECT4D(OBJECT4D_PTR, Matrix4d *, int, int);


void Model_To_World_OBJECT4D(OBJECT4D_PTR obj, int coord_select = TRANSFORM_LOCAL_TO_TRANS);

void Model_To_World_RENDERLIST4D(RENDERLIST4D_PTR rend_list, Vector4d *pos, int coord_select = TRANSFORM_LOCAL_TO_TRANS);

void World_To_Camera_OBJECT4D(CAM4D_PTR, OBJECT4D_PTR);

void World_To_Camera_RENDERLIST4D(CAM4D_PTR, RENDERLIST4D_PTR);

void Camera_To_Perspective_OBJECT4D(OBJECT4D_PTR, CAM4D_PTR);

void Camera_To_Perspective_RENDERLIST4D(RENDERLIST4D_PTR, CAM4D_PTR);

void Perspective_To_Screen_OBJECT4D(OBJECT4D_PTR, CAM4D_PTR);

void Perspective_To_Screen_RENDERLIST4D(RENDERLIST4D_PTR, CAM4D_PTR);

void Camera_To_Perspective_Screen_OBJECT4D(OBJECT4D_PTR, CAM4D_PTR);

void Camera_Tp_Perspective_Screen_RENDERLIST4D(RENDERLIST4D_PTR, CAM4D_PTR);

Matrix4d Build_Model_To_World_Matrix4X4(Vector4d *);

Matrix4d Build_Camera_Tp_Perspective_MATRIX4X4(CAM4D_PTR);

Matrix4d Build_Perspective_To_Screen_MATRIX4X4(CAM4D_PTR);

Matrix4d Build_XYZ_Rotation_MATRIX4X4(float, float, float);

void Convert_From_Homogeneous4D_OBJECT4D(OBJECT4D_PTR);

void Convert_From_Homogeneous4d_REDNERLIST4D(RENDERLIST4D_PTR);