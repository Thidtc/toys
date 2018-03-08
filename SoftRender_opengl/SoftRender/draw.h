#pragma once

#include "primitive.h"
#include "camera.h"
#include "material.h"
#include "light.h"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void Draw_Pixel(int, int, int);

void Draw_Top_Tri(int, int, int, int, int, int, int);

void Draw_Bottom_Tri(int, int, int, int, int, int, int);

void Draw_Triangle_2D(int, int, int, int, int, int, int);

void Draw_Textured_Triangle(int, int, float, float,
	int, int, float, float,
	int, int, float, float, MAT_PTR, LIGHT_PTR, int);

void Draw_Textured_Top_Tri(int, int, float, float,
	int, int, float, float,
	int, int, float, float, MAT_PTR, LIGHT_PTR, int);

void Draw_Textured_Bottom_Tri(int, int, float, float,
	int, int, float, float,
	int, int, float, float, MAT_PTR, LIGHT_PTR, int);

void Draw_Textured_Triangle_Z(int, int, float, float, float, float, float,
	int, int, float, float, float, float, float,
	int, int, float, float, float, float, float, MAT_PTR, LIGHT_PTR, POLY4D_PTR);

void Draw_Textured_Top_Tri_Z(int, int, float, float, float, float, float,
	int, int, float, float, float, float, float,
	int, int, float, float, float, float, float, MAT_PTR, LIGHT_PTR, POLY4D_PTR);

void Draw_Textured_Bottom_Tri_Z(int, int, float, float, float, float, float,
	int, int, float, float, float, float, float,
	int, int, float, float, float, float, float, MAT_PTR, LIGHT_PTR, POLY4D_PTR);

void Draw_OBJECT4D_Wire(OBJECT4D_PTR);

void Draw_RENDERLIST4D_Wire(RENDERLIST4D_PTR);

void Draw_OBJECT4D_Solid(OBJECT4D_PTR, CAM4D_PTR, MAT_PTR, LIGHT_PTR);

void Draw_RENDERLIST4D_Solid(RENDERLIST4D_PTR);