//
// Created by lau on 27-04-2023.
//

#ifndef VI_RT_DISTRIBUTEDSHADER_HPP
#define VI_RT_DISTRIBUTEDSHADER_HPP

#include "RGB.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "AreaLight.hpp"

class DistributedShader: public Shader {
    RGB background;
    RGB directLighting (Intersection isect, Phong *f);
    RGB specularReflection (Intersection isect, Phong *f);
public:
    DistributedShader (Scene *scene, RGB bg): background(bg), Shader(scene) {}
    RGB shade (bool intersected, Intersection isect,int depth);
};


#endif //VI_RT_DISTRIBUTEDSHADER_HPP
