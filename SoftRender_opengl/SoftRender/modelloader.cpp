#include "stdafx.h"
#include "modelloader.h"

//#include <iostream>
//#include <Eigen/Dense>
//
//using namespace std;
//using namespace 


char *Get_Line_PLG(char *buffer, int maxlength, FILE *fp)
{
	int index = 0;
	int length = 0;
	while (1) {
		if (!fgets(buffer, maxlength, fp))
			return NULL;
		// Count the space number
		for (length = strlen(buffer), index = 0; isspace(buffer[index]); index++);
		// Check if the line is empty or comment
		if (index >= length || buffer[index] == '#')
			continue;
		return &buffer[index];
	}
}

int Load_OBJECT4D_PLG(OBJECT4D_PTR obj, char *filename, Vector4d *scale, Vector4d *pos, Vector4d *rot)
{
	FILE *fp;
	char buffer[256];
	char *token_string;

	memset(obj, 0, sizeof(OBJECT4D));

	obj->state = OBJECT4D_STATE_ACTIVE | OBJECT4D_STATE_VISIBLE;

	// Set the position of object
	obj->world_pos = *pos;

	if ((fp = fopen(filename, "r")) == NULL) {
		printf("Couldn't open file %s", filename);
		return 0;
	}

	// Read descriptor of object
	if (!(token_string = Get_Line_PLG(buffer, 255, fp))) {
		printf("PLG file error with file %s(object descriptor invalid)", filename);
		return 0;
	}

	printf("Object Descriptor: %s", token_string);

	sscanf(token_string, "%s %d %d", obj->name, &obj->num_vertices, &obj->num_polys);

	// Read vertex list
	for (int vertex = 0; vertex < obj->num_vertices; vertex++) {
		if (!(token_string = Get_Line_PLG(buffer, 255, fp))) {
			printf("PLG file error with file %s(vertex list invalid)", filename);
			return 0;
		}
		float t1, t2, t3;
		sscanf(token_string, "%f %f %f", &t1, &t2, &t3);
		obj->vlist_local[vertex](0) = t1 * (*scale)(0);
		obj->vlist_local[vertex](1) = t2 * (*scale)(1);
		obj->vlist_local[vertex](2) = t3 * (*scale)(2);
		obj->vlist_local[vertex](3) = 1;

		printf("\nVertext %d = %f, %f, %f, %f\n", vertex, obj->vlist_local[vertex](0)
			, obj->vlist_local[vertex](1)
			, obj->vlist_local[vertex](2)
			, obj->vlist_local[vertex](3));
	}

	Compute_OBJECT4D_Radius(obj);


	// Read polygons list
	int poly_surface_desc = 0;
	int poly_num_vertex = 0;
	char tmp_string[8];

	for (int poly = 0; poly < obj->num_polys; poly++) {
		if (!(token_string = Get_Line_PLG(buffer, 255, fp))) {
			printf("PLG file error with file %s(polygon descriptor invalid)", filename);
			return 0;
		}
		float f1, f2, f3, f4, f5, f6;
		sscanf(token_string, "%s %d %d %d %d %d %f %f %f %f %f %f", tmp_string, &poly_num_vertex,
			&obj->plist[poly].vert[0],
			&obj->plist[poly].vert[1],
			&obj->plist[poly].vert[2],
			&obj->plist[poly].tex_index,
			&f1, &f2, &f3, &f4, &f5, &f6);
		obj->plist[poly].text[0] = Vector2d(f1, f2);
		obj->plist[poly].text[1] = Vector2d(f3, f4);
		obj->plist[poly].text[2] = Vector2d(f5, f6);

		// Check for descriptor
		if (tmp_string[0] == '0' && toupper(tmp_string[1]) == 'X')
			sscanf(tmp_string, "%x", &poly_surface_desc);
		else
			poly_surface_desc = atoi(tmp_string);

		obj->plist[poly].vlist = obj->vlist_local;

		printf("\nSurface Desc = %x%.4x, num_verts = %d, vert_indices[%d, %d, %d]"
			, poly_surface_desc, poly_num_vertex, 
			&obj->plist[poly].vert[0],
			&obj->plist[poly].vert[1],
			&obj->plist[poly].vert[2]);

		obj->plist[poly].state = POLY4D_STATE_ACTIVE;

		obj->plist[poly].attr |= POLY4D_ATTR_SHADE_MODE_FLAT;

		obj->plist[poly].color = RGB(50, 50, 50);

		// Set color mode
		//if ((poly_surface_desc & PLX))
	}

	fclose(fp);
	return 1;
}