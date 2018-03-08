#ifndef TRAYCER_H_
#define TRAYCER_H_

#include <cstdlib>
#include <cfloat>

#include "primitive.h"
#include "scene.h"
#include "surface.h"
#include "material.h"

class Tracer {
public:
    Tracer();
    ~Tracer();

    // Set the target render surface(that is, the rendering target) of the tracer
    inline Surface *get_surface() const { return surface_; }
    inline void set_surface(Surface *surface) { surface_ = surface; }
    // Set the target render scene of the tracer
    inline Scene *get_scene() const { return scene_; }
    inline void set_scene(Scene *scene) { scene_ = scene; }

    bool render();

private:
    // Trace the ray and get the accumulative color
    // Args:
    //      ray: the target ray
    //      pacc: pointer to the acuumulative color
    //      trace_depth: the depth of the ray traycing procedure
    //      RIndex:
    //      pdist: pointer to the nearest primitive distance
    // Return:
    //      the nearest primitive, or nullptr if no intersected primitves
    Primitive *trace(const Ray &ray, Color *pacc, size_t trace_depth,
        double RIndex, double *pdist);
    
    // Find the nearest primitive for the specified ray
    // Args:
    //      ray: the target ray
    //      pdist: the distance from the origin of ray to hit point
    //      pprim: pointer of pointer to the nearest primitive
    // Return:
    //      the hit status
    int find_nearest(const Ray &ray, float *pdist, Primitive **pprim);

private:
    Scene *scene_;
    Surface *surface_;

    // The reciprocal of size of the bounding box of scene (# of grid)
    Vector3d SR_;
    // The size of the bounding box of scene(# of grid)
    Vector3d CW_;
};

#endif // TRAYCER_H_