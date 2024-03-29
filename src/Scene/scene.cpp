
//
//  Scene.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include "scene.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "primitive.hpp"
#include "mesh.hpp"
#include "Phong.hpp"
#include "AreaLight.hpp"

#include <iostream>
#include <set>
#include <vector>

using namespace std;
using namespace tinyobj;

static void PrintInfo (const ObjReader myObj) {
    const tinyobj::attrib_t attrib = myObj.GetAttrib();
    const std::vector<tinyobj::shape_t> shapes = myObj.GetShapes();
    const std::vector<tinyobj::material_t> materials = myObj.GetMaterials();
    //std::cout << "# of vertices  : " << (attrib.vertices/ 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
              << std::endl;

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;
    
    // Iterate shapes
    auto it_shape = shapes.begin();
    for ( ; it_shape != shapes.end() ; it_shape++) {
        // assume each face has 3 vertices
        std::cout << "Processing shape " << it_shape->name << std::endl;
        // iterate faces
        // assume each face has 3 vertices
        auto it_vertex = it_shape->mesh.indices.begin();
        for ( ; it_vertex != it_shape->mesh.indices.end() ; ) {
            // process 3 vertices
            for (int v=0 ; v<3 ; v++) {
                std::cout << it_vertex->vertex_index;
                it_vertex++;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        
        printf("There are %lu material indexes\n", it_shape->mesh.material_ids.size());
    }


    
}

/*
 Use tiny load to load .obj scene descriptions
 https://github.com/tinyobjloader/tinyobjloader
 */




bool Scene::Load (const std::string &fname) {
    ObjReader myObjReader;

    if (!myObjReader.ParseFromFile(fname)) {
        printf("Failed to load .obj file: %s\n", fname.c_str());
        return false;
    }
    const tinyobj::attrib_t attrib = myObjReader.GetAttrib();
    vector<float> vertices = attrib.vertices;

    const std::vector<shape_t> shps = myObjReader.GetShapes();
    const std::vector<material_t> materials = myObjReader.GetMaterials();
    for (auto mat = materials.begin(); mat != materials.end(); mat++){
        auto *m = new Phong();

        m->Kd =RGB(mat->diffuse[0],mat->diffuse[1], mat->diffuse[2]);
        m->Ks =RGB(mat->specular[0],mat->specular[1], mat->specular[2]);
        printf("specular %f %f %f\n",mat->specular[0],mat->specular[1], mat->specular[2]);
        m->Ka = RGB(mat->ambient[0],mat->ambient[1], mat->ambient[2]);
        m->Kt = RGB(mat->transmittance[0],mat->transmittance[1], mat->transmittance[2]);
        m->Ns = mat->shininess;
        this->BRDFs.push_back(m);
        this->numBRDFs++;
    }



    // iterate over shapes
    for (auto shp = shps.begin() ; shp != shps.end() ; shp++) {

        auto *p = new Primitive ();
        Mesh* mesh = new Mesh();
        p->g = mesh;

        // iterate over this shape’s vertices
        auto indices = shp->mesh.indices;
        p->material_ndx= shp->mesh.material_ids[0];


        for (auto vertex = indices.begin() ; vertex != indices.end() ; ) {
            Face* face = new Face();
            //face->bb.min = Point(MAXFLOAT, MAXFLOAT, MAXFLOAT);
            //face->bb.max = Point(-MAXFLOAT, -MAXFLOAT, -MAXFLOAT);
            // each 3 consecutives vertices form a face (triangle)

            Point myVertex[3];
            for (int v = 0 ; v<3 ; v++ , vertex++) {
                // get each vertex XYZ
                const int objNdx = vertex->vertex_index;
                myVertex[v].X = vertices[objNdx*3];
                myVertex[v].Y = vertices[objNdx*3+1];
                myVertex[v].Z = vertices[objNdx*3+2];
                int index = mesh->getIndexVertices(myVertex[v]);
                if (index != -1) face->vert_ndx[v] = index;
                else{
                    mesh->addVertice(myVertex[v]);
                    face->vert_ndx[v] = mesh->numVertices-1;

                }
                face->bb.update(myVertex[v]);
            }

            //calculate normal of face
            Vector edge1 = mesh->vertices[face->vert_ndx[1]].vec2point(mesh->vertices[face->vert_ndx[0]]);
            Vector edge2 = mesh->vertices[face->vert_ndx[2]].vec2point(mesh->vertices[face->vert_ndx[0]]);
            face->geoNormal = edge1.cross(edge2);
            face->geoNormal.normalize(); // ?



            //float fx = face->geoNormal.X == -0 ? 0 : face->geoNormal.X;
            //float fy= face->geoNormal.Y == -0 ? 0 : face->geoNormal.Y;
            //float fz = face->geoNormal.Z == -0 ? 0 : face->geoNormal.Z;
            //face->geoNormal = Vector(fx,fy,fz);


            mesh->faces.push_back(*face);
            mesh->numFaces++;
        }
        this->prims.push_back(p);
        this->numPrimitives++;
    }


    return true;
}

bool Scene::trace (Ray r, Intersection *isect) {
    Intersection curr_isect;
    bool intersection = false;    
    
    if (numPrimitives==0) return false;

    // iterate over all primitives



    isect->isLight = false; // download new intersection.hpp
    // now iterate over light sources and intersect with those that have geometry
    for (auto l = lights.begin() ; l != lights.end() ; l++) {
        if ((*l)->type == AREA_LIGHT) {
            AreaLight *al = (AreaLight *) *l;
            if (al->gem->intersect(r, &curr_isect)) {
                //printf("Intersect light\n");
                if (!intersection) { // first intersection
                    intersection = true;
                    *isect = curr_isect;
                    isect->isLight = true;
                    isect->Le = al->L();
                } else if (curr_isect.depth < isect->depth) {
                    *isect = curr_isect;
                    isect->isLight = true;
                    isect->Le = al->L();
                }
            }
        }
    }

    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() ; prim_itr++) {
        if ((*prim_itr)->g->intersect(r, &curr_isect)) {
            if (!intersection) {
                intersection = true;
                curr_isect.f = BRDFs[(*prim_itr)->material_ndx];
                *isect = curr_isect;
                isect->isLight = false;
            }
            else if (curr_isect.depth < isect->depth) {
                curr_isect.f = BRDFs[(*prim_itr)->material_ndx];
                *isect = curr_isect;
                isect->isLight = false;
            }
        }
    }



    return intersection;
}

// checks whether a point on a light source (distance maxL) is visible
bool Scene::visibility (Ray s, const float maxL) {
    bool visible = true;
    Intersection curr_isect;

    if (numPrimitives==0) return true;

    // iterate over all primitives while visible
    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() && visible ; prim_itr++) {
        if ((*prim_itr)->g->intersect(s, &curr_isect)) {
            if (curr_isect.depth < maxL) {
                visible = false;
            }
        }
    }
    return visible;
}
