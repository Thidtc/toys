#include "camera.h"

Vector3d Camera::get_dir(double x, double y) {
    Vector3d dir = target_ - eye_;
    dir.normalize();
    dir = dir * dist_;
    n_x = dir.cross(Vector3d(0, 1, 0));
    n_x.normalize();
    n_y = n_x.cross(Vector3d(1, 0, 0));
    n_y.normalize();
    Vector3d res = dir + (x * ratio_ - 0.5f * width_) * n_x
        + (y * ratio_ - 0.5f * height_) * n_y;
    return res;
}