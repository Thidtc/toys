// glutEx1.cpp : 定义控制台应用程序的入口点。
//
//注意FPS函数的应用

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "glut.h"
#include "modelloader.h"
#include "utils.h"
#include "camera.h"
#include "transform.h"
#include "light.h"
#include "material.h"
#include "draw.h"

#define WND_WIDTH 800
#define WND_HEIGHT 600

extern LIGHT lights[MAX_LIGHTS];

extern MAT materials[MAX_MATERIALS];

extern int ZBuffer[WND_HEIGHT][WND_WIDTH];

Vector4d *cam_pos;
Vector4d *cam_dir;

Vector4d *vscale;
Vector4d *vpos;
Vector4d *vrot;

CAM4D cam;
OBJECT4D obj;
RENDERLIST4D rend_list;
POLYF4D poly1;
Vector4d *poly1_pos;

unsigned char *texture_arr;

int Game_Init(void *params = NULL);
int Game_Main(void *params = NULL);
int Game_Shutdown(void *params = NULL);

void updateView(int width, int height)
{
}

void reshape(int width, int height)
{
	updateView(height, width);
}

void idle()
{
	glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
}

void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256] = { 0 };
	char *c;

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(buffer, "FPS:%4.2f",
			frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
	glColor3f(1, 1, 1);
	glRasterPos2f(-1, -1);
	for (c = buffer; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
}


void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	getFPS();

	Game_Main();
	
	MAT mat = materials[0];
	int width = mat.texture->info.biWidth;
	int height = mat.texture->info.biHeight;

	/*for (int i = 0; i < height; i++)
	for (int j = 0; j < width; j++) {
		int index = 3 * (i * width + j);
		unsigned char r = mat.texture->arr[index];
		unsigned char g = mat.texture->arr[index + 1];
		unsigned char b = mat.texture->arr[index + 2];
		Draw_Pixel(200 + i, 100 + j, RGB(r, g, b));
	}*/

	/*Draw_Textured_Triangle(400 + 0, 300 + 0, 0, 0,
		400 + 64, 300 + 0, 1, 0,
		400 + 0, 300 + 64, 0, 1, &mat, lights, 0);*/
	/*Draw_Textured_Triangle(400 + 64, 300 + 64, 1, 1,
		400 + 64, 300 + 0, 1, 0,
		400 + 0, 300 + 64, 0, 1, &mat, lights, 0);*/



	Sleep(10);
	glFlush();
}

void init(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(800, 600);
	int windowHandle = glutCreateWindow("Simple GLUT App");
	//init();
	

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(redraw);

	Game_Init();
	glutMainLoop();
	return 0;
}


int Game_Init(void *param)
{
	cam_pos = new Vector4d(0, 0, -100, 0);
	cam_dir = new Vector4d(1, 1, 1, 0);
	//vscale = new Vector4d(0.5, 0.5, 0.5, 1);
	vscale = new Vector4d(4, 4, 4, 1);
	vpos = new Vector4d(0, 0, 0, 1);
	vrot = new Vector4d(0, 0, 0, 1);

	Build_Sin_Cos_Tables();

	Init_CAM4D(&cam,
		CAM_MODEL_EULER,
		cam_pos,
		cam_dir,
		NULL,
		50.0,
		500.0,
		90.0,
		WND_WIDTH,
		WND_HEIGHT);

	// ambient light
	Init_Light_LIGHT(0, LIGHT_STATE_ON, LIGHT_ATTR_AMBIENT, RGB(50, 50, 50), 0, 0, NULL, NULL,
		0, 0, 0, 0, 0, 0);

	// Directional light
	//Init_Light_LIGHT(1, LIGHT_STATE_ON, LIGHT_ATTR_INFINITE, 0, RGB(255, 0, 0), 0, NULL, &Vector4d(1, 0, 0, 0), 0, 0, 0, 0, 0, 0);

	// point light
	Init_Light_LIGHT(1, LIGHT_STATE_ON, LIGHT_ATTR_POINT, 0, RGB(255, 255, 255), 0, &Vector4d(0, 0, 60, 0), NULL, 0, 0.01, 0, 0, 0, 0);

	Load_OBJECT4D_PLG(&obj, "cube.plg", vscale, vpos, vrot);

	obj.world_pos(0) = 0;
	obj.world_pos(1) = 0;
	obj.world_pos(2) = 100;

	TEXTURE_PTR tex = new TEXTURE;
	BITMAPINFOHEADER *bitmapInfoHeader = new BITMAPINFOHEADER;
	tex->arr = LoadBitmapFile("texture4.bmp", bitmapInfoHeader);
	tex->info = *bitmapInfoHeader;

	Init_Material_MATERIAL(0, 1, 0, 0, NULL, "texture4.bmp", tex);

	return 1;
}

int Game_Main(void *params)
{
	static Matrix4d mrot;
	static float ang_y = 1;
	static float ang_x = 1;

	// CLEAR Zbuffer
	for (int i = 0; i < WND_HEIGHT; i++) {
		for (int j = 0; j < WND_WIDTH; j++) {
			ZBuffer[i][j] = INT_MAX;
		}
	}

	int index;

	Reset_OBJECT4D(&obj);

	mrot = Build_XYZ_Rotation_MATRIX4X4(0, ang_y, 0);                      // Transform 0 ms
	Transform_OBJECT4D(&obj, &mrot, TRANSFORM_LOCAL_ONLY, 1);
	Model_To_World_OBJECT4D(&obj);
	Light_OBJECT4D_World(&obj, &cam, lights, 4);

	for (int poly = 0; poly < obj.num_polys; poly++) {
		obj.plist[poly].world_pos[0] = obj.vlist_trans[obj.plist[poly].vert[0]];
		obj.plist[poly].world_pos[1] = obj.vlist_trans[obj.plist[poly].vert[1]];
		obj.plist[poly].world_pos[2] = obj.vlist_trans[obj.plist[poly].vert[2]];
	}

	Build_CAM4D_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);
	Remove_Backfaces_OBJECT4D(&obj, &cam);
	World_To_Camera_OBJECT4D(&cam, &obj);
	Camera_To_Perspective_OBJECT4D(&obj, &cam);
	Perspective_To_Screen_OBJECT4D(&obj, &cam);

	Draw_OBJECT4D_Solid(&obj, &cam, materials, lights);
	//Sleep(160);

	return 1;
}

int Game_Shutdown(void *params)
{
	delete[] texture_arr;
	return 1;
}
