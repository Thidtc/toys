#ifndef RAY_H_
#define RAY_H_

#include <Eigen/Dense>
#include "const.h"

using namespace Eigen;

// ray
class Ray {
public:
    Ray() : origin_(Vector3d(0, 0, 0)),
        dir_(Vector3d(0, 0, 0)) {}
    Ray(const Vector3d &origin, const Vector3d &dir) : origin_(origin),
        dir_(dir) {}
    inline void set_origin(const Vector3d &origin) { origin_ = origin; }
    inline Vector3d get_origin() const { return origin_; }
    inline void set_dir(const Vector3d &dir) { dir_ = dir; }
    inline Vector3d get_dir() const { return dir_; }
private:
    Vector3d origin_;
    Vector3d dir_;
};

#endif // RAY_H_