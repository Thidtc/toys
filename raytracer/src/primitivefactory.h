#ifndef PRIMITIVE_FACTORY_H_
#define PRIMITIVE_FACTORY_H_

#include "primitive.h"

class PrimitiveFactory {
public:
    inline static Primitive *create_light_source(Primitive *prim) {
        if (prim->get_is_light_source()) {
            return prim;
        }
        prim->set_is_light_source(true);
        // Box light
        if (prim->get_type() == Primitive::PRIMITIVE_BOX) {
            Box *p = (Box *)prim; 
            std::vector<double> vec = p->get_grid_ref();
            vec = std::vector<double>(32);
            vec[0] = 1, vec[1] = 2;
            vec[2] = 3, vec[3] = 3;
            vec[4] = 2, vec[5] = 0;
            vec[6] = 0, vec[7] = 1;
            vec[8] = 2, vec[9] = 3;
            vec[10] = 0, vec[11] = 3;
            vec[12] = 0, vec[13] = 0;
            vec[14] = 2, vec[15] = 2;
            vec[16] = 3, vec[17] = 1;
            vec[18] = 1, vec[19] = 3;
            vec[20] = 1, vec[21] = 0;
            vec[22] = 3, vec[23] = 2;
            vec[24] = 2, vec[25] = 1;
            vec[26] = 3, vec[27] = 0;
            vec[28] = 1, vec[29] = 1;
            vec[30] = 0, vec[31] = 2;
            for (int i = 0; i < 16; ++i) {
                vec[2 * i] = vec[2 * i] * p->get_size()[0] / 4 + p->get_pos()[0];
                vec[2 * i + 1] = vec[2 * i + 1] * p->get_size()[2] / 4 + p->get_pos()[2];
            }
        }
        return prim;
    }
};

#endif // PRIMITIVE_FACTORY_H