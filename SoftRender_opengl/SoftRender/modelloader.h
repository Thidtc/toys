#pragma once

#include "primitive.h"

/************************
 PLG file
 ************************/

// Get one line (not empty or comment) from the file
char *Get_Line_PLG(char *, int, FILE *);

// Load object from file
int Load_OBJECT4D_PLG(OBJECT4D_PTR, char *, Vector4d *, Vector4d *, Vector4d *);