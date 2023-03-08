//
//  mesh.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#include "mesh.hpp"
#include <iostream>
/*
void GetUVs(Point uv[3])  {
    if (mesh->uv) {
        uv[0] = mesh->uv[v[0]];
        uv[1] = mesh->uv[v[1]];
        uv[2] = mesh->uv[v[2]];
    } else {
        uv[0] = Point2f(0, 0);
        uv[1] = Point2f(1, 0);
        uv[2] = Point2f(1, 1);
    }
}
 */
// see pbrt book (3rd ed.), sec 3.6.2, pag 2

bool Mesh::TriangleIntersect (Ray r, Face f, Intersection *isect) {
    /*Get triangle vertices in p0, p1, and p2 157
      Perform ray–triangle intersection test 158
        〈Transform triangle vertices to ray coordinate space 158〉
            〈Translate vertices based on ray origin 158〉
            〈Permute components of triangle vertices and ray direction 159〉
            〈Apply shear transformation to translated vertex positions 159〉
        〈Compute edge function coefficients e0, e1, and e2 161〉
        〈Fall back to double-precision test at triangle edges〉
        〈Perform triangle edge and determinant tests 162〉
        〈Compute scaled hit distance to triangle and test against ray t range 162〉
        〈Compute barycentric coordinates and t value for triangle intersection 163〉
        〈Ensure that computed triangle t is conservatively greater than zero 234
            〈Compute δz term for triangle t error bounds 234〉
            〈Compute δx and δy terms for triangle t error bounds 234〉
            〈Compute δe term for triangle t error bounds 235〉
            〈Compute δt term for triangle t error bounds and check t 235〉
    Compute triangle partial derivatives 164
    Compute error bounds for triangle intersection 227
    Interpolate (u, v) parametric coordinates and hit point 164
    Test intersection against alpha texture, if present 165
    Fill in SurfaceInteraction from triangle hit 165*/

    Point p0 = this->vertices[f.vert_ndx[0]];
    Point p1 = this->vertices[f.vert_ndx[1]];
    Point p2 = this->vertices[f.vert_ndx[2]];

    Point p0t = p0 - r.o;
    Point p1t = p1 - r.o;
    Point p2t = p2 - r.o;

    int kz = r.dir.Abs().MaxDimension();
    int kx = kz + 1; if (kx == 3) kx = 0;
    int ky = kx + 1; if (ky == 3) ky = 0;
    Vector d = r.dir.Permute(kx, ky, kz);
    p0t = p0t.Permute(kx, ky, kz);
    p1t = p1t.Permute(kx, ky, kz);
    p2t = p2t.Permute(kx, ky, kz);

    float Sx = -r.dir.X / r.dir.Z;
    float Sy = -r.dir.Y / r.dir.Z;
    float Sz = 1.f / r.dir.Z;
    p0t.X += Sx * p0t.Z;
    p0t.Y += Sy * p0t.Z;
    p1t.X += Sx * p1t.Z;
    p1t.Y += Sy * p1t.Z;
    p2t.X += Sx * p2t.Z;
    p2t.Y += Sy * p2t.Z;

    float e0 = p1t.X * p2t.Y - p1t.Y * p2t.X;
    float e1 = p2t.X * p0t.Y - p2t.Y * p0t.X;
    float e2 = p0t.X * p1t.Y - p0t.Y * p1t.X;

    //double f0 = p1t.X * p2t.Y - p1t.Y * p2t.X;
    //double f1 = p2t.X * p0t.Y - p2t.Y * p0t.X;
    //double f2 = p0t.X * p1t.Y - p0t.Y * p1t.X;

    if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0))
        return false;
    float det = e0 + e1 + e2;
    if (det == 0)
        return false;

    p0t.Z *= Sz;
    p1t.Z *= Sz;
    p2t.Z *= Sz;
    float tScaled = e0 * p0t.Z + e1 * p1t.Z + e2 * p2t.Z;
    if (det < 0 && (tScaled >= 0 || tScaled < r.tMax * det))
        return false;
    else if (det > 0 && (tScaled <= 0 || tScaled > r.tMax * det))
        return false;

    float invDet = 1 / det;
    float b0 = e0 * invDet;
    float b1 = e1 * invDet;
    float b2 = e2 * invDet;
    float t = tScaled * invDet;

    float maxZt = ((Vector(p0t.Z, p1t.Z, p2t.Z)).Abs()).MaxComponent();
    float deltaZ = float(gamma(3) * maxZt);
    float maxXt = ((Vector(p0t.X, p1t.X, p2t.X)).Abs()).MaxComponent();
    float maxYt = ((Vector(p0t.Y, p1t.Y, p2t.Y)).Abs()).MaxComponent();
    float deltaX = float(gamma(5) * (maxXt + maxZt));
    float deltaY = float(gamma(5) * (maxYt + maxZt));

    float deltaE = 2.0 * (gamma(2) * maxXt * maxYt + deltaY * maxXt + deltaX * maxYt);
    float maxE = Vector(e0, e1, e2).Abs().MaxComponent();
    float deltaT = 3.0 * (gamma(3) * maxE * maxZt + deltaE * maxZt +
                        deltaZ * maxE) * std::abs(invDet);

    if (t <= deltaT)
        return false;

    /*
    Vector dpdu, dpdv;
    Point uv[3];
    GetUVs(uv);
    Vector2f duv02 = uv[0] - uv[2], duv12 = uv[1] - uv[2];
    Vector3f dp02 = p0 - p2, dp12 = p1 - p2;
    Float determinant = duv02[0] * duv12[1] - duv02[1] * duv12[0];
    if (determinant == 0) {
        CoordinateSystem(Normalize(Cross(p2 - p0, p1 - p0)), &dpdu, &dpdv);
    } else {
        Float invdet = 1 / determinant;
        dpdu = ( duv12[1] * dp02 - duv02[1] * dp12) * invdet;
        dpdv = (-duv12[0] * dp02 + duv02[0] * dp12) * invdet;
    }

    Float xAbsSum = (std::abs(b0 * p0.x) + std::abs(b1 * p1.x) +
                     std::abs(b2 * p2.x));
    Float yAbsSum = (std::abs(b0 * p0.y) + std::abs(b1 * p1.y) +
                     std::abs(b2 * p2.y));
    Float zAbsSum = (std::abs(b0 * p0.z) + std::abs(b1 * p1.z) +
                     std::abs(b2 * p2.z));
    Vector3f pError = gamma(7) * Vector3f(xAbsSum, yAbsSum, zAbsSum);

    Point3f pHit = b0 * p0 + b1 * p1 + b2 * p2;
    Point2f uvHit = b0 * uv[0] + b1 * uv[1] + b2 * uv[2];

    if (testAlphaTexture && mesh->alphaMask) {
        SurfaceInteraction isectLocal(pHit, Vector3f(0,0,0), uvHit,
                                      Vector3f(0,0,0), dpdu, dpdv, Normal3f(0,0,0), Normal3f(0,0,0),
                                      ray.time, this);
        if (mesh->alphaMask->Evaluate(isectLocal) == 0)
            return false;
    }

    *isect = SurfaceInteraction(pHit, pError, uvHit, -ray.d, dpdu, dpdv,
                                Normal3f(0, 0, 0), Normal3f(0, 0, 0), ray.time, this);
    isect->n = isect->shading.n = Normal3f(Normalize(Cross(dp02, dp12)));
    if (mesh->n || mesh->s) {
        Normal3f ns;
        if (mesh->n) ns = Normalize(b0 * mesh->n[v[0]] +
                                    b1 * mesh->n[v[1]] +
                                    b2 * mesh->n[v[2]]);
        else
            ns = isect->n;

        Vector3f ss;
        if (mesh->s) ss = Normalize(b0 * mesh->s[v[0]] +
                                    b1 * mesh->s[v[1]] +
                                    b2 * mesh->s[v[2]]);
        else
            ss = Normalize(isect->dpdu);

        Vector3f ts = Cross(ss, ns);
        if (ts.LengthSquared() > 0.f) {
            ts = Normalize(ts);
            ss = Cross(ts, ns);
        }
        else
            CoordinateSystem((Vector3f)ns, &ss, &ts);

        isect->SetShadingGeometry(ss, ts, dndu, dndv, true);
    }

    if (mesh->n)
        isect->n = Faceforward(isect->n, isect->shading.n);
    else if (reverseOrientation ^ transformSwapsHandedness)
        isect->n = isect->shading.n = -isect->n;
        */
    return false;
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

