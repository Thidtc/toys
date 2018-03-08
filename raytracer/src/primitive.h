#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include <Eigen/Dense>
#include <string>
#include <vector>

#include "ray.h"
#include "material.h"
#include "boundingbox.h"

using namespace Eigen;

// Primitive
// Note that the light source is also treated as a Primitive
class Primitive {
public:
    enum{
        PRIMITIVE_LIGHT = 0,
        PRIMITIVE_SPHERE = 1,
        PRIMITIVE_PLANE,
        PRIMITIVE_BOX,
    };
public:
    Primitive() : name_(""),
        is_light_source_(false),
        material_(new Material()) {}
    virtual ~Primitive() {
        if (material_ != nullptr) {
            delete material_;
        }
    }
    inline Material *get_material() { return material_; }
    inline void set_material(Material *material) {
        if (material_ != nullptr) {
            delete material_;
        }
        material_ = material;
    }
    inline std::string get_name() const { return name_; }
    inline void set_name(const std::string & name) { name_ = name; }
    virtual Color get_color() const { return material_->get_color(); }

    // Get type of primitive
    virtual int get_type() const = 0;
    // Get normal vector of a point of the primitive
    virtual Vector3d get_normal(const Vector3d &pos) = 0;
    // Get the distance from the origin of ray and the hit intersection point
    virtual int intersect(const Ray &ray, double *pdist) = 0;
    // Get the bounding box
    virtual bool intersect_box(const BoundingBox &box) = 0;

    inline bool get_is_light_source() const { return is_light_source_; }
    inline void set_is_light_source(bool is_light_source) {
        is_light_source_ = is_light_source;
    }
    
private:
    std::string name_; // Name of the primitive
    bool is_light_source_; // Whether this is a light source or not
    Material *material_; // Material of the primitive
};

class Sphere : public Primitive {
public:
    Sphere(const Vector3d &center, double radius) : center_(center),
        radius_(radius),
        sq_radius_(radius * radius),
        rec_radius_(1.0 / radius_) {}

    // Override
    int get_type() const { return PRIMITIVE_SPHERE; }
    Vector3d get_normal(const Vector3d &pos) { return (pos - center_) * rec_radius_; }
    int intersect(const Ray &ray, double *pdist);
    bool intersect_box(const BoundingBox &box);

    inline Vector3d get_center() const { return center_; }
    inline double get_radius() const { return radius_; }
    inline double get_sq_radius() const { return sq_radius_; }
    inline double get_rec_radius() const { return rec_radius_; }


private:
    Vector3d center_;
    double radius_;
    double sq_radius_; // Squared radius
    double rec_radius_; // Reciprocal radius
};

class Plane : public Primitive {
public:
    struct plane {
        plane() : N(0.0f, 0.0f, 0.0f), D(0.0f) {}
        plane(const Vector3d &normal, double depth) :
            N(normal), D(depth) {}
        Vector3d N;
        double D;
    };
    Plane(const Vector3d &normal, double depth) :
        plane_(normal, depth) {}

    // Override
    inline int get_type() const { return PRIMITIVE_PLANE; }
    Vector3d get_normal(const Vector3d &pos) { return plane_.N; }
    int intersect(const Ray &ray, double *pdist);
    bool intersect_box(const BoundingBox &box);

    inline plane &get_plane_ref() { return plane_; }
    inline double get_D() const { return plane_.D; }
    
private:
    plane plane_;
};

class Box : public Primitive {
public:
    Box() : box_(Vector3d(0, 0, 0), Vector3d(0, 0, 0)) {}
    Box(const BoundingBox &box) : box_(box) {}

    // Override
    inline int get_type() const { return PRIMITIVE_BOX; }
    Vector3d get_normal(const Vector3d &pos);
    int intersect(const Ray &ray, double *pdist);
    bool intersect_box(const BoundingBox &box) { return box_.intersect(box); }

    inline Vector3d get_pos() const { return box_.get_pos(); }
    inline Vector3d get_size() const { return box_.get_size(); }
    inline std::vector<double> &get_grid_ref() { return grid_; }

    inline bool intersect(const BoundingBox &box) { return box_.intersect(box); }
    inline bool intersect(const Vector3d &pos) { return box_.intersect(pos); }

private:
    BoundingBox box_;
    std::vector<double> grid_;
};

#endif // PRIMITIVE_H_