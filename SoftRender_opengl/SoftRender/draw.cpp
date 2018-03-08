#include "stdafx.h"
#include "draw.h"
#include "utils.h"

int min_clip_x = 0,                             // clipping rectangle 
max_clip_x = (SCREEN_WIDTH - 1),
min_clip_y = 0,
max_clip_y = (SCREEN_HEIGHT - 1);

int ZBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

void Draw_Pixel(int x, int y, int color)
{
	//glBegin(GL_POINTS);
	glColor3f(AJUST(GetRValue(color)), AJUST(GetGValue(color)), AJUST(GetBValue(color)));

	glVertex2f((float)(x * 2) / SCREEN_WIDTH - 1, (float)(y * 2) / SCREEN_WIDTH - 1);
	
	//glEnd();
}

void Draw_Textured_Triangle(int x1, int y1, float u1, float v1,
	int x2, int y2, float u2, float v2,
	int x3, int y3, float u3, float v3, MAT_PTR mat, LIGHT_PTR lights, int color)
{
	int temp_x, temp_y, new_x;
	float temp_u, temp_v, new_u, new_v;

	if ((x1 == x2 && x2 == x3) || (y1 == y2 && y2 == y3))
		return;
	if (y2 < y1) {
		temp_x = x2;
		temp_y = y2;
		temp_u = u2;
		temp_v = v2;
		x2 = x1;
		y2 = y1;
		u2 = u1;
		v2 = v1;
		x1 = temp_x;
		y1 = temp_y;
		u1 = temp_u;
		v1 = temp_v;
	}
	if (y3 < y1) {
		temp_x = x3;
		temp_y = y3;
		temp_u = u3;
		temp_v = v3;
		x3 = x1;
		y3 = y1;
		u3 = u1;
		v3 = v1;
		x1 = temp_x;
		y1 = temp_y;
		v3 = temp_v;
		u3 = temp_u;
	}
	if (y3 < y2) {
		temp_x = x3;
		temp_y = y3;
		temp_u = u3;
		temp_v = v3;
		x3 = x2;
		y3 = y2;
		u3 = u2;
		v3 = v2;
		x2 = temp_x;
		y2 = temp_y;
		u2 = temp_u;
		v2 = temp_v;
	}

	if (y3<min_clip_y || y1>max_clip_y ||
		(x1<min_clip_x && x2<min_clip_x && x3<min_clip_x) ||
		(x1>max_clip_x && x2>max_clip_x && x3>max_clip_x))
		return;

	if (y1 == y2) {
		Draw_Textured_Top_Tri(x1, y1, u1, v1,
			x2, y2, u2, v2,
			x3, y3, u3, v3, mat, lights, color);
	} else if (y2 == y3) {
		Draw_Textured_Bottom_Tri(x1, y1, u1, v1,
			x2, y2, u2, v2,
			x3, y3, u3, v3, mat, lights, color);
	} else {
		new_x = x1 + (int)(0.5 + (float)(y2 - y1) * (float)(x3 - x1) / (float)(y3 - y1));
		new_u = u1 + (y2 - y1) * (u3 - u1) / (y3 - y1);
		new_v = v1 + (y2 - y1) * (v3 - v1) / (y3 - y1);

		Draw_Textured_Bottom_Tri(x1, y1, u1, v1,
			new_x, y2, new_u, new_v,
			x2, y2, u2, v2, mat, lights, color);

		Draw_Textured_Top_Tri(x2, y2, u2, v2,
			new_x, y2, new_u, new_v, 
			x3, y3, u3, v3, mat, lights, color);
	}
}

void Draw_Textured_Top_Tri(int x1, int y1, float u1, float v1,
	int x2, int y2, float u2, float v2,
	int x3, int y3, float u3, float v3, MAT_PTR mat, LIGHT_PTR lights, int color)
{
	float dx_right, dx_left, xs, xe, height, height_recip;
	float du_right, dv_right;
	float du_left, dv_left;
	float us, vs, ue, ve;
	float ui, vi;
	float du, dv, dx_recip;

	int temp_x, temp_y;
	float temp_u, temp_v;
	int right, left;
	int row, col;
	unsigned char r, g, b;

	//printf("%f,%f %f,%f %f,%f\n", u1, v1, u2, v2, u3, v3);

	if (x2 < x1) {
		temp_x = x2;
		temp_u = u2;
		temp_v = v2;
		x2 = x1;
		u2 = u1;
		v2 = v1;
		x1 = temp_x;
		u1 = temp_u;
		v1 = temp_v;
	}

	height = y3 - y1;
	height_recip = 1.0 / height;

	dx_left = (x3 - x1) * height_recip;
	du_left = (u3 - u1) * height_recip;
	dv_left = (v3 - v1) * height_recip;

	dx_right = (x3 - x2) * height_recip;
	du_right = (u3 - u2) * height_recip;
	dv_right = (v3 - v2) * height_recip;

	xs = (float)x1;
	xe = (float)x2 + (float)0.5;
	us = u1;
	vs = v1;
	ue = u2;
	ve = v2;

	if (y1 < min_clip_y) {
		xs = xs + dx_left * (float)(-y1 + min_clip_y);
		xe = xe + dx_right * (float)(-y1 + min_clip_y);
		us = us + du_left * (float)(-y1 + min_clip_y);
		ue = ue + du_right * (float)(-y1 + min_clip_y);
		vs = vs + dv_left * (float)(-y1 + min_clip_y);
		ve = ve + dv_right * (float)(-y1 + min_clip_y);

		y1 = min_clip_y;
	}

	if (y3 > max_clip_y) {
		y3 = max_clip_y;
	}

	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x) {
		for (temp_y = y1; temp_y < y3; temp_y++) {
			ui = us;
			vi = vs;
			dx_recip = 1.0 / (xe - xs);
			du = (ue - us) * dx_recip;
			dv = (ve - vs) * dx_recip;
			for (int i = (int)(xs + 0.5); i <= (int)(xe + 0.5); i++) {
				col = ui * mat->texture->info.biWidth;
				col = MIN(col, mat->texture->info.biWidth - 1);
				col = MAX(col, 0);
				row = vi * mat->texture->info.biHeight;
				row = MIN(row, mat->texture->info.biWidth - 1);
				row = MAX(row, 0);

				int index = 3 * (row * mat->texture->info.biWidth + col);
				r = mat->texture->arr[index];
				g = mat->texture->arr[index + 1];
				b = mat->texture->arr[index + 2];
				r += GetRValue(color);
				g += GetGValue(color);
				b += GetBValue(color);
				if (r >= 255) r = 255;
				if (g >= 255) g = 255;
				if (b >= 255) b = 255;
				Draw_Pixel(i, temp_y, RGB(r, g, b));
				ui += du;
				vi += dv;
			}
			xs += dx_left;
			xe += dx_right;
			us += du_left;
			ue += du_right;
			vs += dv_left;
			ve += dv_right;
		}
	} else {
		for (temp_y = y1; temp_y = y3; temp_y++) {
			left = (int)xs;
			right = (int)xe;

			if (left < min_clip_x) {
				left = min_clip_x;
				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x) {
				right = max_clip_x;
				if (left > max_clip_x)
					continue;
			}

			dx_recip = 1.0 / (xe - xs);
			du = (ue - us) * dx_recip;
			dv = (ve - vs) * dx_recip;
			ui = us + du * (left - (int)xs);
			vi = vs + dv * (left - (int)xs);

			for (int i = left; i <= right; i++) {
				col = ui * mat->texture->info.biWidth;
				col = MIN(col, mat->texture->info.biWidth - 1);
				col = MAX(col, 0);
				row = vi * mat->texture->info.biHeight;
				row = MIN(row, mat->texture->info.biWidth - 1);
				row = MAX(row, 0);

				int index = 3 * (row * mat->texture->info.biWidth + col);
				r = mat->texture->arr[index];
				g = mat->texture->arr[index + 1];
				b = mat->texture->arr[index + 2];
				r += GetRValue(color);
				g += GetGValue(color);
				b += GetBValue(color);
				if (r >= 255) r = 255;
				if (g >= 255) g = 255;
				if (b >= 255) b = 255;
				Draw_Pixel(i, temp_y, RGB(r, g, b));
				ui += du;
				vi += dv;
			}
			xs += dx_left;
			xe += dx_right;
			us += du_left;
			ue += du_right;
			vs += dv_left;
			ve += dv_right;
		}
	}
}

void Draw_Textured_Bottom_Tri(int x1, int y1, float u1, float v1,
	int x2, int y2, float u2, float v2,
	int x3, int y3, float u3, float v3, MAT_PTR mat, LIGHT_PTR lights, int color)
{
	float dx_right, dx_left, xs, xe, height, height_recip;
	float du_right, dv_right;
	float du_left, dv_left;
	float us, vs, ue, ve;
	float ui, vi;
	float du, dv, dx_recip;

	int temp_x, temp_y;
	float temp_u, temp_v;
	int right, left;
	int row, col;
	unsigned char r, g, b;
	if (x3 < x2) {
		temp_x = x2;
		temp_u = u2;
		temp_v = v2;
		x2 = x3;
		u2 = u3;
		v2 = v3;
		x3 = temp_x;
		u3 = temp_u;
		v3 = temp_v;
	}

	height = y3 - y1;
	height_recip = 1.0 / height;

	dx_left = (x2 - x1) * height_recip;
	du_left = (u2 - u1) * height_recip;
	dv_left = (v2 - v1) * height_recip;

	dx_right = (x3 - x1) * height_recip;
	du_right = (u3 - u1) * height_recip;
	dv_right = (v3 - v1) * height_recip;

	xs = (float)x1;
	xe = (float)x1 + (float)0.5;
	us = u1;
	vs = v1;
	ue = u1;
	ve = v1;

	if (y1 < min_clip_y) {
		xs = xs + dx_left * (float)(-y1 + min_clip_y);
		xe = xe + dx_right * (float)(-y1 + min_clip_y);
		us = us + du_left * (float)(-y1 + min_clip_y);
		ue = ue + du_right * (float)(-y1 + min_clip_y);
		vs = vs + dv_left * (float)(-y1 + min_clip_y);
		ve = ve + dv_right * (float)(-y1 + min_clip_y);

		y1 = min_clip_y;
	}

	if (y3 > max_clip_y) {
		y3 = max_clip_y;
	}

	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x) {
		for (temp_y = y1; temp_y < y3; temp_y++) {
			ui = us;
			vi = vs;
			dx_recip = 1.0 / (xe - xs);
			du = (ue - us) * dx_recip;
			dv = (ve - vs) * dx_recip;
			for (int i = (int)(xs + 0.5); i <= (int)(xe + 0.5); i++) {
				col = ui * mat->texture->info.biWidth;
				col = MIN(col, mat->texture->info.biWidth - 1);
				col = MAX(col, 0);
				row = vi * mat->texture->info.biHeight;
				row = MIN(row, mat->texture->info.biWidth - 1);
				row = MAX(row, 0);

				int index = 3 * (row * mat->texture->info.biWidth + col);
				r = mat->texture->arr[index];
				g = mat->texture->arr[index + 1];
				b = mat->texture->arr[index + 2];
				r += GetRValue(color);
				g += GetGValue(color);
				b += GetBValue(color);
				if (r >= 255) r = 255;
				if (g >= 255) g = 255;
				if (b >= 255) b = 255;

				Draw_Pixel(i, temp_y, RGB(r, g, b));
				ui += du;
				vi += dv;
			}
			xs += dx_left;
			xe += dx_right;
			us += du_left;
			ue += du_right;
			vs += dv_left;
			ve += dv_right;
		}
	}
	else {
		for (temp_y = y1; temp_y < y3; temp_y++) {
			left = (int)xs;
			right = (int)xe;

			if (left < min_clip_x) {
				left = min_clip_x;
				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x) {
				right = max_clip_x;
				if (left > max_clip_x)
					continue;
			}

			dx_recip = 1.0 / (xe - xs);
			du = (ue - us) * dx_recip;
			dv = (ve - vs) * dx_recip;
			ui = us + du * (left - (int)xs);
			vi = vs + dv * (left - (int)xs);

			for (int i = left; i <= right; i++) {
				col = ui * mat->texture->info.biWidth;
				col = MIN(col, mat->texture->info.biWidth - 1);
				col = MAX(col, 0);
				row = vi * mat->texture->info.biHeight;
				row = MIN(row, mat->texture->info.biWidth - 1);
				row = MAX(row, 0);
				if (col >= 64 || col < 0 || row >= 64 || row < 0) {
					int m;
					m = 0;
				}
				int index = 3 * (row * mat->texture->info.biWidth + col);
				r = mat->texture->arr[index];
				g = mat->texture->arr[index + 1];
				b = mat->texture->arr[index + 2];
				r += GetRValue(color);
				g += GetGValue(color);
				b += GetBValue(color);
				if (r >= 255) r = 255;
				if (g >= 255) g = 255;
				if (b >= 255) b = 255;
				Draw_Pixel(i, temp_y, RGB(r, g, b));
				ui += du;
				vi += dv;
			}

			xs += dx_left;
			xe += dx_right;
			us += du_left;
			ue += du_right;
			vs += dv_left;
			ve += dv_right;
		}
	}
}

void Draw_Textured_Triangle_Z(int x1, int y1, float xx1, float yy1, float zz1, float u1, float v1,
	int x2, int y2, float xx2, float yy2, float zz2, float u2, float v2,
	int x3, int y3, float xx3, float yy3, float zz3, float u3, float v3, MAT_PTR mat, LIGHT_PTR lights, POLY4D_PTR poly)
{
	int temp_x, temp_y, new_x;
	int temp_xx, temp_yy, temp_zz;
	float new_xx, new_yy, new_zz;
	float temp_u, temp_v, new_u, new_v;

	if ((x1 == x2 && x2 == x3) || (y1 == y2 && y2 == y3))
		return;
	if (y2 < y1) {
		temp_xx = xx2;
		temp_yy = yy2;
		temp_zz = zz2;
		temp_x = x2;
		temp_y = y2;
		temp_u = u2;
		temp_v = v2;
		xx2 = xx1;
		yy2 = yy1;
		zz2 = zz1;
		x2 = x1;
		y2 = y1;
		u2 = u1;
		v2 = v1;
		xx1 = temp_xx;
		yy1 = temp_yy;
		zz1 = temp_zz;
		x1 = temp_x;
		y1 = temp_y;
		u1 = temp_u;
		v1 = temp_v;
	}
	if (y3 < y1) {
		temp_xx = xx3;
		temp_yy = yy3;
		temp_zz = zz3;
		temp_x = x3;
		temp_y = y3;
		temp_u = u3;
		temp_v = v3;
		xx3 = xx1;
		yy3 = yy1;
		zz3 = zz1;
		x3 = x1;
		y3 = y1;
		u3 = u1;
		v3 = v1;
		xx1 = temp_xx;
		yy1 = temp_yy;
		zz1 = temp_zz;
		x1 = temp_x;
		y1 = temp_y;
		v3 = temp_v;
		u3 = temp_u;
	}
	if (y3 < y2) {
		temp_xx = xx3;
		temp_yy = yy3;
		temp_zz = zz3;
		temp_x = x3;
		temp_y = y3;
		temp_u = u3;
		temp_v = v3;
		xx3 = xx2;
		yy3 = yy2;
		zz3 = zz2;
		x3 = x2;
		y3 = y2;
		u3 = u2;
		v3 = v2;
		xx2 = temp_xx;
		yy2 = temp_yy;
		zz2 = temp_zz;
		x2 = temp_x;
		y2 = temp_y;
		u2 = temp_u;
		v2 = temp_v;
	}

	if (y3<min_clip_y || y1>max_clip_y ||
		(x1<min_clip_x && x2<min_clip_x && x3<min_clip_x) ||
		(x1>max_clip_x && x2>max_clip_x && x3>max_clip_x))
		return;

	if (y1 == y2) {
		Draw_Textured_Top_Tri_Z(x1, y1, xx1, yy1, zz1, u1, v1,
			x2, y2, xx2, yy2, zz2, u2, v2,
			x3, y3, xx3, yy3, zz3, u3, v3, mat, lights, poly);
	}
	else if (y2 == y3) {
		Draw_Textured_Bottom_Tri_Z(x1, y1, xx1, yy1, zz1, u1, v1,
			x2, y2, xx2, yy2, zz2, u2, v2,
			x3, y3, xx3, yy3, zz3, u3, v3, mat, lights, poly);
	}
	else {
		new_x = x1 + (int)(0.5 + (float)(y2 - y1) * (float)(x3 - x1) / (float)(y3 - y1));
		//new_u = u1 + (y2 - y1) * (u3 - u1) / (y3 - y1);
		//new_v = v1 + (y2 - y1) * (v3 - v1) / (y3 - y1);
		float z1 = 1.0 / zz1;
		float z3 = 1.0 / zz3;
		float t = (float)(y2 - y1) / (float)(y3 - y1);
		float z_recip = z1 * (1 - t) + z3 * t;

		new_u = 1.0 / z_recip * (z1 * u1 * (1 - t) + z3 * u3 * t);
		new_v = 1.0 / z_recip * (z1 * v1 * (1 - t) + z3 * v3 * t);

		new_xx = xx1 + (y2 - y1) * (xx3 - xx1) / (y3 - y1);
		new_yy = yy1 + (y2 - y1) * (yy3 - yy1) / (y3 - y1);
		new_zz = zz1 + (y2 - y1) * (zz3 - zz1) / (y3 - y1);

		Draw_Textured_Bottom_Tri_Z(x1, y1, xx1, yy1, zz1, u1, v1,
			new_x, y2, new_xx, new_yy, new_zz, new_u, new_v,
			x2, y2, xx2, yy2, zz2, u2, v2, mat, lights, poly);

		Draw_Textured_Top_Tri_Z(x2, y2, xx2, yy2, zz2, u2, v2,
			new_x, y2, new_xx, new_yy, new_zz, new_u, new_v,
			x3, y3, xx3, yy3, zz3, u3, v3, mat, lights, poly);
	}
}


void Draw_Textured_Top_Tri_Z(int x1, int y1, float xx1, float yy1, float zz1, float u1, float v1,
	int x2, int y2, float xx2, float yy2, float zz2, float u2, float v2,
	int x3, int y3, float xx3, float yy3, float zz3, float u3, float v3, MAT_PTR mat, LIGHT_PTR lights, POLY4D_PTR poly)
{
	float dx_right, dx_left, xs, xe, height, height_recip;
	float du_right, dv_right;
	float du_left, dv_left;
	float us, vs, ue, ve;
	float ui, vi;
	float zi;
	float du, dv, dx_recip;
	float dzz_left, dzz_right, dzz, temp_zz, zzi;
	float zzs, zze;
	float dxx_left, dxx_right, dxx, temp_xx, xxi;
	float xxs, xxe;
	float dyy_left, dyy_right, dyy, temp_yy, yyi;
	float yys, yye;
	float z_recip;

	int temp_x, temp_y;
	float temp_u, temp_v;
	int right, left;
	int row, col;
	int r, g, b;
	int r_base, g_base, b_base;

	if (x2 < x1) {
		temp_zz = zz2;
		temp_yy = yy2;
		temp_xx = xx2;
		temp_x = x2;
		temp_u = u2;
		temp_v = v2;
		zz2 = zz1;
		yy2 = yy1;
		xx2 = xx1;
		x2 = x1;
		u2 = u1;
		v2 = v1;
		zz1 = temp_zz;
		yy1 = temp_yy;
		xx1 = temp_xx;
		x1 = temp_x;
		u1 = temp_u;
		v1 = temp_v;
	}

	height = y3 - y1;
	height_recip = 1.0 / height;

	dx_left = (x3 - x1) * height_recip;
	dzz_left = (zz3 - zz1) * height_recip;
	dyy_left = (yy3 - yy1) * height_recip;
	dxx_left = (xx3 - xx1) * height_recip;
	du_left = (u3 - u1) * height_recip;
	dv_left = (v3 - v1) * height_recip;

	dx_right = (x3 - x2) * height_recip;
	dzz_right = (zz3 - zz2) * height_recip;
	dyy_right = (yy3 - yy2) * height_recip;
	dxx_right = (xx3 - xx2) * height_recip;
	du_right = (u3 - u2) * height_recip;
	dv_right = (v3 - v2) * height_recip;


	zzs = (float)zz1;
	zze = (float)zz2 + (float)0.5;
	yys = (float)yy1;
	yye = (float)yy2 + (float)0.5;
	xxs = (float)xx1;
	xxe = (float)xx2 + (float)0.5;
	xs = (float)x1;
	xe = (float)x2 + (float)0.5;
	us = u1;
	vs = v1;
	ue = u2;
	ve = v2;

	if (y1 < min_clip_y) {
		zzs = zzs + dzz_left * (float)(-y1 + min_clip_y);
		zze = zze + dzz_right * (float)(-y1 + min_clip_y);
		yys = yys + dyy_left * (float)(-y1 + min_clip_y);
		yye = yye + dyy_right * (float)(-y1 + min_clip_y);
		xxs = xxs + dxx_left * (float)(-y1 + min_clip_y);
		xxe = xxe + dxx_right * (float)(-y1 + min_clip_y);
		xs = xs + dx_left * (float)(-y1 + min_clip_y);
		xe = xe + dx_right * (float)(-y1 + min_clip_y);
		us = us + du_left * (float)(-y1 + min_clip_y);
		ue = ue + du_right * (float)(-y1 + min_clip_y);
		vs = vs + dv_left * (float)(-y1 + min_clip_y);
		ve = ve + dv_right * (float)(-y1 + min_clip_y);

		y1 = min_clip_y;
	}

	if (y3 > max_clip_y) {
		y3 = max_clip_y;
	}

	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x) {
		for (temp_y = y1; temp_y <= y3; temp_y++) {


			zzi = zzs;
			yyi = yys;
			xxi = xxs;

			dx_recip = 1.0 / (xe - xs);
			du = (ue - us) * dx_recip;
			dv = (ve - vs) * dx_recip;
			dzz = (zze - zzs) * dx_recip;
			dyy = (yye - yys) * dx_recip;
			dxx = (xxe - xxs) * dx_recip;

			float zz3_recip = 1.0 / zz3;
			float zz2_recip = 1.0 / zz2;
			float zz1_recip = 1.0 / zz1;

			float zl = (temp_y - y1) * (zz3_recip - zz1_recip) / (y3 - y1) + zz1_recip;
			float zr = (temp_y - y1) * (zz3_recip - zz2_recip) / (y3 - y1) + zz2_recip;

			float u3_recip = u3 * zz3_recip;
			float u2_recip = u2 * zz2_recip;
			float u1_recip = u1 * zz1_recip;

			float ul = (temp_y - y1) * (u3_recip - u1_recip) / (y3 - y1) + u1_recip;
			float ur = (temp_y - y1) * (u3_recip - u2_recip) / (y3 - y1) + u2_recip;

			float v3_recip = v3 * zz3_recip;
			float v2_recip = v2 * zz2_recip;
			float v1_recip = v1 * zz1_recip;

			float vl = (temp_y - y1) * (v3_recip - v1_recip) / (y3 - y1) + v1_recip;
			float vr = (temp_y - y1) * (v3_recip - v2_recip) / (y3 - y1) + v2_recip;

			float u_step = (ur - ul) / ((int)xe - (int)xs);
			float v_step = (vr - vl) / ((int)xe - (int)xs);
			float z_step = (zr - zl) / ((int)xe - (int)xs);

			ui = ul;
			vi = vl;
			zi = zl;


			for (int i = (int)(xs); i <= (int)(xe); i++) {
				if (temp_y == 300) {
					int m = 1;
				}
				/*col = ui * mat->texture->info.biWidth;
				col = MIN(col, mat->texture->info.biWidth - 1);
				col = MAX(col, 0);
				row = vi * mat->texture->info.biHeight;
				row = MIN(row, mat->texture->info.biWidth - 1);
				row = MAX(row, 0);*/
				/*float zl = 1.0 / zzs;
				float zr = 1.0 / zze;
				float t = (float)(i - xs) / (float)(xe - xs);
				float z_recip = zl * (1 - t) + zr * t;
				
				float u = 1.0 / z_recip * (zl * us * (1 - t) + zr * ue * t);
				float v = 1.0 / z_recip * (zl * vs * (1 - t) + zr * ve * t);*/
				float u = ui / zi;
				float v = vi / zi;
				/*float t = (float)(i - xs) / (xe - xs);
				float zzz = zl * (1 - t) + zr * t;
				float u = 1.0 / zzz * (ul * (1 - t) + ur * t);
				float v = 1.0 / zzz * (vl * (1 - t) + vr * t);*/


				col = u * mat->texture->info.biWidth;
				col = MIN(col, mat->texture->info.biWidth - 1);
				col = MAX(col, 0);
				row = v * mat->texture->info.biHeight;
				row = MIN(row, mat->texture->info.biWidth - 1);
				row = MAX(row, 0);

				int index = 3 * (row * mat->texture->info.biWidth + col);
				r = mat->texture->arr[index];
				g = mat->texture->arr[index + 1];
				b = mat->texture->arr[index + 2];

				r_base = GetRValue(poly->color);
				g_base = GetGValue(poly->color);
				b_base = GetBValue(poly->color);

				int curr_light = 1;
				if (lights[curr_light].attr & LIGHT_ATTR_POINT) {
					int x = lights[curr_light].pos(0) - xxi;
					int y = lights[curr_light].pos(1) - yyi;
					int z = lights[curr_light].pos(2) - zzi;

					float dist = Fast_Distance(x, y, z);
					float dp = poly->n(0) * x + poly->n(1) * y + poly->n(2) * z;

					if (dp > 0)
					{
						float atten = (lights[curr_light].kc + lights[curr_light].kl*dist + lights[curr_light].kq*dist*dist);
						atten *= dist;
						r += ((GetRValue(lights[curr_light].c_diffuse) * r_base * dp / atten)) / (256);
						g += ((GetGValue(lights[curr_light].c_diffuse) * g_base * dp / atten)) / (256);
						b += ((GetBValue(lights[curr_light].c_diffuse) * b_base * dp / atten)) / (256);
					}
				}

				if (r >= 255) r = 255;
				if (g >= 255) g = 255;
				if (b >= 255) b = 255;

				if (zzi < ZBuffer[i][temp_y]) {
					Draw_Pixel(i, temp_y, RGB(r, g, b));
					ZBuffer[i][temp_y] = zzi;
				}
				
				ui += u_step;
				vi += v_step;
				zi += z_step;

				zzi += dzz;
				yyi += dyy;
				xxi += dxx;
			}

			xs += dx_left;
			xe += dx_right;
			zzs += dzz_left;
			zze += dzz_right;
			yys += dyy_left;
			yye += dyy_right;
			xxs += dxx_left;
			xxe += dxx_right;
			us += du_left;
			ue += du_right;
			vs += dv_left;
			ve += dv_right;
		}
	}
	else {
		for (temp_y = y1; temp_y = y3; temp_y++) {
			left = (int)xs;
			right = (int)xe;

			if (left < min_clip_x) {
				left = min_clip_x;
				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x) {
				right = max_clip_x;
				if (left > max_clip_x)
					continue;
			}

			dx_recip = 1.0 / (xe - xs);
			du = (ue - us) * dx_recip;
			dv = (ve - vs) * dx_recip;
			ui = us + du * (left - (int)xs);
			vi = vs + dv * (left - (int)xs);

			for (int i = left; i <= right; i++) {
				col = ui * mat->texture->info.biWidth;
				col = MIN(col, mat->texture->info.biWidth - 1);
				col = MAX(col, 0);
				row = vi * mat->texture->info.biHeight;
				row = MIN(row, mat->texture->info.biWidth - 1);
				row = MAX(row, 0);

				int index = 3 * (row * mat->texture->info.biWidth + col);
				r = mat->texture->arr[index];
				g = mat->texture->arr[index + 1];
				b = mat->texture->arr[index + 2];

				if (r >= 255) r = 255;
				if (g >= 255) g = 255;
				if (b >= 255) b = 255;
				Draw_Pixel(i, temp_y, RGB(r, g, b));
				ui += du;
				vi += dv;
			}
			xs += dx_left;
			xe += dx_right;
			us += du_left;
			ue += du_right;
			vs += dv_left;
			ve += dv_right;
		}
	}
}

void Draw_Textured_Bottom_Tri_Z(int x1, int y1, float xx1, float yy1, float zz1, float u1, float v1,
	int x2, int y2, float xx2, float yy2, float zz2, float u2, float v2,
	int x3, int y3, float xx3, float yy3, float zz3, float u3, float v3, MAT_PTR mat, LIGHT_PTR lights, POLY4D_PTR poly)

{
	float dx_right, dx_left, xs, xe, height, height_recip;
	float du_right, dv_right;
	float du_left, dv_left;
	float us, vs, ue, ve;
	float ui, vi;
	float zi;
	float du, dv, dx_recip; 
	float dzz_left, dzz_right, dzz, temp_zz, zzi;
	float zzs, zze;
	float dxx_left, dxx_right, dxx, temp_xx, xxi;
	float xxs, xxe;
	float dyy_left, dyy_right, dyy, temp_yy, yyi;
	float yys, yye;

	int temp_x, temp_y;
	float temp_u, temp_v;
	int right, left;
	int row, col;
	int r, g, b;
	int r_base, g_base, b_base;


	if (x3 < x2) {
		temp_zz = zz2;
		temp_yy = yy2;
		temp_xx = xx2;
		temp_x = x2;
		temp_u = u2;
		temp_v = v2;
		zz2 = zz3;
		yy2 = yy3;
		xx2 = xx3;
		x2 = x3;
		u2 = u3;
		v2 = v3;
		zz3 = temp_zz;
		yy3 = temp_yy;
		xx3 = temp_xx;
		x3 = temp_x;
		u3 = temp_u;
		v3 = temp_v;
	}

	height = y3 - y1;
	height_recip = 1.0 / height;

	dx_left = (x2 - x1) * height_recip;
	dzz_left = (zz2 - zz1) * height_recip;
	dyy_left = (yy2 - yy1) * height_recip;
	dxx_left = (xx2 - xx1) * height_recip;
	du_left = (u2 - u1) * height_recip;
	dv_left = (v2 - v1) * height_recip;

	dx_right = (x3 - x1) * height_recip;
	dzz_right = (zz3 - zz1) * height_recip;
	dyy_right = (yy3 - yy1) * height_recip;
	dxx_right = (xx3 - xx1) * height_recip;
	du_right = (u3 - u1) * height_recip;
	dv_right = (v3 - v1) * height_recip;

	xs = (float)x1;
	xe = (float)x1 + (float)0.5;
	zzs = (float)zz1;
	zze = (float)zz1 + (float)0.5;
	yys = (float)yy1;
	yye = (float)yy1 + (float)0.5;
	xxs = (float)xx1;
	xxe = (float)xx1 + (float)0.5;
	us = u1;
	vs = v1;
	ue = u1;
	ve = v1;

	if (y1 < min_clip_y) {
		xs = xs + dx_left * (float)(-y1 + min_clip_y);
		xe = xe + dx_right * (float)(-y1 + min_clip_y);
		us = us + du_left * (float)(-y1 + min_clip_y);
		ue = ue + du_right * (float)(-y1 + min_clip_y);
		vs = vs + dv_left * (float)(-y1 + min_clip_y);
		ve = ve + dv_right * (float)(-y1 + min_clip_y);

		y1 = min_clip_y;
	}

	if (y3 > max_clip_y) {
		y3 = max_clip_y;
	}

	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x) {
		for (temp_y = y1; temp_y <= y3; temp_y++) {
			if (temp_y == 300) {
				int m = 1;
			}

			zzi = zzs;
			yyi = yys;
			xxi = xxs;

			dx_recip = 1.0 / (xe - xs);
			du = (ue - us) * dx_recip;
			dv = (ve - vs) * dx_recip;
			dzz = (zze - zzs) * dx_recip;
			dyy = (yye - yys) * dx_recip;
			dxx = (xxe - xxs) * dx_recip;

			float zz3_recip = 1.0 / zz3;
			float zz2_recip = 1.0 / zz2;
			float zz1_recip = 1.0 / zz1;

			float zl = (temp_y - y1) * (zz2_recip - zz1_recip) / (y3 - y1) + zz1_recip;
			float zr = (temp_y - y1) * (zz3_recip - zz1_recip) / (y3 - y1) + zz1_recip;

			float u3_recip = u3 * zz3_recip;
			float u2_recip = u2 * zz2_recip;
			float u1_recip = u1 * zz1_recip;

			float ul = (temp_y - y1) * (u2_recip - u1_recip) / (y3 - y1) + u1_recip;
			float ur = (temp_y - y1) * (u3_recip - u1_recip) / (y3 - y1) + u1_recip;

			float v3_recip = v3 * zz3_recip;
			float v2_recip = v2 * zz2_recip;
			float v1_recip = v1 * zz1_recip;

			float vl = (temp_y - y1) * (v2_recip - v1_recip) / (y3 - y1) + v1_recip;
			float vr = (temp_y - y1) * (v3_recip - v1_recip) / (y3 - y1) + v1_recip;

			float u_step = (ur - ul) / ((int)xe - (int)xs);
			float v_step = (vr - vl) / ((int)xe - (int)xs);
			float z_step = (zr - zl) / ((int)xe - (int)xs);

			ui = ul;
			vi = vl;
			zi = zl;

			for (int i = (int)(xs); i < (int)(xe); i++) {
				if (xe - i <= 1) {
					int m = 0;
				}

				float u = ui / zi;
				float v = vi / zi;

				col = u * mat->texture->info.biWidth;
				col = MIN(col, mat->texture->info.biWidth - 1);
				col = MAX(col, 0);
				row = v * mat->texture->info.biHeight;
				row = MIN(row, mat->texture->info.biWidth - 1);
				row = MAX(row, 0);

				int index = 3 * (row * mat->texture->info.biWidth + col);
				r = mat->texture->arr[index];
				g = mat->texture->arr[index + 1];
				b = mat->texture->arr[index + 2];

				r_base = GetRValue(poly->color);
				g_base = GetGValue(poly->color);
				b_base = GetBValue(poly->color);

				int curr_light = 1;
				if (lights[curr_light].attr & LIGHT_ATTR_POINT) {
					int x = lights[curr_light].pos(0) - xxi;
					int y = lights[curr_light].pos(1) - yyi;
					int z = lights[curr_light].pos(2) - zzi;

					float dist = Fast_Distance(x, y, z);
					float dp = poly->n(0) * x + poly->n(1) * y + poly->n(2) * z;

					if (dp > 0)
					{
					float atten = (lights[curr_light].kc + lights[curr_light].kl*dist + lights[curr_light].kq*dist*dist);
						atten *= dist;
						r += ((GetRValue(lights[curr_light].c_diffuse) * r_base * dp / atten)) / (256);
						g += ((GetGValue(lights[curr_light].c_diffuse) * g_base * dp / atten)) / (256);
						b += ((GetBValue(lights[curr_light].c_diffuse) * b_base * dp / atten)) / (256);
					}
				}

				if (r >= 255) r = 255;
				if (g >= 255) g = 255;
				if (b >= 255) b = 255;

				if (zzi < ZBuffer[i][temp_y]) {
					Draw_Pixel(i, temp_y, RGB(r, g, b));
					ZBuffer[i][temp_y] = zzi;
				}

				ui += u_step;
				vi += v_step;
				zi += z_step;

				zzi += dzz;
				yyi += dyy;
				xxi += dxx;
			}
			xs += dx_left;
			xe += dx_right;
			zzs += dzz_left;
			zze += dzz_right;
			yys += dyy_left;
			yye += dyy_right;
			xxs += dxx_left;
			xxe += dxx_right;
			us += du_left;
			ue += du_right;
			vs += dv_left;
			ve += dv_right;
		}
	}
	else {
		for (temp_y = y1; temp_y < y3; temp_y++) {
			left = (int)xs;
			right = (int)xe;

			if (left < min_clip_x) {
				left = min_clip_x;
				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x) {
				right = max_clip_x;
				if (left > max_clip_x)
					continue;
			}

			dx_recip = 1.0 / (xe - xs);
			du = (ue - us) * dx_recip;
			dv = (ve - vs) * dx_recip;
			ui = us + du * (left - (int)xs);
			vi = vs + dv * (left - (int)xs);

			for (int i = left; i <= right; i++) {
				col = ui * mat->texture->info.biWidth;
				col = MIN(col, mat->texture->info.biWidth - 1);
				col = MAX(col, 0);
				row = vi * mat->texture->info.biHeight;
				row = MIN(row, mat->texture->info.biWidth - 1);
				row = MAX(row, 0);
				if (col >= 64 || col < 0 || row >= 64 || row < 0) {
					int m;
					m = 0;
				}
				int index = 3 * (row * mat->texture->info.biWidth + col);
				r = mat->texture->arr[index];
				g = mat->texture->arr[index + 1];
				b = mat->texture->arr[index + 2];

				if (r >= 255) r = 255;
				if (g >= 255) g = 255;
				if (b >= 255) b = 255;
				Draw_Pixel(i, temp_y, RGB(r, g, b));
				ui += du;
				vi += dv;
			}

			xs += dx_left;
			xe += dx_right;
			us += du_left;
			ue += du_right;
			vs += dv_left;
			ve += dv_right;
		}
	}
}


void Draw_Top_Tri(int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color)
{
	float dx_right, dx_left, xs, xe, height;

	int temp_x, temp_y, right, left;

	if (x2 < x1) {
		temp_x = x2;
		x2 = x1;
		x1 = temp_x;
	} 
	height = y3 - y1;

	dx_left = (x3 - x1) / height;
	dx_right = (x3 - x2) / height;

	xs = (float)x1;
	xe = (float)x2 + (float)0.5;

	if (y1<min_clip_y)
	{
		xs = xs + dx_left*(float)(-y1 + min_clip_y);
		xe = xe + dx_right*(float)(-y1 + min_clip_y);

		y1 = min_clip_y;

	} 

	if (y3>max_clip_y)
		y3 = max_clip_y;

	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x) {
		for (temp_y = y1; temp_y <= y3; temp_y++) {
			for (int i = (int)(xs+0.5); i <= (int)(xe+0.5); i++) {
				Draw_Pixel(i, temp_y, color);
			}
			xs += dx_left;
			xe += dx_right;
		}
	} else {
		for (temp_y = y1; temp_y <= y3; temp_y++) {
			left = (int)xs;
			right = (int)xe;

			xs += dx_left;
			xe += dx_right;

			if (left < min_clip_x) {
				left = min_clip_x;
				if (right < min_clip_x)
					continue;
			}
			if (right > max_clip_x) {
				right = max_clip_x;
				if (left > max_clip_x)
					continue;
			}

			for (int i = left; i <= right; i++) {
				Draw_Pixel(i, temp_y, color);
			}
		}
	}
}

void Draw_Bottom_Tri(int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color)
{
	float dx_right, dx_left, xs, xe, height;

	int temp_x, temp_y, right, left;

	if (x3 < x2) {
		temp_x = x2;
		x2 = x3;
		x3 = temp_x;
	}
	height = y3 - y1;

	dx_left = (x2 - x1) / height;
	dx_right = (x3 - x1) / height;

	xs = (float)x1;
	xe = (float)x1 + (float)0.5;

	if (y1<min_clip_y) {
		xs = xs + dx_left*(float)(-y1 + min_clip_y);
		xe = xe + dx_right*(float)(-y1 + min_clip_y);

		y1 = min_clip_y;
	}

	if (y3>max_clip_y)
		y3 = max_clip_y;

	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x) {
		for (temp_y = y1; temp_y <= y3; temp_y++) {
			for (int i = (int)(xs + 0.5); i <= (int)(xe + 0.5); i++) {
				Draw_Pixel(i, temp_y, color);
			}
			xs += dx_left;
			xe += dx_right;
		}
	} else {
		for (temp_y = y1; temp_y <= y3; temp_y++) {
			left = (int)xs;
			right = (int)xe;

			xs += dx_left;
			xe += dx_right;

			if (left < min_clip_x) {
				left = min_clip_x;

				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x) {
				right = max_clip_x;

				if (left > max_clip_x)
					continue;
			}

			for (int i = left; i <= right; i++) {
				Draw_Pixel(i, temp_y, color);
			}
		}
	}
}

void Draw_Triangle_2D(int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color)
{
	int temp_x, temp_y, new_x;

	// When 3 point is on one line
	if ((x1 == x2 && x2 == x3) || (y1 == y2 && y2 == y3))
		return;

	// Sorting the 3 points in ascending y order
	if (y2 < y1) {
		temp_x = x2;
		temp_y = y2;
		x2 = x1;
		y2 = y1;
		x1 = temp_x;
		y1 = temp_y;
	}
	if (y3 < y1) {
		temp_x = x3;
		temp_y = y3;
		x3 = x1;
		y3 = y1;
		x1 = temp_x;
		y1 = temp_y;
	}
	if (y3 < y2) {
		temp_x = x3;
		temp_y = y3;
		x3 = x2;
		y3 = y2;
		x2 = temp_x;
		y2 = temp_y;
	}

	if (y3<min_clip_y || y1>max_clip_y ||
		(x1<min_clip_x && x2<min_clip_x && x3<min_clip_x) ||
		(x1>max_clip_x && x2>max_clip_x && x3>max_clip_x))
		return;

	if (y1 == y2) {
		Draw_Top_Tri(x1, y1, x2, y2, x3, y3, color);
	} else if (y2 == y3) {
		Draw_Bottom_Tri(x1, y1, x2, y2, x3, y3, color);
	} else {
		new_x = x1 + (int)(0.5 + (float)(y2 - y1) * (float)(x3 - x1) / (float)(y3 - y1));

		Draw_Bottom_Tri(x1, y1, new_x, y2, x2, y2, color);
		Draw_Top_Tri(x2, y2, new_x, y2, x3, y3, color);
	}
}



void Draw_OBJECT4D_Wire(OBJECT4D_PTR obj, HDC hdc)
{
	for (int poly = 0; poly < obj->num_polys; poly++) {
		if (!(obj->plist[poly].state & POLY4D_STATE_ACTIVE) ||
			(obj->plist[poly].state & POLY4D_STATE_CLIPPED) ||
			(obj->plist[poly].state & POLY4D_STATE_BACKFACE))
			continue;

		int vindex0 = obj->plist[poly].vert[0];
		int vindex1 = obj->plist[poly].vert[1];
		int vindex2 = obj->plist[poly].vert[2];

		HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		SelectObject(hdc, pen);

		MoveToEx(hdc, (int)obj->vlist_trans[vindex0](0), (int)obj->vlist_trans[vindex0](1), NULL);
		LineTo(hdc, (int)obj->vlist_trans[vindex1](0), (int)obj->vlist_trans[vindex1](1));

		MoveToEx(hdc, (int)obj->vlist_trans[vindex1](0), (int)obj->vlist_trans[vindex1](1), NULL);
		LineTo(hdc, (int)obj->vlist_trans[vindex2](0), (int)obj->vlist_trans[vindex2](1));

		MoveToEx(hdc, (int)obj->vlist_trans[vindex2](0), (int)obj->vlist_trans[vindex2](1), NULL);
		LineTo(hdc, (int)obj->vlist_trans[vindex0](0), (int)obj->vlist_trans[vindex0](1));
	}
}

void Draw_RENDERLIST4D_Wire(RENDERLIST4D_PTR rend_list)
{
	for (int poly = 0; poly < rend_list->num_polys; poly++) {
		if (!(rend_list->poly_ptrs[poly]->state & POLY4D_STATE_ACTIVE) ||
			(rend_list->poly_ptrs[poly]->state & POLY4D_STATE_CLIPPED) ||
			(rend_list->poly_ptrs[poly]->state & POLY4D_STATE_BACKFACE))
			continue;

		HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		/*SelectObject(hdc, pen);

		MoveToEx(hdc, (int)rend_list->poly_ptrs[poly]->tvlist[0](0), (int)rend_list->poly_ptrs[poly]->tvlist[0](1), NULL);
		LineTo(hdc, (int)rend_list->poly_ptrs[poly]->tvlist[1](0), (int)rend_list->poly_ptrs[poly]->tvlist[1](1));

		MoveToEx(hdc, (int)rend_list->poly_ptrs[poly]->tvlist[1](0), (int)rend_list->poly_ptrs[poly]->tvlist[1](1), NULL);
		LineTo(hdc, (int)rend_list->poly_ptrs[poly]->tvlist[2](0), (int)rend_list->poly_ptrs[poly]->tvlist[2](1));

		MoveToEx(hdc, (int)rend_list->poly_ptrs[poly]->tvlist[2](0), (int)rend_list->poly_ptrs[poly]->tvlist[2](1), NULL);
		LineTo(hdc, (int)rend_list->poly_ptrs[poly]->tvlist[0](0), (int)rend_list->poly_ptrs[poly]->tvlist[0](1));*/
	}
}

void Draw_OBJECT4D_Solid(OBJECT4D_PTR obj, CAM4D_PTR cam, MAT_PTR mat, LIGHT_PTR lights)
{
	glBegin(GL_POINTS);
	int count;
	int r_base, g_base, b_base, color;
	color = RGB(255, 255, 255);
	DWORD start, end;
	DWORD m;
	start = GetTickCount();
	for (int poly = 0; poly < obj->num_polys; poly++) {
		Vector4d t;
		Vector3d u, v, n;

		/*if (!(poly == 8 || poly == 9))
			continue;*/

		if (!(obj->plist[poly].state & POLY4D_STATE_ACTIVE) ||
			(obj->plist[poly].state & POLY4D_STATE_CLIPPED) ||
			(obj->plist[poly].state & POLY4D_STATE_BACKFACE))
			continue;

		

		int vindex0 = obj->plist[poly].vert[0];
		int vindex1 = obj->plist[poly].vert[1];
		int vindex2 = obj->plist[poly].vert[2];

		// calculate the n vector of each poly of object
		t = obj->plist[poly].world_pos[0] - obj->plist[poly].world_pos[1];
		u = Vector3d(t(0), t(1), t(2));

		t = obj->plist[poly].world_pos[0] - obj->plist[poly].world_pos[2];
		v = Vector3d(t(0), t(1), t(2));

		n = u.cross(v);
		obj->plist[poly].n = Vector4d(n(0), n(1), n(2), 0);
		obj->plist[poly].n.normalize();
		
		if (obj->plist[poly].attr & POLY4D_ATTR_SHADE_MODE_FLAT || obj->plist[poly].attr & POLY4D_ATTR_SHADE_MODE_GOURAUD) {
			color = obj->plist[poly].color;
		}
		// texture & lights on point
		start = GetTickCount();
		Draw_Textured_Triangle_Z(obj->vlist_trans[vindex0](0), obj->vlist_trans[vindex0](1), obj->plist[poly].world_pos[0](0), obj->plist[poly].world_pos[0](1), obj->plist[poly].world_pos[0](2), obj->plist[poly].text[0](0), obj->plist[poly].text[0](1),
			obj->vlist_trans[vindex1](0), obj->vlist_trans[vindex1](1), obj->plist[poly].world_pos[1](0), obj->plist[poly].world_pos[1](1), obj->plist[poly].world_pos[1](2), obj->plist[poly].text[1](0), obj->plist[poly].text[1](1),
			obj->vlist_trans[vindex2](0), obj->vlist_trans[vindex2](1), obj->plist[poly].world_pos[2](0), obj->plist[poly].world_pos[2](1), obj->plist[poly].world_pos[2](2), obj->plist[poly].text[2](0), obj->plist[poly].text[2](1),
			&mat[obj->plist[poly].tex_index], lights, &obj->plist[poly]);
		
		// texture & lights on poly
		/*Draw_Textured_Triangle(obj->vlist_trans[vindex0](0), obj->vlist_trans[vindex0](1), obj->plist[poly].text[0](0), obj->plist[poly].text[0](1),
			obj->vlist_trans[vindex1](0), obj->vlist_trans[vindex1](1), obj->plist[poly].text[1](0), obj->plist[poly].text[1](1),
			obj->vlist_trans[vindex2](0), obj->vlist_trans[vindex2](1), obj->plist[poly].text[2](0), obj->plist[poly].text[2](1), 
			&mat[obj->plist[poly].tex_index], lights, obj->plist[poly].color_trans);*/

		// only lights
		/*Draw_Triangle_2D(obj->vlist_trans[vindex0](0), obj->vlist_trans[vindex0](1),
			obj->vlist_trans[vindex1](0), obj->vlist_trans[vindex1](1), 
			obj->vlist_trans[vindex2](0), obj->vlist_trans[vindex2](1), obj->plist[poly].color_trans);*/


	}
	end = GetTickCount();
	m = end - start;
	//printf("%d\n", m);

	glEnd();
}

void Draw_RENDERLIST4D_Solid(RENDERLIST4D_PTR rend_list)
{
	for (int poly = 0; poly < rend_list->num_polys; poly++) {
		if (!(rend_list->poly_ptrs[poly]->state & POLY4D_STATE_ACTIVE) ||
			(rend_list->poly_ptrs[poly]->state & POLY4D_STATE_CLIPPED) ||
			(rend_list->poly_ptrs[poly]->state & POLY4D_STATE_BACKFACE))
			continue;

		Draw_Triangle_2D(rend_list->poly_ptrs[poly]->tvlist[0](0), rend_list->poly_ptrs[poly]->tvlist[0](1),
			rend_list->poly_ptrs[poly]->tvlist[1](0), rend_list->poly_ptrs[poly]->tvlist[1](1),
			rend_list->poly_ptrs[poly]->tvlist[2](0), rend_list->poly_ptrs[poly]->tvlist[2](1), rend_list->poly_ptrs[poly]->color);
	}
}