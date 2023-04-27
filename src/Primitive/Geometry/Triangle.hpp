//
// Created by lau on 27-04-2023.
//

#ifndef VI_RT_TRIANGLE_HPP
#define VI_RT_TRIANGLE_HPP


#include "vector"
#include "BB.hpp"
#include "intersection.hpp"
#include "geometry.hpp"

class Triangle: public Geometry {
public:
    Point v1, v2, v3;
    Vector normal,edge1, edge2, edge3;
    BB bb;
    bool intersect (Ray r, Intersection *isect){
        Vector  h, s, q;
        float a,i,u,v;

        h = r.dir.cross(edge2);
        a = edge1.dot(h);
        if (a > -EPSILON && a < EPSILON)
            return false;    // This ray is parallel to this triangle.
        i = 1.0f/a;
        Point ps = r.o - v1;
        s = Vector(ps.X,ps.Y,ps.Z);
        u = i * s.dot(h);
        if (u < 0.0 || u > 1.0)
            return false;
        q = s.cross(edge1);
        v = i * r.dir.dot(q);
        if (v < 0.0 || u + v > 1.0)
            return false;
        // At this stage we can compute t to find out where the intersection point is on the line.
        float t = i * edge2.dot(q);
        if (t > EPSILON) // ray intersection
        {
            isect->p = r.o + (r.dir * t);
            //printf("depth: %f\n", t);
            //printf("Ray direction: %f %f %f\n", r.dir.X, r.dir.Y, r.dir.Z);
            //printf("Ray origin: %f %f %f\n", r.o.X, r.o.Y, r.o.Z);
            //printf("Vertex 0: %f %f %f\n", vertex0.X, vertex0.Y, vertex0.Z);
            //printf("Vertex 1: %f %f %f\n", vertex1.X, vertex1.Y, vertex1.Z);
            //printf("Vertex 2: %f %f %f\n", vertex2.X, vertex2.Y, vertex2.Z);

            Vector wo = -1.f * r.dir;
            // make sure the normal points to the same side of the surface as wo
            normal.Faceforward(wo);
            isect->gn = normal;
            isect->sn = normal;

            isect->depth = t;
            // preencher shading
            isect->wo = Vector(-r.dir.X,-r.dir.Y,-r.dir.Z);
            return true;
        }
        else {// This means that there is a line intersection but not a ray intersection.
            return false;
        }


    }
    Triangle(Point _v1, Point _v2, Point _v3, Vector _normal):
        v1(_v1), v2(_v2), v3(_v3), normal(_normal) {
        edge1 = v1.vec2point(v2); edge2 = v1.vec2point(v3), edge3=v2.vec2point(v3);
        bb.min.set(v1.X, v1.Y, v1.Z); bb.max.set(v1.X, v1.Y, v1.Z);
        bb.update(v2); bb.update(v3);
    }


// https://www.mathopenref.com/heronsformula.html
    float area () {
        float a = edge1.norm();
        float b = edge2.norm();
        float c = edge3.norm();
        float p = (a + b + c)/2;

        return sqrt(p*(p-a)*(p-b)*(p-c));
    }
};


#endif //VI_RT_TRIANGLE_HPP
