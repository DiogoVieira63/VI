 //
//  AmbientShader.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "AmbientShader.hpp"
#include "Phong.hpp"

RGB AmbientShader::shade(bool intersected, Intersection isect) {
    RGB color(0.,0.,0.);

    // if no intersection, return background
    if (!intersected) {
        return (background);
    }


    auto *f = (Phong *)isect.f;
    // if no material, return background

    if (f->Ka.isZero()) return color;
    RGB Ka = f->Ka;

    // ambient shade
    // Loop over scene's light sources and process Ambient Lights
    for (auto light_itr= scene->lights.begin() ; light_itr != scene->lights.end() ; light_itr++) {
        Point p_dummy;
        // is it an ambient light ?
        if (!(*light_itr)->isAmbient) continue;
        color += Ka * (*light_itr)->L(p_dummy);

        /* atualização prof - ns se é para tirar o p_dummy
        if ((*light_itr)->type == AMBIENT_LIGHT) {  // is it an ambient light ?
            color += Ka * (*light_itr)->L();
            continue;
        }
         */
    }

    return color;
};
