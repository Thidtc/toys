#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <cctype>

#include "primitive.h"
#include "boundingbox.h"

class Scene {
public:
    Scene() {}
    ~Scene();

    inline size_t get_n_primitives() const { return primitives_.size(); }
    inline size_t get_n_lights() const { return lights_.size(); }
    inline std::vector<Primitive *> &get_primitives_ref() { return primitives_; }
    inline Primitive *get_primitive(size_t ind) { return primitives_[ind]; }
    inline Primitive *get_light(size_t ind) { return primitives_[ind]; }
    inline BoundingBox &get_boundingbox_ref() { return boundingbox_; }
    inline void add_primitive(Primitive *prim) { primitives_.push_back(prim); }
    inline void add_light(Primitive *light) {
        lights_.push_back(light);
    }
    inline std::vector<Primitive *> *grid get_grid() const { return grid_; }

private:
    void init();
    void init_light();
    // Build grid for the scene
    void build_grid();
private:
    std::vector<Primitive *> primitives_;
    std::vector<Primitive *> lights_;
    // Bounding box of the scene
    BoundingBox boundingbox_; 
    // Grid for the scene
    std::vector<Primitive *> *grid_;
};

#endif // SCENE_H_