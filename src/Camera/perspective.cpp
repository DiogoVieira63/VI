//
//  perspective.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include "perspective.hpp"

bool Perspective::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {

    float xs, ys, xc, yc, angleh, anglew;

    //screen space
    xs = (2 * (x + 0.5f))/this->W - 1;
    ys = (2 * (y + 0.5f))/this->W - 1;

    //camera space
    xc = xs * tan (anglew/2);
    yc = ys * tan (angleh/2);

    // TODO
    r->o = Eye;
    return false;
}
