//
// Created by diogo on 5/11/23.
//

#ifndef VI_RT_PATHTRACERSHADER_HPP
#define VI_RT_PATHTRACERSHADER_HPP


#include "RGB.hpp"
#include "intersection.hpp"
#include "Phong.hpp"
#include "scene.hpp"
#include "shader.hpp"

class PathTracerShader: public Shader {
    RGB background;
    RGB directLighting (Intersection isect, Phong *f);
    RGB specularReflection (Intersection isect, Phong *f, int
    depth);
    RGB diffuseReflection (Intersection isect, Phong *f, int depth);
    float continue_p;
    int MAX_DEPTH;
public:
    PathTracerShader (Scene *scene, RGB bg): background(bg),Shader(scene) {continue_p = 0.5f; MAX_DEPTH=2;}
    RGB shade (bool intersected, Intersection isect, int depth = 0);
};


#endif //VI_RT_PATHTRACERSHADER_HPP
