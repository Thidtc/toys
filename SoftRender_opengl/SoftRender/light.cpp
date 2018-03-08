#include "stdafx.h"
#include "light.h"
#include "utils.h"

LIGHT lights[MAX_LIGHTS];
int num_lights = 0;

int Init_Light_LIGHT(
	int index,
	int _state,
	int _attr,
	int _c_ambient,
	int _c_diffuse,
	int _c_specular,
	Vector4d *_pos,
	Vector4d *_dir,
	float _kc,
	float _kl,
	float _kq,
	float _spot_inner,
	float _spot_outer,
	float _pf)
{
	if (index < 0 || index >= MAX_LIGHTS)
		return 0;

	lights[index].state = _state;
	lights[index].id = index;
	lights[index].attr = _attr;
	lights[index].c_ambient = _c_ambient;
	lights[index].c_diffuse = _c_diffuse;
	lights[index].c_specular = _c_specular;
	lights[index].kc = _kc;
	lights[index].kl = _kl;
	lights[index].kq = _kq;
	if (_pos != NULL)
		lights[index].pos = *_pos;
	if (_dir != NULL)
		lights[index].dir = _dir->normalized();
	lights[index].spot_inner = _spot_inner;
	lights[index].spot_outer = _spot_outer;
	lights[index].pf = _pf;

	return index;
}

int Light_OBJECT4D_World(OBJECT4D_PTR obj,
	CAM4D_PTR cam,
	LIGHT_PTR lights,
	int max_lights)
{
	unsigned int r_base = 0, g_base = 0, b_base = 0,
		r_sum, g_sum, b_sum,
		shaded_color;

	float dp, dist, i, nl, atten;

	if (!(obj->state & OBJECT4D_STATE_ACTIVE) ||
		(obj->state & OBJECT4D_STATE_CULLED) ||
		!(obj->state & OBJECT4D_STATE_VISIBLE))
		return 0;

	for (int poly = 0; poly < obj->num_polys; poly++) {
		POLY4D_PTR curr_poly = &obj->plist[poly];

		if (!(curr_poly->state & POLY4D_STATE_ACTIVE) ||
			(curr_poly->state & POLY4D_STATE_CLIPPED) ||
			(curr_poly->state & POLY4D_STATE_BACKFACE))
			continue;

		int vindex0 = curr_poly->vert[0];
		int vindex1 = curr_poly->vert[1];
		int vindex2 = curr_poly->vert[2];

		if (curr_poly->attr & POLY4D_ATTR_SHADE_MODE_FLAT || curr_poly->attr & POLY4D_ATTR_SHADE_MODE_GOURAUD) {
			r_base = GetRValue(curr_poly->color);
			g_base = GetGValue(curr_poly->color);
			b_base = GetBValue(curr_poly->color);
		}

		r_sum = 0;
		g_sum = 0;
		b_sum = 0;

		for (int curr_light = 0; curr_light < max_lights; curr_light++) {
			if (lights[curr_light].state == LIGHT_STATE_OFF)
				continue;

			if (lights[curr_light].attr & LIGHT_ATTR_AMBIENT) {
				r_sum += ((GetRValue(lights[curr_light].c_ambient)) * r_base) / 256;
				g_sum += ((GetGValue(lights[curr_light].c_ambient)) * g_base) / 256;
				b_sum += ((GetBValue(lights[curr_light].c_ambient)) * b_base) / 256;
			} else if (lights[curr_light].attr & LIGHT_ATTR_INFINITE) {
				Vector4d t;
				Vector3d u, v, n;

				t = obj->vlist_trans[vindex0] - obj->vlist_trans[vindex1];
				u = Vector3d(t(0), t(1), t(2));

				t = obj->vlist_trans[vindex0] - obj->vlist_trans[vindex2];
				v = Vector3d(t(0), t(1), t(2));

				n = u.cross(v);

				nl = Fast_Distance_3D(&n);

				dp = n(0) * lights[curr_light].dir(0) + n(1) * lights[curr_light].dir(1) + n(2) * lights[curr_light].dir(2);

				if (dp > 0) {
					i = 128 * dp / nl;
					r_sum += ((GetRValue(lights[curr_light].c_diffuse) * r_base * i)) / (256 * 128);
					g_sum += ((GetGValue(lights[curr_light].c_diffuse) * g_base * i)) / (256 * 128);
					b_sum += ((GetBValue(lights[curr_light].c_diffuse) * b_base * i)) / (256 * 128);
				}
			} else if (lights[curr_light].attr & LIGHT_ATTR_POINT) {
				Vector4d t, l;
				Vector3d u, v, n;

				t = obj->vlist_trans[vindex0] - obj->vlist_trans[vindex1];
				u = Vector3d(t(0), t(1), t(2));

				t = obj->vlist_trans[vindex0] - obj->vlist_trans[vindex2];
				v = Vector3d(t(0), t(1), t(2));

				n = u.cross(v);

				nl = Fast_Distance_3D(&n);

				l = obj->vlist_trans[vindex0] - lights[curr_light].pos;

				dist = Fast_Distance_4D(&l);

				dp = n(0) * l(0) + n(1) * l(1) + n(2) * l(2);


				if (dp > 0)
				{
					atten = (lights[curr_light].kc + lights[curr_light].kl*dist + lights[curr_light].kq*dist*dist);

					i = 128 * dp / (nl * dist * atten);

					r_sum += ((GetRValue(lights[curr_light].c_diffuse) * r_base * i)) / (256 * 128);
					g_sum += ((GetGValue(lights[curr_light].c_diffuse) * g_base * i)) / (256 * 128);
					b_sum += ((GetBValue(lights[curr_light].c_diffuse) * b_base * i)) / (256 * 128);
				}
			} else if (lights[curr_light].attr & LIGHT_ATTR_SPOTLIGHT1) {

			} else if (lights[curr_light].attr & LIGHT_ATTR_SPOTLIGHT2) {
				Vector4d t, l;
				Vector3d u, v, n;

				t = obj->vlist_trans[vindex0] - obj->vlist_trans[vindex1];
				u = Vector3d(t(0), t(1), t(2));

				t = obj->vlist_trans[vindex0] - obj->vlist_trans[vindex2];
				v = Vector3d(t(0), t(1), t(2));

				n = u.cross(v);

				nl = Fast_Distance_3D(&n);

				dp = n(0) * lights[curr_light].dir(0) + n(1) * lights[curr_light].dir(1) + n(2) * lights[curr_light].dir(2);

				if (dp > 0) {
					l = obj->vlist_trans[vindex0] - lights[curr_light].pos;
					dist = Fast_Distance_4D(&l);
					float dpsl = l(0) * lights[curr_light].dir(0) + l(0) * lights[curr_light].dir(0) + l(0) * lights[curr_light].dir(0);
					dpsl /= dist;

					if (dpsl > 0) {
						float atten = lights[curr_light].kc + lights[curr_light].kl * dist + lights[curr_light].kq * dist * dist;

						float dpsl_exp = dpsl;
						for (int e_index = 1; e_index < (int)lights[curr_light].pf; e_index++)
							dpsl_exp *= dpsl;

						int i = 128 * dp * dpsl_exp / (nl * atten);

						r_sum += ((GetRValue(lights[curr_light].c_diffuse) * r_base * i)) / (256 * 128);
						g_sum += ((GetGValue(lights[curr_light].c_diffuse) * g_base * i)) / (256 * 128);
						b_sum += ((GetBValue(lights[curr_light].c_diffuse) * b_base * i)) / (256 * 128);
					}
				}
			}
		}
		if (r_sum > 255) r_sum = 255;
		if (g_sum > 255) g_sum = 255;
		if (b_sum > 255) b_sum = 255;

		curr_poly->color_trans = RGB(r_sum, g_sum, b_sum);
	}

	return 1;
}