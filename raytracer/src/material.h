#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <Eigen/Dense>
#include "const.h"

using namespace Eigen;

typedef Vector3d Color;

class Material {
public:
    Material() : color_(Color(0.2f, 0.2f, 0.2f)),
        coe_refl_(0.0f),
        coe_diff_(0.2f),
        coe_refr_rate_(1.5f),
        coe_spec_(0.8f) {}
    inline void set_color(const Color & color) { color_ = color; }
    inline Color get_color() const { return color_; }
    inline void set_refl(double refl) { coe_refl_ = refl; }
    inline double get_refl() const { return coe_refl_; }
    inline void set_diff(double diff) { coe_diff_ = diff; }
    inline double get_diff() const { return coe_diff_; }
    inline void set_spec(double spec) { coe_spec_ = spec; } 
    inline double get_spec() const { return coe_spec_; }
    inline void set_refr(double refr) { coe_refr_ = refr; }
    inline double get_refr() const { return coe_refr_; }
    inline void set_refr_rate(double rate) { coe_refr_rate_ = rate; }
    inline double get_refr_rate() const { return coe_refr_rate_; }

public:
    Color emission;
    enum ReflType {
        DIFF = 1,
        SPEC,
        REFR,
    };
    ReflType BRDFType_;

private:
    Color color_; // 材质颜色
    double coe_refl_; // 反射系数
    double coe_refr_; // 折射系数
    double coe_diff_; // 漫反射系数
    double coe_refr_rate_; // 折射率
    double coe_spec_; // 高光

};



#endif // MATERIAL_H_