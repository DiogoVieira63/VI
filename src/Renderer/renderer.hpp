//
//  renderer.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef renderer_hpp
#define renderer_hpp

#include "camera.hpp"
#include "scene.hpp"

class Renderer {
protected:
    Camera *cam;
    Scene *scene;
public:
    Renderer (Camera *cam, Scene * scene): cam(cam), scene(scene) {}
    void Render () {}
};

#endif /* renderer_hpp */
