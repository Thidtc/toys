#pragma once
#include "stdafx.h"


#define MAT_ATTR_2SIDED                 0x0001
#define MAT_ATTR_TRANSPARENT            0x0002
#define MAT_ATTR_8BITCOLOR              0x0004
#define MAT_ATTR_RGB16                  0x0008
#define MAT_ATTR_RGB24                  0x0010

#define MAT_ATTR_SHADE_MODE_CONSTANT    0x0020
#define MAT_ATTR_SHADE_MODE_EMMISIVE    0x0020
#define MAT_ATTR_SHADE_MODE_FLAT        0x0040
#define MAT_ATTR_SHADE_MODE_GOURAUD     0x0080
#define MAT_ATTR_SHADE_MODE_FASTPHONG   0x0100
#define MAT_ATTR_SHADE_MODE_TEXTURE     0x0200

#define MAT_STATE_ACTIVE 0x0001

#define MAX_MATERIALS 16

#define _RGBA(r, g, b, a) ((a) + ((b) << 8) + ((g) << 16) + ((r) << 24))

#define RGB_R(a) ((a >> 24) & 0xff)
#define RGB_G(a) ((a >> 16) & 0xff)
#define RGB_B(a) ((a >> 8) & 0xff)

#define MAX_TEXTURES 16

typedef struct RGBA_TYP {
	union {
		int rgba;
		UCHAR rgba_M[4];
		struct {
			UCHAR a, b, g, r;
		};
	};
} RGBA, *RGBA_PTR;

typedef struct TEXTURE_TYP {
	unsigned char *arr;
	BITMAPINFOHEADER info;
} TEXTURE, *TEXTURE_PTR;

typedef struct MAT_TYP {
	int state;
	int id;
	char name[64];
	int attr;

	int color;
	float ka, kd, ks, power;

	int ra, rd, rs;

	char texture_file[80];
	TEXTURE_PTR texture;
} MAT, *MAT_PTR;

int Reset_Materials_MAT();

int Init_Material_MATERIAL(int, int, int, int, char *, char *, TEXTURE_PTR);