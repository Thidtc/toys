#ifndef BOUNDING_BOX_
#define BOUNDING_BOX_

#include <Eigen/Dense>

#include "ray.h"

using namespace Eigen;

class BoundingBox {
public:
    BoundingBox() : pos_(Vector3d(0.0f, 0.0f, 0.0f)),
        size_(Vector3d(0.0f, 0.0f, 0.0f)) {};
    BoundingBox(const Vector3d &pos, const Vector3d &size) :
        pos_(pos),
        size_(size) {};

    bool intersect(const Vector3d &pos);
    bool intersect(const BoundingBox &box);

    inline Vector3d get_pos() const { return pos_; }
    inline Vector3d get_size() const { return size_; }
private:
    Vector3d pos_;
    Vector3d size_;
};

#endif // BOUNDING_BOX_