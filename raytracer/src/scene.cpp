#include <vector>
#include <iostream>
#include <cassert>

#include "const.h"
#include "primitive.h"
#include "scene.h"
#include "boundingbox.h"

Scene::~Scene() {
    // Delete all primitives
    for (Primitive *prim : primitives_) {
        if (prim != nullptr) {
            delete prim;
        }
    }
    primitives_.clear();

    // Delete all lights
    lights_.clear();
}

void Scene::init() {
    std::cout << "init" << std::endl;

#ifdef ENABLE_GRID
    build_grid();
#endif
}

void Scene::build_grid() {
    grid_ = new std::vector<Primitive *>[GRID_SIZE * GRID_SIZE * GRID_SIZE];
    // The area of the scene space
    Vector3d p1(-14, -5, -6), p2(14, 8, 30);
    float dx = (p2[0] - p1[0]) / GRID_SIZE, dx_rec = 1.0f / dx;
    float dy = (p2[1] - p1[1]) / GRID_SIZE, dy_rec = 1.0f / dy;
    float dz = (p2[2] - p1[2]) / GRID_SIZE, dz_rec = 1.0f / dz;

    boundingbox_ = BoundingBox(p1, p2 - p1);
    for (int i = 0; i < primitives_.size(); ++i) {
        Primitive *prim = primitives_[i];
        assert(prim->get_is_light_source() == false);
        const BoundingBox &box = prim->get_boundingbox();
        Vector3d bv1 = box.get_pos(), bv2 = box.get_pos() + box.get_size();
        int x1 = (int)((bv1[0] - p1[0]) * dx_rec),
            x2 = (int)((bv2[0] - p1[0]) * dx_rec) + 1; 
        int y1 = (int)((bv1[1] - p1[1]) * dy_rec),
            y2 = (int)((bv2[1] - p1[1]) * dy_rec) + 1;
        int z1 = (int)((bv1[2] - p1[2]) * dz_rec),
            z2 = (int)((bv2[1] - p1[2]) * dz_rec) + 1;
        x1 = std::max(0, x1), x2 = std::min(x2, GRID_SIZE - 1);
        y1 = std::max(0, y1), y2 = std::min(y2, GRID_SIZE - 1);
        z1 = std::max(0, z1), z2 = std::min(z2, GRID_SIZE - 1);

        for (int xi = x1; xi < x2; ++xi)
            for (int yi = y1; yi < y2; ++yi) 
                for (int zi = z1; zi < z2; ++zi) {
                    // BoundingBox for current cell
                    int ind = xi + yi * GRID_SIZE + zi * GRID_SIZE * GRID_SIZE;
                    BoundingBox cell(Vector3d(p1.x + xi * dx, p1.y + yi * dy,
                        p1.z + zi * dz), Vector3d(dx, dy, dz));
                    if (prim->intersect(cell)) {
                        grid_[ind].push_back(prim);
                    }
                }
        
    }
}