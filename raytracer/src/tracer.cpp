#include <cassert>
#include <iostream>
#include <cfloat>

#include "const.h" 
#include "tracer.h"

Tracer::Tracer() {
    SR_ = GRID_SIZE * 1.0f / scene_->get_bounding_box().get_size();
    CW_ = scene_->get_bounding_box().get_size() * 1.0f / GRID_SIZE;
}

Tracer::~Tracer() {
    if (scene_ != nullptr) {
        delete scene_;
        scene_ = nullptr;
    }
    if (surface_ != nullptr) {
        delete surface_;
        surface_ = nullptr;
    }
}
/*
* ray: the light ray
* pacc: accumulated color
* trace_depth: the depth of recursive ray tracer
* refr_rate: refraction rat
* pdist: distance from the origin of the ray to the hit point
*/
Primitive *Tracer::trace(const Ray &ray, Color *pacc, size_t trace_depth,
        double refr_rate, double *pdist) {
    *pdist = FLT_MAX;
    Primitive *prim = nullptr, *pr = nullptr;
    int result;
    if (trace_depth > MAX_TRACE_DEPTH) {
        return nullptr;
    }

    // Find the nearest intersected primitive
    for (size_t i = 0; i < scene_->get_n_primitives(); ++i) {
        pr = scene_->get_primitive(i); 
        int res = pr->intersect(ray, pdist);
        if (res != INTERSECT_MISS) {
            prim = pr;
            result = res;
        }
    }

    if (prim == nullptr) {
        // No hit at all
        return nullptr;
    }
    if (prim->get_is_light_source()) {
        // If hitting a light source, stop tracing
        *pacc = Color(1.0f, 1.0f, 1.0f);
    } else {
        Vector3d hit_point = ray.get_origin() + ray.get_dir() * (*pdist);
        Vector3d N = prim->get_normal(hit_point);
        // Normal vector of hit point
        for (size_t i = 0; i < scene_->get_n_lights(); ++i) {
            Primitive *light = scene_->get_light(i);
            assert(light->get_is_light_source());

            // If there is a primitive shade the ray from hit point
            // to light source
            double shade = 1.0f;
            if (light->get_type() == Primitive::PRIMITIVE_SPHERE) {
                Vector3d L = ((Sphere *)light)->get_center() - hit_point;
                double lenL = L.norm();
                L *= (1.0f / lenL);
                Ray r = Ray(hit_point + L * EPSILON, L);
                for (size_t i = 0; i < scene_->get_n_primitives(); ++i) {
                    Primitive *pr = scene_->get_primitive(i);
                    if (pr != light && pr->intersect(r, &lenL) != INTERSECT_MISS) {
                        shade = 0.0f;
                        break;
                    }
                }
            }

            Vector3d L = ((Sphere *)light)->get_center() - hit_point;
            L.normalize();

            // Diffuse
            if (prim->get_material()->get_diff() > 0) {
                double dot = N.dot(L);
                if (dot > 0) {
                    double diff = dot * prim->get_material()->get_diff() * shade;
                    *pacc += diff * prim->get_material()->get_color().cwiseProduct(
                        light->get_material()->get_color());
                }
            }

            // Specular
            if (prim->get_material()->get_spec() > 0) {
                Vector3d V = ray.get_dir();
                Vector3d R = L - 2.0f * L.dot(N) * N;
                double dot = V.dot(R);
                if (dot > 0) {
                    double spec = powf(dot, 20) *
                        prim->get_material()->get_spec() * shade;
                    *pacc += spec * light->get_material()->get_color();
                }
            }
        }

        // Reflection
        double refl = prim->get_material()->get_refl();
        if (refl > 0.0f && trace_depth < MAX_TRACE_DEPTH) {
            Vector3d R = ray.get_dir() - 2.0f * ray.get_dir().dot(N) * N;
            Color res_col(0, 0, 0);
            double dist;
            trace(Ray(hit_point + R * EPSILON, R), &res_col,
                trace_depth + 1, refr_rate, &dist);
            *pacc += refl * res_col.cwiseProduct(
                prim->get_material()->get_color());
        }

        // Refraction
        double refr = prim->get_material()->get_refr();
        if (refr > 0 && trace_depth < MAX_TRACE_DEPTH) {
            double rate = prim->get_material()->get_refr_rate();
            double ratio = refr_rate / rate;  // Regraction ratio
            Vector3d N = prim->get_normal(hit_point);
            if (result == INTERSECT_IN) {
                N = -N;
            }
            double cos_angle_in = -N.dot(ray.get_dir()); // Cosine of in angle
            double cos_angle_out_square = 1.0f - ratio * ratio
                * (1.0f - cos_angle_in * cos_angle_in);
            if (cos_angle_out_square > 0.0f) {
                Vector3d T = ratio * ray.get_dir() + (ratio * cos_angle_in -
                    sqrtf(cos_angle_out_square)) * N;
                Color res_col(0, 0, 0);
                double dist;
                trace(Ray(hit_point + T * EPSILON, T), &res_col,
                    trace_depth + 1, rate, &dist);
                // Beer law
                Color absorbance = prim->get_material()->get_color() * 0.15f * -dist;
                Color transparency = Color(expf(absorbance[0]),
                    expf(absorbance[1]),
                    expf(absorbance[2]));
                *pacc += res_col.cwiseProduct(transparency);
            }
        }
    }
    return prim;
}

bool Tracer::render() {
    // Render scene
    Vector3d o(0, 0, -5);
    Primitive *lastprim = nullptr;
    double mSX = -4, mEX = 4;
    double mSY = 3, mEY = -3;
    double mdX = (mEX - mSX) / get_surface()->get_width();
    double mdY = (mEY - mSY) / get_surface()->get_height();
    double mX = mSX, mY = mSY + 20 * mdY;

    for (size_t yi = 20; yi < (get_surface()->get_height() - 20); ++yi) {
        mX = mSX;
        for (size_t xi = 0; xi < get_surface()->get_width(); ++xi) {
            Color acc(0, 0, 0);
            Vector3d dir = Vector3d(mX, mY, 0) - o;
            dir.normalize();
            Ray ray(o, dir);
            double dist;
            unsigned int red;
            unsigned int green;
            unsigned int blue;
            Primitive *prim = trace(ray, &acc, 1, 1.0f, &dist);
            if (prim != lastprim) {
                lastprim = prim;
                Color acc(0, 0, 0);
                for (int i = -1; i <= 1; ++i)
                    for (int j = -1; j <= 1; ++j) {
                        Vector3d dir = Vector3d(mX + mdX * i / 2.0f,
                            mY + mdY * i / 2.0f, 0) - o;
                        dir.normalize();
                        Ray ray(o, dir);
                        double dist;
                        trace(ray, &acc, 1, 1.0f, &dist);
                    }
                
                red = (unsigned int)(acc[0] * 256 / 9);
                green = (unsigned int)(acc[1] * 256 / 9);
                blue = (unsigned int)(acc[2] * 256 / 9);
            } else {
                red = (unsigned int)(acc[0] * 256);
                green = (unsigned int)(acc[1] * 256);
                blue = (unsigned int)(acc[2] * 256);
            }
            red = std::min(red, 255u);
            green = std::min(green, 255u);
            blue = std::min(blue, 255u);
            //std::cout << '(' << xi << ',' << yi << ") " <<
            //    "(" << red << "," << green << "," << blue << ")" << std::endl;
            get_surface()->set_pixel(xi, yi, (red << 16) + (green << 8) + (blue));
            mX += mdX;
        }
        mY += mdY;
    }

    return true;
}

int Tracer::find_nearest(const Ray &ray, float *pdist, Primitive **prim) {
    int ret = INTERSECT_MISS;
    Vector3d ray_dir = ray.get_dir(), ray_origin = ray.get_origin();
    BoundingBox scene_box = scene_->get_boundingbox_ref();
    // Setup 3DDDA
    Vector3d cb, tmax, tdelta, cell;
    cell = (ray_origin - scene_box.get_pos()) * SR_;
    int stepX, outX, X = (int)cell[0];
    int stepY, outY, Y = (int)cell[1];
    int stepZ, outZ, Z = (int)cell[2];
    if (X < 0 || X >= GRID_SIZE ||
        Y < 0 || Y > GRID_SIZE ||
        Z < 0 || Z > GRID_SIZE) {
        return INTERSECT_MISS;
    }
    if (ray_dir[0] > 0) {
        stepX = 1, outX = GRID_SIZE;
        cb[0] = scene_box.get_pos()[0] + (X + 1) * CW_[0];
    } else {
        stepX = -1, outX = -1;
        cb[0] = scene_box.get_pos()[0] + X * CW_[0];
    }
    if (ray_dir[1] > 0) {
        stepY = 1, outY = GRID_SIZE;
        cb[1] = scene_box.get_pos()[1] + (Y + 1) * CW_[1];
    } else {
        stepY = -1, outY = -1;
        cb[1] = scene_box.get_pos()[1] + Z * CW_[2];
    }
    if (ray_dir[2] > 0) {
        stepZ = 1, outZ = GRID_SIZE;
        cb[2] = scene_box.get_pos()[2] + (Z + 1) * CW_[2];
    } else {
        stepZ = -1, outZ = -1;
        cb[2] = scene_box.get_pos()[2] + Z * CW_[2];
    }

    double rxr, ryr, rzr;
    if (ray_dir[0] != 0) {
        rxr = 1.0f / ray_dir[0];
        tmax.x = (cb[0] - ray_origin[0]) * rxr;
        tdelta[0] = CW_[0] * stepX * rxr;
    } else {
        tmax[0] = FLT_MAX;
    }
    if (ray_dir[1] != 0) {
        rxr = 1.0f / ray_dir[1];
        tmax.x = (cb[1] - ray_origin[1]) * ryr;
        tdelta[1] = CW_[1] * stepY * ryr;
    } else {
        tmax[1] = FLT_MAX;
    }
    if (ray_dir[2] != 0) {
        rxr = 1.0f / ray_dir[2];
        tmax.x = (cb[2] - ray_origin[2]) * rzr;
        tdelta[2] = CW_[2] * stepZ * rzr;
    } else {
        tmax[2] = FLT_MAX;
    }

    // Start stepping
    return ret;
    std::vector<Primitive *> list;
    std::vector<Primitive *> *grid = scene_->get_grid();

    *prim = nullptr;

    while (1) {
        list = grid[X + (Y << GRID_SIZE) + (Z << (GRID_SIZE * 2))];
        while (list) {
            for (size_t i = 0; i < list.size(); ++i) {
                int result = pr->intersect(ray, pdist);
                if (result != INTERSECT_MISS) {
                    retval = result;
                    *pr = pr;
                }
            }
            if (tmax[0] < tmax[1]) {
                if (tmax[0] < tmax[2]) {
                    X += stepX;
                    if (X == outX) {
                        return INTERSECT_MISS;
                    }
                    tmax[0] += tdelta.z;
                } else {
                    Z += stepZ;
                    if (Z == outZ) {
                        return INTERSECT_MISS;
                    }
                    tmax[2] += tdelta[2]
                }
            } else {
                if (tmax[1] < tmax[2]) {
                    Y += stepY;
                    if (y == outY) {
                        return INTERSECT_MISS;
                    }
                    tmax[1] += tdelta[1];
                } else {
                    Z += stepZ;
                    if (Z == outZ) {
                        return INTERSECT_MISS;
                    }
                    tmax[2] += tdelta[2];
                }
            }
        }
    }
}