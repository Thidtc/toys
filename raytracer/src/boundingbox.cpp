#include "boundingbox.h"

//Vector3d BoundingBox::get_normal(const Vector3d &pos) {
//    return Vector3d(0.0f, 0.0f, 0.0f);
//}

bool BoundingBox::intersect(const Vector3d &pos) {
    return pos[0] > pos_[0] && pos[0] < pos_[0] + size_[0] &&
        pos[1] > pos_[1] && pos[1] < pos_[1] + size_[1] &&
        pos[2] > pos_[2] && pos[2] < pos_[2] + size_[2];
}

bool BoundingBox::intersect(const BoundingBox &box) {
    Vector3d pos1 = box.get_pos();
    Vector3d size1 = box.get_size();
    return pos1[0] < pos_[0] + size_[0] && pos1[0] + size1[0] > pos_[0] &&
        pos1[1] < pos_[1] + size_[1] && pos1[1] + size1[1] > pos_[1] &&
        pos1[2] < pos_[2] + size_[2] && pos1[2] + size1[2] > pos_[2];
}
