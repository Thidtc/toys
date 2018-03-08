#include "stdafx.h"
#include "material.h"

MAT materials[MAX_MATERIALS];
int num_materials = 0;

int Reset_Materials_MAT()
{
	static int first_time = 1;
	return 1;
}

int Init_Material_MATERIAL(
	int index,
	int _id,
	int _state,
	int _attr,
	char *_name,
	char *_texture_file,
	TEXTURE_PTR _texture)
{
	if (_id < 0 || _id >= MAX_MATERIALS)
		return 0;

	materials[index].id = _id;
	materials[index].state = _state;
	materials[index].attr = _attr;
	if (_name != NULL)
		strncpy(materials[index].name, _name, 64);
	if (_texture_file != NULL)
		strncpy(materials[index].texture_file, _texture_file, 80);
	materials[index].texture = _texture;
	return 1;
}