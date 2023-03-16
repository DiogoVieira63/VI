//
//  perspective.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include "perspective.hpp"

bool Perspective::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {

    float xs, ys, xc, yc;

    //screen space
    xs = (2 * (x + 0.5f))/this->W - 1;
    ys = (2 * (y + 0.5f))/this->H - 1;

    //camera space
    xc = xs * tan (this->fovW/2);
    yc = ys * tan (this->fovH/2);

    r->dir = Vector(xc,yc,1);
    // TODO  -  É suposto multiplicar pelo c2w?
    r->o = Eye;
    return false;
}
