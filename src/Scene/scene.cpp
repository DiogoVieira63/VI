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

#include <iostream>
#include <set>
#include <vector>

using namespace std;
using namespace tinyobj;

static void PrintInfo (const ObjReader myObj) {
    const tinyobj::attrib_t attrib = myObj.GetAttrib();
    const std::vector<tinyobj::shape_t> shapes = myObj.GetShapes();
    const std::vector<tinyobj::material_t> materials = myObj.GetMaterials();
    std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
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
        return false;
    }
    const tinyobj::attrib_t attrib = myObjReader.GetAttrib();
    float *vertices = attrib.vertices;


    const std::vector<shape_t> shps = myObjReader.GetShapes();
    // iterate over shapes
    for (auto shp = shps.begin() ; shp != shps.end() ; shp++) {

        Primitive* p = new Primitive ();
        Mesh* mesh = new Mesh();
        p->g = mesh;

        // iterate over this shape’s vertices
        auto indices = shp->mesh.indices;

        for (auto vertex = indices.begin() ; vertex != indices.end() ; ) {

            Face* face = new Face();
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
            }
        }
        this->prims.push_back(p);
    }

    /*typedef struct Face {
    int vert_ndx[3];            // indices to our internal vector of vertices (in Mesh)
    Vector geoNormal;           // geometric normal
    bool hasShadingNormals;     // are there per vertex shading normals ??
    int vert_normals_ndx[3];    // indices to veritices normals
    BB bb;      // face bounding box
                // this is min={0.,0.,0.} , max={0.,0.,0.} due to the Point constructor
} Face;*/



    //PrintInfo (myObjReader)

    // convert loader's representation to my representation

    // your code here
    return true;
}

bool Scene::trace (Ray r, Intersection *isect) {
    Intersection curr_isect;
    bool intersection = false;    
    
    if (numPrimitives==0) return false;
    
    // iterate over all primitives
    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() ; prim_itr++) {
        if (prim_itr->g->intersect(r, &curr_isect)) {
            if (!intersection) { // first intersection
                intersection = true;
                *isect = curr_isect;
                isect->f = &BRDFs[prim_itr->material_ndx];
            }
            else if (curr_isect.depth < isect->depth) {
                *isect = curr_isect;
            }
        }
    }
    return intersection;
}

