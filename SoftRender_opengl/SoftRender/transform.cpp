#include "stdafx.h"
#include "transform.h"
#include "utils.h"

void Transform_RENDERLIST4D(RENDERLIST4D_PTR rend_list, Matrix4d *mt, int coord_select)
{
	switch (coord_select) {
	case TRANSFORM_LOCAL_ONLY: {
		for (int poly = 0; poly < rend_list->num_polys; poly++) {
			POLYF4D_PTR curr_poly = rend_list->poly_ptrs[poly];
			if ((curr_poly == NULL) || (!curr_poly->state & POLY4D_STATE_ACTIVE) ||
				(curr_poly->state & POLY4D_STATE_CLIPPED) ||
				(curr_poly->state & POLY4D_STATE_BACKFACE))
				continue;
			for (int vertex = 0; vertex < 3; vertex++) {
				curr_poly->vlist[vertex] = (*mt) * curr_poly->vlist[vertex];
			}
		}
	}
		break;
	case TRANSFORM_TRANS_ONLY: {
		for (int poly = 0; poly < rend_list->num_polys; poly++) {
			POLYF4D_PTR curr_poly = rend_list->poly_ptrs[poly];
			if ((curr_poly == NULL) || (!curr_poly->state & POLY4D_STATE_ACTIVE) ||
				(curr_poly->state & POLY4D_STATE_CLIPPED) ||
				(curr_poly->state & POLY4D_STATE_BACKFACE))
				continue;
			for (int vertex = 0; vertex < 3; vertex++) {
				curr_poly->tvlist[vertex] = (*mt) * curr_poly->tvlist[vertex];
			}
		}
	}
		break;
	case TRANSFORM_LOCAL_TO_TRANS: {
		for (int poly = 0; poly < rend_list->num_polys; poly++) {
			POLYF4D_PTR curr_poly = rend_list->poly_ptrs[poly];
			if ((curr_poly == NULL) || (!curr_poly->state & POLY4D_STATE_ACTIVE) ||
				(curr_poly->state & POLY4D_STATE_CLIPPED) ||
				(curr_poly->state & POLY4D_STATE_BACKFACE))
				continue;
			for (int vertex = 0; vertex < 3; vertex++) {
				curr_poly->tvlist[vertex] = (*mt) * curr_poly->vlist[vertex];
			}
		}
	}
		break;
	default:
		break;
	}
}

void Transform_OBJECT4D(OBJECT4D_PTR obj, Matrix4d *mt, int coord_select, int transform_basis)
{
	switch (coord_select) {
	case TRANSFORM_LOCAL_ONLY: {
		for (int vertex = 0; vertex < obj->num_vertices; vertex++) {
			obj->vlist_local[vertex] = (*mt) * obj->vlist_local[vertex];
		}
	} break;
	case TRANSFORM_TRANS_ONLY: {
		for (int vertex = 0; vertex < obj->num_vertices; vertex++) {
			obj->vlist_trans[vertex] = (*mt) * obj->vlist_trans[vertex];
		}
	} break;
	case TRANSFORM_LOCAL_TO_TRANS: {
		for (int vertex = 0; vertex < obj->num_vertices; vertex++) {
			obj->vlist_trans[vertex] = (*mt) * obj->vlist_local[vertex];
		}
	} break;
	default:
		break;
	}

	// Transform the local coord of the object
	if (transform_basis) {
		obj->ux = (*mt) * obj->ux;
		obj->uy = (*mt) * obj->uy;
		obj->uz = (*mt) * obj->uz;
	}
}

void Model_To_World_OBJECT4D(OBJECT4D_PTR obj, int coord_select)
{
	if (coord_select == TRANSFORM_LOCAL_TO_TRANS) {
		for (int vertex = 0; vertex < obj->num_vertices; vertex++) {
			obj->vlist_trans[vertex] = obj->vlist_local[vertex] + obj->world_pos;
		}
	} else {
		for (int vertex = 0; vertex < obj->num_vertices; vertex++) {
			obj->vlist_trans[vertex] = obj->vlist_trans[vertex] + obj->world_pos;
			obj->vlist_trans[vertex](3) = 1;
		}
	}
}

void Model_To_World_RENDERLIST4D(RENDERLIST4D_PTR rend_list, Vector4d *pos, int coord_select)
{
	if (coord_select == TRANSFORM_LOCAL_TO_TRANS) {
		for (int poly = 0; poly < rend_list->num_polys; poly++) {
			POLYF4D_PTR curr_poly = rend_list->poly_ptrs[poly];

			if ((curr_poly == NULL) || (!curr_poly->state & POLY4D_STATE_ACTIVE) ||
				(curr_poly->state & POLY4D_STATE_CLIPPED) ||
				(curr_poly->state & POLY4D_STATE_BACKFACE))
				continue;
			for (int vertex = 0; vertex < 3; vertex++) {
				curr_poly->tvlist[vertex] = curr_poly->vlist[vertex] + (*pos);
			}
		}
	} else {
		for (int poly = 0; poly < rend_list->num_polys; poly++) {
			POLYF4D_PTR curr_poly = rend_list->poly_ptrs[poly];

			if ((curr_poly == NULL) || (!curr_poly->state & POLY4D_STATE_ACTIVE) ||
				(curr_poly->state & POLY4D_STATE_CLIPPED) ||
				(curr_poly->state & POLY4D_STATE_BACKFACE))
				continue;
			for (int vertex = 0; vertex < 3; vertex++) {
				curr_poly->tvlist[vertex] = curr_poly->tvlist[vertex] + (*pos);
			}
		}
	}
}

void World_To_Camera_OBJECT4D(CAM4D_PTR cam, OBJECT4D_PTR obj)
{
	for (int vertex = 0; vertex < obj->num_vertices; vertex++) {
		obj->vlist_trans[vertex] = cam->mcam * obj->vlist_trans[vertex];
		obj->vlist_trans[vertex](3) = 1;
	}
}

void World_To_Camera_RENDERLIST4D(CAM4D_PTR cam, RENDERLIST4D_PTR rend_list)
{
	for (int poly = 0; poly < rend_list->num_polys; poly++) {
		POLYF4D_PTR curr_poly = rend_list->poly_ptrs[poly];
		if ((curr_poly == NULL) || (!curr_poly->state & POLY4D_STATE_ACTIVE) ||
			(curr_poly->state & POLY4D_STATE_CLIPPED) ||
			(curr_poly->state & POLY4D_STATE_BACKFACE))
			continue;
		for (int vertex = 0; vertex < 3; vertex++) {
			curr_poly->tvlist[vertex] = cam->mcam * curr_poly->tvlist[vertex];
		}
	}
}

void Camera_To_Perspective_OBJECT4D(OBJECT4D_PTR obj, CAM4D_PTR cam)
{
	for (int vertex = 0; vertex < obj->num_vertices; vertex++) {
		float z = obj->vlist_trans[vertex](2);
		obj->vlist_trans[vertex](0) = cam->view_dist * obj->vlist_trans[vertex](0) / z;
		obj->vlist_trans[vertex](1) = cam->view_dist * obj->vlist_trans[vertex](1) * cam->aspect_ratio / z;
	}
}

void Camera_To_Perspective_RENDERLIST4D(RENDERLIST4D_PTR rend_list, CAM4D_PTR cam)
{
	for (int poly = 0; poly < rend_list->num_polys; poly++) {
		POLYF4D_PTR curr_poly = rend_list->poly_ptrs[poly];
		if ((curr_poly == NULL) || (!curr_poly->state & POLY4D_STATE_ACTIVE) ||
			(curr_poly->state & POLY4D_STATE_CLIPPED) ||
			(curr_poly->state & POLY4D_STATE_BACKFACE))
			continue;
		for (int vertex = 0; vertex < 3; vertex++) {
			float z = curr_poly->tvlist[vertex](2);
			curr_poly->tvlist[vertex](0) = cam->view_dist * curr_poly->tvlist[vertex](0) / z;
			curr_poly->tvlist[vertex](1) = cam->view_dist * curr_poly->tvlist[vertex](1) * cam->aspect_ratio / z;
		}
	}
}

void Perspective_To_Screen_OBJECT4D(OBJECT4D_PTR obj, CAM4D_PTR cam)
{
	float alpha = 0.5 * cam->viewport_width - 0.5;
	float beta = 0.5 * cam->viewport_height - 0.5;

	for (int vertex = 0; vertex < obj->num_vertices; vertex++) {
		obj->vlist_trans[vertex](0) = alpha + alpha * obj->vlist_trans[vertex](0);
		obj->vlist_trans[vertex](1) = alpha + alpha * obj->vlist_trans[vertex](1);
	}
}

void Perspective_To_Screen_RENDERLIST4D(RENDERLIST4D_PTR rend_list, CAM4D_PTR cam)
{
	for (int poly = 0; poly < rend_list->num_polys; poly++) {
		POLYF4D_PTR curr_poly = rend_list->poly_ptrs[poly];
		if ((curr_poly == NULL) || (!curr_poly->state & POLY4D_STATE_ACTIVE) ||
			(curr_poly->state & POLY4D_STATE_CLIPPED) ||
			(curr_poly->state & POLY4D_STATE_BACKFACE))
			continue;

		float alpha = 0.5 * cam->viewport_width - 0.5;
		float beta = 0.5 * cam->viewport_height - 0.5;

		for (int vertex = 0; vertex < 3; vertex++) {
			curr_poly->tvlist[vertex](0) = alpha + alpha * curr_poly->tvlist[vertex](0);
			curr_poly->tvlist[vertex](1) = beta - beta * curr_poly->tvlist[vertex](1);
		}
	}
}

void Camera_To_Perspective_Screen_OBJECT4D(OBJECT4D_PTR obj, CAM4D_PTR cam)
{
	float alpha = 0.5 * cam->viewport_width - 0.5;
	float beta = 0.5 * cam->viewport_height - 0.5;

	for (int vertex = 0; vertex < obj->num_vertices; vertex++) {
		float z = obj->vlist_trans[vertex](2);

		obj->vlist_trans[vertex](0) = cam->view_dist*obj->vlist_trans[vertex](0) / z;
		obj->vlist_trans[vertex](1) = cam->view_dist*obj->vlist_trans[vertex](1) / z;

		obj->vlist_trans[vertex](0) = obj->vlist_trans[vertex](0) + alpha;
		obj->vlist_trans[vertex](1) = -obj->vlist_trans[vertex](1) + beta;

		obj->vlist_trans[vertex](3) = 1;
	}
}

void Camera_Tp_Perspective_Screen_RENDERLIST4D(RENDERLIST4D_PTR rend_list, CAM4D_PTR cam)
{
	for (int poly = 0; poly < rend_list->num_polys; poly++) {
		POLYF4D_PTR curr_poly = rend_list->poly_ptrs[poly];
		if ((curr_poly == NULL) || (!curr_poly->state & POLY4D_STATE_ACTIVE) ||
			(curr_poly->state & POLY4D_STATE_CLIPPED) ||
			(curr_poly->state & POLY4D_STATE_BACKFACE))
			continue;

		float alpha = 0.5 * cam->viewport_width - 0.5;
		float beta = 0.5 * cam->viewport_height - 0.5;

		for (int vertex = 0; vertex < 3; vertex++) {
			float z = curr_poly->tvlist[vertex](2);

			curr_poly->tvlist[vertex](0) = cam->view_dist*curr_poly->tvlist[vertex](0) / z;
			curr_poly->tvlist[vertex](1) = cam->view_dist*curr_poly->tvlist[vertex](1) / z;

			curr_poly->tvlist[vertex](0) = curr_poly->tvlist[vertex](0) + alpha;
			curr_poly->tvlist[vertex](1) = -curr_poly->tvlist[vertex](1) + beta;
		}
	}
}

Matrix4d Build_Model_To_World_Matrix4X4(Vector4d *pos)
{
	Matrix4d m;
	m << 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		(*pos)(0), (*pos)(1), (*pos)(2), 1;
	return m;
}

Matrix4d Build_Camera_Tp_Perspective_MATRIX4X4(CAM4D_PTR cam)
{
	Matrix4d m;
	m << cam->view_dist, 0, 0, 0,
		0, cam->view_dist * cam->aspect_ratio, 0, 0,
		0, 0, 1, 1,
		0, 0, 0, 0;
	return m;
}

Matrix4d Build_Perspective_To_Screen_MATRIX4X4(CAM4D_PTR cam)
{
	float alpha = 0.5 * cam->viewport_width - 0.5;
	float beta = 0.5 * cam->viewport_height - 0.5;

	Matrix4d m;
	m << alpha, 0, 0, 0,
		0, -beta, 0, 0,
		alpha, beta, 1, 0,
		0, 0, 0, 1;
	return m;
}

Matrix4d Build_XYZ_Rotation_MATRIX4X4(float theta_x, float theta_y, float theta_z)
{
	Matrix4d mx, my, mz, mtmp;
	float sin_theta = 0, cos_theta = 0;
	int rot_seq = 0;

	mtmp = Matrix4d::Identity(4, 4);

	if (fabs(theta_x) > EPSILON_E5)
		rot_seq = rot_seq | 1;
	if (fabs(theta_y) > EPSILON_E5)
		rot_seq = rot_seq | 2;
	if (fabs(theta_z) > EPSILON_E5)
		rot_seq = rot_seq | 4;

	switch (rot_seq) {
	case 0:
	{
		return mtmp;
	} break;
	case 1:
	{
		cos_theta = Fast_Cos(theta_x);
		sin_theta = Fast_Sin(theta_x);
		mx << 1, 0, 0, 0,
			0, cos_theta, sin_theta, 0,
			0, -sin_theta, cos_theta, 0,
			0, 0, 0, 1;
		mtmp = mx;
		return mtmp;
	} break;
	case 2:
	{
		cos_theta = Fast_Cos(theta_y);
		sin_theta = Fast_Sin(theta_y);
		my << cos_theta, 0, -sin_theta, 0,
			0, 1, 0, 0,
			sin_theta, 0, cos_theta, 0,
			0, 0, 0, 1;
		mtmp = my;
		return mtmp;
	} break;
	case 3:
	{
		cos_theta = Fast_Cos(theta_x);
		sin_theta = Fast_Sin(theta_x);
		mx << 1, 0, 0, 0,
			0, cos_theta, sin_theta, 0,
			0, -sin_theta, cos_theta, 0,
			0, 0, 0, 1;
		cos_theta = Fast_Cos(theta_y);
		sin_theta = Fast_Sin(theta_y);
		my << cos_theta, 0, -sin_theta, 0,
			0, 1, 0, 0,
			sin_theta, 0, cos_theta, 0,
			0, 0, 0, 1;
		mtmp = my * mx;
		return mtmp;
	} break;
	case 4:
	{
		cos_theta = Fast_Cos(theta_z);
		sin_theta = Fast_Sin(theta_z);
		mz << cos_theta, sin_theta, 0, 0,
			-sin_theta, cos_theta, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;
		mtmp = mz;
		return mtmp;
	} break;
	case 5:
	{
		cos_theta = Fast_Cos(theta_x);
		sin_theta = Fast_Sin(theta_x);
		mx << 1, 0, 0, 0,
			0, cos_theta, sin_theta, 0,
			0, -sin_theta, cos_theta, 0,
			0, 0, 0, 1;
		cos_theta = Fast_Cos(theta_z);
		sin_theta = Fast_Sin(theta_z);
		mz << cos_theta, sin_theta, 0, 0,
			-sin_theta, cos_theta, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;
		mtmp = mz * mx;
		return mtmp;
	} break;
	case 6:
	{
		cos_theta = Fast_Cos(theta_y);
		sin_theta = Fast_Sin(theta_y);
		my << cos_theta, 0, -sin_theta, 0,
			0, 1, 0, 0,
			sin_theta, 0, cos_theta, 0,
			0, 0, 0, 1;
		cos_theta = Fast_Cos(theta_z);
		sin_theta = Fast_Sin(theta_z);
		mz << cos_theta, sin_theta, 0, 0,
			-sin_theta, cos_theta, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;
		mtmp = mz * my;
		return mtmp;
	} break;
	case 7:
	{
		cos_theta = Fast_Cos(theta_x);
		sin_theta = Fast_Sin(theta_x);
		mx << 1, 0, 0, 0,
			0, cos_theta, sin_theta, 0,
			0, -sin_theta, cos_theta, 0,
			0, 0, 0, 1;
		cos_theta = Fast_Cos(theta_y);
		sin_theta = Fast_Sin(theta_y);
		my << cos_theta, 0, -sin_theta, 0,
			0, 1, 0, 0,
			sin_theta, 0, cos_theta, 0,
			0, 0, 0, 1;
		cos_theta = Fast_Cos(theta_z);
		sin_theta = Fast_Sin(theta_z);
		mz << cos_theta, sin_theta, 0, 0,
			-sin_theta, cos_theta, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;
		mtmp = mz * my * mx;
		return mtmp;
	} break;
	default:
		return mtmp;
		break;
	}
}

void Convert_From_Homogeneous4D_OBJECT4D(OBJECT4D_PTR obj)
{
	for (int vertex = 0; vertex < obj->num_vertices; vertex++) {
		float w = obj->vlist_trans[vertex](3);
		obj->vlist_trans[vertex](0) /= w;
		obj->vlist_trans[vertex](1) /= w;
		obj->vlist_trans[vertex](2) /= w;
	}
}

void Convert_From_Homogeneous4d_REDNERLIST4D(RENDERLIST4D_PTR rend_list)
{
	for (int poly = 0; poly < rend_list->num_polys; poly++) {
		POLYF4D_PTR curr_poly = rend_list->poly_ptrs[poly];
		if ((curr_poly == NULL) || (!curr_poly->state & POLY4D_STATE_ACTIVE) ||
			(curr_poly->state & POLY4D_STATE_CLIPPED) ||
			(curr_poly->state & POLY4D_STATE_BACKFACE))
			continue;
		for (int vertex = 0; vertex < 3; vertex++) {
			float w = curr_poly->tvlist[vertex](3);
			curr_poly->tvlist[vertex](0) /= w;
			curr_poly->tvlist[vertex](1) /= w;
			curr_poly->tvlist[vertex](2) /= w;
		}
	}
}