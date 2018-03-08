#include "stdafx.h"
#include "camera.h"
#include "utils.h"


void Init_CAM4D(CAM4D_PTR cam,
	int cam_attr,
	Vector4d *cam_pos,
	Vector4d *cam_dir,
	Vector4d *cam_target,
	float near_clip_z,
	float far_clip_z,
	float fov,
	float viewport_width,
	float viewport_height)
{
	cam->attr = cam_attr;

	cam->pos = (*cam_pos);
	cam->dir = (*cam_dir);

	cam->u = Vector4d(1, 0, 0, 1);
	cam->v = Vector4d(0, 1, 0, 1);
	cam->n = Vector4d(0, 0, 1, 1);

	if (cam_target != NULL) {
		cam->target = (*cam_target);
	} else {
		cam->target = Vector4d(0, 0, 0, 1);
	}

	cam->near_clip_z = near_clip_z;
	cam->far_clip_z = far_clip_z;

	cam->viewport_width = viewport_width;
	cam->viewport_height = viewport_height;

	cam->viewport_center_x = (viewport_width - 1) / 2;
	cam->viewport_center_y = (viewport_height - 1) / 2;

	cam->aspect_ratio = (float)viewport_width / (float)viewport_height;

	// Init matrixs to be identity matrix
	cam->mcam = Matrix4d::Identity(4, 4);
	cam->mper = Matrix4d::Identity(4, 4);
	cam->mscr = Matrix4d::Identity(4, 4);

	cam->fov = fov;

	cam->viewplane_width = 2.0;
	cam->viewplane_height = 2.0f / cam->aspect_ratio;

	float tan_fov_div2 = tan(DEG_TO_RAD(fov / 2));
	cam->view_dist = 0.5 * (cam->viewplane_width) * tan_fov_div2;

	if (fov == 90.0) {
		Vector3d pt_origin = Vector3d(0, 0, 0);
		Vector3d vn;

		// Init clip plane
		vn = Vector3d(1, 0, -1);
		PLANE3D_Init(&cam->rt_clip_plane, &pt_origin, &vn, 1);
		vn = Vector3d(-1, 0, -1);
		PLANE3D_Init(&cam->lt_clip_plane, &pt_origin, &vn, 1);
		vn = Vector3d(0, 1, -1);
		PLANE3D_Init(&cam->tp_clip_plane, &pt_origin, &vn, 1);
		vn = Vector3d(0, -1, -1);
		PLANE3D_Init(&cam->bt_clip_plane, &pt_origin, &vn, 1);
	} else {
		Vector3d pt_origin = Vector3d(0, 0, 0);
		Vector3d vn;

		// Init clip plane
		vn = Vector3d(cam->view_dist, 0, -cam->viewplane_width / 2.0);
		PLANE3D_Init(&cam->rt_clip_plane, &pt_origin, &vn, 1);
		vn = Vector3d(-cam->view_dist, 0, -cam->viewplane_width / 2.0);
		PLANE3D_Init(&cam->lt_clip_plane, &pt_origin, &vn, 1);
		vn = Vector3d(0, cam->view_dist, -cam->viewplane_width / 2.0);
		PLANE3D_Init(&cam->tp_clip_plane, &pt_origin, &vn, 1);
		vn = Vector3d(0, -cam->view_dist, -cam->viewplane_width / 2.0);
		PLANE3D_Init(&cam->bt_clip_plane, &pt_origin, &vn, 1);
	}
}

void Build_CAM4D_Matrix_Euler(CAM4D_PTR cam, int cam_rot_seq)
{
	Matrix4d mt_inv,	// camera transpose
		mx_inv,			// camera rotate around x-axis
		my_inv,
		mz_inv,
		mrot;			// product of all rotation;

	mt_inv << 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-cam->pos(0), -cam->pos(1), -cam->pos(2), 1;

	float theta_x = cam->dir(0);
	float theta_y = cam->dir(1);
	float theta_z = cam->dir(2);

	float cos_theta = Fast_Cos(theta_x);
	float sin_theta = -Fast_Sin(theta_x);
	mx_inv << 1, 0, 0, 0,
		0, cos_theta, sin_theta, 0,
		0, -sin_theta, cos_theta, 0,
		0, 0, 0, 1;

	cos_theta = Fast_Cos(theta_y);
	sin_theta = -Fast_Sin(theta_y);
	my_inv << cos_theta, 0, -sin_theta, 0,
		0, 1, 0, 0,
		sin_theta, 0, cos_theta, 0,
		0, 0, 0, 1;

	cos_theta = Fast_Cos(theta_z);
	sin_theta = -Fast_Sin(theta_z);
	mz_inv << cos_theta, sin_theta, 0, 0,
		-sin_theta, cos_theta, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;

	switch (cam_rot_seq)
	{
	case CAM_ROT_SEQ_XYZ: {
		mrot = mz_inv * my_inv * mx_inv;
	} break;
	case CAM_ROT_SEQ_YXZ: {
		mrot = mz_inv * mx_inv * my_inv;
	} break;
	case CAM_ROT_SEQ_XZY: {
		mrot = my_inv * mz_inv * mx_inv;
	} break;
	case CAM_ROT_SEQ_YZX: {
		mrot = mx_inv * mz_inv * my_inv;
	} break;
	case CAM_ROT_SEQ_ZYX: {
		mrot = mx_inv * my_inv * mz_inv;
	} break;
	case CAM_ROT_SEQ_ZXY: {
		mrot = my_inv * mx_inv * mz_inv;
	} break;
	default:
		break;
	}
	cam->mcam = mrot * mt_inv;
}

void Build_CAM4D_Matrix_UVN(CAM4D_PTR cam, int mode)
{
	Matrix4d mt_inv,
		mt_uvn;

	mt_inv << 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0.
		- cam->pos(0), -cam->pos(1), -cam->pos(2), 1;

	if (mode == UVN_MODE_SPHERICAL) {
		float phi = cam->dir(0);
		float theta = cam->dir(1);

		float sin_phi = Fast_Sin(phi);
		float cos_phi = Fast_Cos(phi);
		float sin_theta = Fast_Sin(theta);
		float cos_theta = Fast_Cos(theta);

		cam->target(0) = -1 * sin_phi * sin_theta;
		cam->target(1) = 1 * cos_phi;
		cam->target(2) = 1 * sin_phi * cos_theta;
	}

	cam->n = cam->target - cam->pos;
	Vector3d t1, t2, t3;
	t1 = Vector3d(cam->n(0), cam->n(1), cam->n(2));
	t2 = Vector3d(0, 1, 0);
	t3 = t2.cross(t1);
	t2 = t1.cross(t3);

	cam->u = Vector4d(t3(0), t3(1), t3(2), 1);
	cam->v = Vector4d(t2(0), t2(1), t2(2), 1);

	cam->u.normalize();
	cam->v.normalize();
	cam->n.normalize();

	mt_uvn << cam->u(0), cam->v(0), cam->n(0), 0,
		cam->u(1), cam->v(1), cam->n(1), 0,
		cam->u(2), cam->v(2), cam->n(2), 0,
		0, 0, 0, 1;
	cam->mcam = mt_inv * mt_uvn;
}


int Cull_OBJECT4D(OBJECT4D_PTR obj, CAM4D_PTR cam, int cull_flags)
{
	Vector4d sphere_pos;
	sphere_pos = cam->mcam * obj->world_pos;

	if (cull_flags & CULL_OBJECT_Z_PLANE) {
		if (((sphere_pos(2) - obj->max_radius) > cam->far_clip_z) ||
			((sphere_pos(2) + obj->max_radius) < cam->near_clip_z)) {
			obj->state |= OBJECT4D_STATE_CULLED;
			return 1;
		}
	}
	if (cull_flags & CULL_OBJECT_X_PLANE) {
		float z_test = (0.5) * cam->viewplane_width * sphere_pos(2) / cam->view_dist;
		if (((sphere_pos(0) - obj->max_radius) > z_test) ||
			((sphere_pos(0) + obj->max_radius) < -z_test)) {
			obj->state |= OBJECT4D_STATE_CULLED;
			return 1;
		}
	}

	if (cull_flags & CULL_OBJECT_Y_PLANE) {
		float z_test = (0.5)*cam->viewplane_height*sphere_pos(2) / cam->view_dist;

		if (((sphere_pos(1) - obj->max_radius) > z_test) ||
			((sphere_pos(1) + obj->max_radius) < -z_test))  {
			obj->state |= OBJECT4D_STATE_CULLED;
			return 1;
		}
	}

	return 0;
}

void Reset_OBJECT4D(OBJECT4D_PTR obj)
{
	obj->state &= ~OBJECT4D_STATE_CULLED;
	for (int poly = 0; poly < obj->num_polys; poly++) {
		POLY4D_PTR curr_poly = &obj->plist[poly];
		/*if (!(curr_poly->state & POLY4D_STATE_ACTIVE))
			continue;
		curr_poly->state &= ~POLY4D_STATE_ACTIVE;
		curr_poly->state &= ~POLY4D_STATE_BACKFACE;*/
		curr_poly->state &= ~POLY4D_STATE_BACKFACE;
	}
}

void Reset_RENDERLIST4D(RENDERLIST4D_PTR rend_list)
{
	rend_list->num_polys = 0;
}

void Remove_Backfaces_OBJECT4D(OBJECT4D_PTR obj, CAM4D_PTR cam)
{
	if (obj->state & OBJECT4D_STATE_CULLED)
		return;
	int count = 0;
	int count2 = 0;
	for (int poly = 0; poly < obj->num_polys; poly++) {
		POLY4D_PTR curr_poly = &obj->plist[poly];

		if (!(curr_poly->state & POLY4D_STATE_ACTIVE) ||
			(curr_poly->state & POLY4D_STATE_CLIPPED) ||
			(curr_poly->attr & POLY4D_ATTR_2SIDED) ||
			(curr_poly->state & POLY4D_STATE_BACKFACE)) {
			continue;
		}

		count2++;
		int vindex0 = curr_poly->vert[0];
		int vindex1 = curr_poly->vert[1];
		int vindex2 = curr_poly->vert[2];

		Vector4d t;
		Vector3d u, v, n;

		t = obj->vlist_trans[vindex0] - obj->vlist_trans[vindex1];
		u = Vector3d(t(0), t(1), t(2));

		t = obj->vlist_trans[vindex0] - obj->vlist_trans[vindex2];
		v = Vector3d(t(0), t(1), t(2));

		n = u.cross(v);

		Vector4d view;
		//view = obj->vlist_trans[vindex0] - cam->pos;
		view = Vector4d(0, 0, 1, 0);
		float dp = n(0)*view(0) + n(1)*view(1) + n(2)*view(2);

		if (dp >= 0.0) {
			curr_poly->state |= POLY4D_STATE_BACKFACE;
		}
	}
}

void Remove_Backfaces_RENDERLIST4D(RENDERLIST4D_PTR rend_list, CAM4D_PTR cam)
{
	for (int poly = 0; poly < rend_list->num_polys; poly++) {
		POLYF4D_PTR curr_poly = rend_list->poly_ptrs[poly];
		if ((curr_poly == NULL) || (!curr_poly->state & POLY4D_STATE_ACTIVE) ||
			(curr_poly->state & POLY4D_STATE_CLIPPED) ||
			(curr_poly->state & POLY4D_ATTR_2SIDED) ||
			(curr_poly->state & POLY4D_STATE_BACKFACE))
			continue;

		Vector4d t;
		Vector3d u, v, n;

		t = curr_poly->tvlist[0] - curr_poly->tvlist[1];
		u = Vector3d(t(0), t(1), t(2));

		t = curr_poly->tvlist[0] - curr_poly->tvlist[2];
		v = Vector3d(t(0), t(1), t(2));

		n = u.cross(v);

		Vector4d view;
		view = curr_poly->tvlist[0] - cam->pos;
		float dp = n(0)*view(0) + n(1)*view(1) + n(2)*view(2);

		if (dp <= 0.0)
			curr_poly->state |= POLY4D_STATE_BACKFACE;
	}
}