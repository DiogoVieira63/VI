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

    Vector linha1 = Vector(c2w[0][0], c2w[0][1], c2w[0][2]).cross( Vector(xc,yc,1));
    Vector linha2 = Vector(c2w[1][0], c2w[1][1], c2w[1][2]).cross( Vector(xc,yc,1));
    Vector linha3 = Vector(c2w[2][0], c2w[2][1], c2w[2][2]).cross( Vector(xc,yc,1));

    float l1 = linha1.X + linha1.Y + linha1.Z;
    float l2 = linha2.X + linha2.Y + linha2.Z;
    float l3 = linha3.X + linha3.Y + linha3.Z;

    r->dir = Vector(l1, l2, l3);
    r->o = Eye;

    return true;
}
