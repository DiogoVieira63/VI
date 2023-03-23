//
//  AABB.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef BB_hpp
#define BB_hpp
#include <iostream>

#include "vector.hpp"
#include "ray.hpp"

// do livro Ray Tracing Gems II
typedef struct BB {
    Point min, max;
    bool intersect (Ray r) {
        Vector invRaydir = Vector(1/r.dir.X,1/r.dir.Y,1/r.dir.Z);
        Vector tLower = r.o.vec2point(min);
        Vector tUpper = r.o.vec2point(max);

        tLower = Vector(tLower.X * invRaydir.X,tLower.Y * invRaydir.Y,tLower.Z * invRaydir.Z);
        tUpper = Vector(tUpper.X * invRaydir.X,tUpper.Y * invRaydir.Y,tUpper.Z * invRaydir.Z);

        float tBoxMin = tLower.MaxComponent();
        float tBoxMax = tUpper.MinComponent();

        return tBoxMin <= tBoxMax;

    }
    void update (Point p) {
        if (p.X < min.X) min.X = p.X;
        else if (p.X > max.X) max.X = p.X;
        if (p.Y < min.Y) min.Y = p.Y;
        else if (p.Y > max.Y) max.Y = p.Y;
        if (p.Z < min.Z) min.Z = p.Z;
        else if (p.Z > max.Z) max.Z = p.Z;
    }
} BB;

#endif /* AABB_hpp */
