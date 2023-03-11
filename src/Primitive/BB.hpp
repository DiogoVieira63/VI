//
//  AABB.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef BB_hpp
#define BB_hpp

#include "vector.hpp"
#include "ray.hpp"

// do livro Ray Tracing Gems II
typedef struct BB {
    Point min, max;
    bool intersect (Ray r) {
        Vector invRaydir = Vector(1/r.dir.X,1/r.dir.Y,1/r.dir.Z);
        Vector tLower = r.o.vec2point(min).cross(invRaydir);
        Vector tUpper = r.o.vec2point(max).cross(invRaydir);

        Vector tMins = tLower.min(tUpper);
        Vector tMaxes = tLower.max(tUpper);

        float tMn = tMins.MaxComponent();
        float tMx = tMaxes.MinComponent();

        float tBoxMin = (0.0f < tMn) ? tMn : 0.0f; // deveria haver um r.tMin... por enquanto usei 0
        float tBoxMax = (r.tMax < tMx) ? tMx : r.tMax;

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
