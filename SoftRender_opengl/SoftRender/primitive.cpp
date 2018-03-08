#include "stdafx.h"
#include "primitive.h"

void PLANE3D_Init(PLANE3D_PTR plane, Vector3d *p0, Vector3d *normal, int normalize = 0)
{
	plane->p0 = (*p0);
	if (!normalize)
		plane->n = (*normal);
	else
		plane->n = normal->normalized();
}

float Compute_OBJECT4D_Radius(OBJECT4D_PTR obj)
{
	obj->avg_radius = 0;
	obj->max_radius = 0;

	for (int vertex = 0; vertex < obj->num_vertices; vertex++) {
		float dist_to_vertex =
			sqrt(obj->vlist_local[vertex](0)*obj->vlist_local[vertex](0) +
			obj->vlist_local[vertex](1)*obj->vlist_local[vertex](1) +
			obj->vlist_local[vertex](2)*obj->vlist_local[vertex](2));

		obj->avg_radius += dist_to_vertex;

		if (dist_to_vertex > obj->max_radius)
			obj->max_radius = dist_to_vertex;
	}

	obj->avg_radius /= obj->num_vertices;

	return obj->max_radius;
}

int Insert_POLY4D_RENDERLIST4D(RENDERLIST4D_PTR rend_list, POLY4D_PTR poly)
{
	if (rend_list->num_polys >= RENDERLIST4D_MAX_POLYS)
		return 0;
	rend_list->poly_ptrs[rend_list->num_polys] = &rend_list->poly_data[rend_list->num_polys];
	rend_list->poly_data[rend_list->num_polys].state = poly->state;
	rend_list->poly_data[rend_list->num_polys].attr = poly->attr;
	rend_list->poly_data[rend_list->num_polys].color = poly->color;

	rend_list->poly_data[rend_list->num_polys].tvlist[0] = poly->vlist[poly->vert[0]];
	rend_list->poly_data[rend_list->num_polys].tvlist[1] = poly->vlist[poly->vert[1]];
	rend_list->poly_data[rend_list->num_polys].tvlist[2] = poly->vlist[poly->vert[2]];

	rend_list->poly_data[rend_list->num_polys].vlist[0] = poly->vlist[poly->vert[0]];
	rend_list->poly_data[rend_list->num_polys].vlist[1] = poly->vlist[poly->vert[1]];
	rend_list->poly_data[rend_list->num_polys].vlist[2] = poly->vlist[poly->vert[2]];

	if (rend_list->num_polys == 0) {
		rend_list->poly_data[0].next = NULL;
		rend_list->poly_data[0].prev = NULL;
	} else {
		rend_list->poly_data[rend_list->num_polys].next = NULL;
		rend_list->poly_data[rend_list->num_polys].prev =
			&rend_list->poly_data[rend_list->num_polys - 1];

		rend_list->poly_data[rend_list->num_polys - 1].next =
			&rend_list->poly_data[rend_list->num_polys];
	}

	rend_list->num_polys++;

	return 1;
}

int Insert_POLYF4D_RENDERLIST4D(RENDERLIST4D_PTR rend_list, POLYF4D_PTR poly)
{
	if (rend_list->num_polys >= RENDERLIST4D_MAX_POLYS)
		return(0);

	rend_list->poly_ptrs[rend_list->num_polys] = &rend_list->poly_data[rend_list->num_polys];
	memcpy((void *)&rend_list->poly_data[rend_list->num_polys], (void *)poly, sizeof(POLYF4D));

	if (rend_list->num_polys == 0) {
		rend_list->poly_data[0].next = NULL;
		rend_list->poly_data[0].prev = NULL;
	} else {
		rend_list->poly_data[rend_list->num_polys].next = NULL;
		rend_list->poly_data[rend_list->num_polys].prev =
			&rend_list->poly_data[rend_list->num_polys - 1];
		rend_list->poly_data[rend_list->num_polys - 1].next =
			&rend_list->poly_data[rend_list->num_polys];
	}
	rend_list->num_polys++;

	return 1;
}
