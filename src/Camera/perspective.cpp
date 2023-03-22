//
//  perspective.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include "perspective.hpp"

#include <cmath>
#include <cstdio>

bool Perspective::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {

    float xs, ys, xc, yc;

    //screen space
    xs = (2 * ((float)x + 0.5f))/(float)this->W - 1;
    ys = (2 * ((float)y + 0.5f))/(float)this->H - 1;

    //camera space
    xc = xs * std::tan (this->fovW/2);
    yc = ys * std::tan (this->fovH/2);


    float l1 = c2w[0][0]* xc + c2w[0][1]* yc + c2w[0][2];
    float l2 = c2w[1][0]* xc + c2w[1][1]* yc + c2w[1][2];
    float l3 = c2w[2][0]* xc + c2w[2][1]* yc + c2w[2][2];

    r->dir = Vector(l1, l2, l3);
    r->o = Eye;

    return true;
}
