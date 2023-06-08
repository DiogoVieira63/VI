//
// Created by diogo on 6/7/23.
//

#include "Environment.h"
//
//  perspective.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include "perspective.hpp"

#include <cmath>
#include <cstdio>
Point Lerp(float t, Point p0, Point p1) {
    return (1 - t) * p0 + t * p1;
}


bool Environment::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {



    float theta = M_PI * (y + cam_jitter[1]) /H;
    float phi = M_PI * (x + cam_jitter[0]) / W;
    Vector dir(std::sin(theta) * std::cos(phi), std::cos(theta),
                 std::sin(theta) * std::sin(phi));

    //r->dir = Vector(l1, l2, l3);
    r->dir = dir;
    r->o = Eye;

    return true;
}
