//
//  mesh.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#include "mesh.hpp"
#include <iostream>
/*
void Mesh::GetUVs(Point uv[3])  {
    Mesh *mesh = new Mesh();
    if (mesh->uv) {
        uv[0] = mesh->uv[v[0]];
        uv[1] = mesh->uv[v[1]];
        uv[2] = mesh->uv[v[2]];
    } else {
        uv[0] = Point(0, 0);
        uv[1] = Point(1, 0);
        uv[2] = Point(1, 1);
    }
}
*/
// see pbrt book (3rd ed.), sec 3.6.2, pag 2

bool Mesh::TriangleIntersect (Ray r, Face f, Intersection *isect) {

    if(!f.bb.intersect(r)) {
        return false;  // pp do prof
    }
    // print ray
    const float EPSILON = 0.0000001;
    Point vertex0 = this->vertices[f.vert_ndx[0]];
    Point vertex1 = this->vertices[f.vert_ndx[1]];
    Point vertex2 = this->vertices[f.vert_ndx[2]];

    Vector edge1, edge2, h, s, q;
    float a,i,u,v;
    edge1 = vertex1.vec2point(vertex0);
    edge2 = vertex2.vec2point(vertex0);


    h = r.dir.cross(edge2);
    a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    i = 1.0f/a;
    Point ps = r.o - vertex0;
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
        isect->gn = f.geoNormal;
        // preencher shading
        isect->wo = Vector(-r.dir.X,-r.dir.Y,-r.dir.Z);
        return true;
    }
    else {// This means that there is a line intersection but not a ray intersection.
        printf("4\n");
        return false;
    }
}


bool Mesh::intersect (Ray r, Intersection *isect) {
    bool intersect = true, intersect_this_face;
    Intersection min_isect, curr_isect;
    float min_depth=MAXFLOAT;
    // intersect the ray with the mesh BB
    
    if (!intersect) return false;
    
    // If it intersects then loop through the faces
    intersect = false;
    for (auto face_it=faces.begin() ; face_it != faces.end() ; face_it++) {
        intersect_this_face = TriangleIntersect(r, *face_it, &curr_isect);
        if (!intersect_this_face) continue;
        
        intersect = true;
        if (curr_isect.depth < min_depth) {  // this is closer
            min_depth = curr_isect.depth;
            min_isect = curr_isect;
        }
    }
    
    return intersect;
}

int Mesh::getIndexVertices(Point K){
    int index =0;
    for (auto it = vertices.begin(); it != vertices.end(); it++){
        if (it->equals(K)) {
            return index;
        }
        index++;
    }
    return -1;
}

void Mesh::addVertice(Point p){
    vertices.push_back(p);
    numVertices++;
}

