#ifndef CAMERA_H_
#define CAMERA_H_

#include <Eigen/Dense>
#include "const.h"

using namespace Eigen;

class Camera {
public:
    Camera() : eye_(0.0f, 0.0f, -5.0f),
        target_(0.0f, 0.0f, 0.0f),
        dist_(5.0f),
        width_(8.0f),
        height_(6.0f),
        ratio_(0.01f) {}
    Camera(const Vector3d &eye, const Vector3d &target,
        double dist, double width, double height,
        double ratio) : eye_(eye),
        target_(target),
        dist_(dist),
        width_(width),
        height_(height),
        ratio_(ratio) {}
    inline Vector3d get_eye() const { return eye_; }
    inline Vector3d get_target() const { return target_; }
    inline Vector3d &get_eye_ref() { return eye_; }
    inline Vector3d &get_target_ref() { return target_; }
    inline double get_dist() const { return dist_; }
    inline double get_width() const { return width_; }
    inline double get_height() const { return height_; }
    inline double get_ratio() const { return ratio_; }
    inline void set_RV(double R, double V) { R_ = R; V_ = V; }

    // Get direction from camera to the specified pixel
    // Params:
    //      x, y : the coordination of the pixel in the picture
    // Return:
    //      the direction from camera to the pixel in the space
    Vector3d get_dir(double x, double y);
private:
    Vector3d eye_; // Position of camera
    Vector3d target_; // Position of target
    double dist_; // Distance from the eye to the 
    double width_; // The width of the projection plane corresponding to the picture
    double height_;
    Vector3d n_x; // X-axis of projection plane
    Vector3d n_y; // Y-axis of projection plane
    double ratio_; // The ratio between the projection plane and target picture
    double R_; // Radius of aperture
    double V_; // 
};

#endif // CAMERA_H_