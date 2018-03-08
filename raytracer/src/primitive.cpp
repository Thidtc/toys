#include <string.h>
#include <Eigen/Dense>

#include "const.h"
#include "ray.h"
#include "primitive.h"

using namespace Eigen;


// Sphere
int Sphere::intersect(const Ray &ray, double *pdist) {
    Vector3d v = center_ - ray.get_origin();
    double d = v.dot(ray.get_dir());
    int retval = INTERSECT_MISS;
    double dif = d * d - v.dot(v) + sq_radius_;
    // Intersect
    if (dif > 0) {
        double sq_dif = sqrt(dif);
        double d1 = d - sq_dif; 
        double d2 = d + sq_dif;
        if (d2 > 0) {
            if (d1 < 0) {
                // The origin of ray is inside the shpere
                if (d2 < *pdist) {
                    *pdist = d2;
                    retval = INTERSECT_IN;
                }
            } else {
                if (d1 < *pdist) {
                    *pdist = d1;
                    retval = INTERSECT_HIT;
                }
            }
        }
    }

    return retval;
}

bool Sphere::intersect_box(const BoundingBox &box) {
    double dmin = 0;
    Vector3d v1 = box.get_pos(), v2 = box.get_pos() + box.get_size();
    // X-axis
    if (center_[0] < v1[0]) {
        dmin += (center_[0] - v1[0]) * (center_[0] - v1[0]);
    } else if (center_[0] > v2[0]) {
        dmin += (center_[0] - v2[0]) * (center_[0] - v2[0]);
    }
    // Y-axis
    if (center_[1] < v1[1]) {
        dmin += (center_[1] - v1[1]) * (center_[1] - v1[1]);
    } else if (center_[1] > v2[1]) {
        dmin += (center_[1] - v2[1]) * (center_[1] - v2[1]);
    }
    // Z-axis
    if (center_[2] < v1[2]) {
        dmin += (center_[2] - v1[2]) * (center_[2] - v1[2]);
    } else if (center_[2] > v2[2]) {
        dmin += (center_[2] - v2[2]) * (center_[2] - v2[2]);
    }

    return dmin <= sq_radius_;
}


// Plane
int Plane::intersect(const Ray &ray, double *pdist) {
    // The cosine value of angle between ray direction vector 
    // and plane normal vector
    double d = plane_.N.dot(ray.get_dir());
    if (!ALMOST_EQUAL(d, 0)) {
        double dist = -(plane_.D + plane_.N.dot(ray.get_origin())) / d;
        if (dist > 0) {
            if (dist < *pdist) {
                *pdist  = dist;
                return INTERSECT_HIT;
            }
        }
    }
    return INTERSECT_MISS;
}

bool Plane::intersect_box(const BoundingBox &box) {
    Vector3d v[2];
    v[0] = box.get_pos(), v[1] = box.get_pos() + box.get_size();
    int side1 = 0, side2 = 0;
    for (int i = 0; i < 8; i++) {
        Vector3d p(v[i & 1][0], v[(i >> 1) & 1][1], v[(i >> 2) & 1][2]);
        if (p.dot(plane_.N) + plane_.D < 0) {
            side1++;
        } else {
            side2++;
        }
    }
    return side1 > 0 && side2 > 0;
}

// Box
Vector3d Box::get_normal(const Vector3d &pos) {
    double dist[6];
    dist[0] = fabs(box_.get_size()[0] - pos[0]);
    dist[1] = fabs(box_.get_size()[0] + box_.get_size()[0] - pos[0]);
    dist[2] = fabs(box_.get_size()[1] - pos[1]);
    dist[3] = fabs(box_.get_size()[1] + box_.get_size()[1] - pos[1]);
    dist[4] = fabs(box_.get_size()[2] - pos[0]);
    dist[5] = fabs(box_.get_size()[2] + box_.get_size()[2] - pos[2]);

    int ind = 0;
    double short_dist = dist[0];
    for (int i = 1; i < 6; ++i) {
        if (dist[i] < short_dist) {
            short_dist = dist[i];
            ind = i;
        }
    }
    switch (ind) {
        case 0: return Vector3d(-1, 0, 0);
        case 1: return Vector3d(1, 0, 0);
        case 2: return Vector3d(0, -1, 0);
        case 3: return Vector3d(0, 1, 0);
        case 4: return Vector3d(0, 0, -1);
        case 5: return Vector3d(0, 0, 1);
        default: return Vector3d(0, 0, 0);
    }
}

int Box::intersect(const Ray &ray, double *pdist) {
    double dist[6] = {0};
    Vector3d hit_point[6];
    Vector3d d = ray.get_dir();
    Vector3d o = ray.get_origin();
    Vector3d v1 = box_.get_pos();
    Vector3d v2 = box_.get_pos() + box_.get_size();
    int retval = INTERSECT_MISS;
    // X-axis
    if (d[0]) {
        float rc = 1.0f / d[0];
        dist[0] = (v1[0] -  o[0]) * rc;
        dist[3] = (v2[0] - o[0]) * rc;
    }
    // Y-axis
    if (d[1]) {
        float rc = 1.0f / d[1];
        dist[1] = (v1[1] - o[1]) * rc;
        dist[4] = (v2[1] - o[1]) * rc;
    }
    // Z-axis
    if (d[2]) {
        float rc = 1.0f / d[2];
        dist[2] = (v1[2] - o[2]) * rc;
        dist[5] = (v2[2] - o[2]) * rc;
    }
    for (int i = 0; i < 6; ++i) {
        if (dist[i] > 0) {
            hit_point[i] = o + dist[i] * d;
            if ((hit_point[i][0] > (v1[0] - EPSILON)) && (hit_point[i][0] < (v2[0] + EPSILON))
                && (hit_point[i][1] > (v1[1] - EPSILON)) && (hit_point[i][1] < (v2[1] + EPSILON))
                && (hit_point[i][2] > (v1[2] - EPSILON)) && (hit_point[i][2] < (v2[2] + EPSILON))) {
                    if (dist[i] < *pdist) {
                        *pdist = dist[i];
                        retval = INTERSECT_HIT;
                    }
                }
        }
    }
    return retval;
}
