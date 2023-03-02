//
//  main.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include "scene.hpp"
#include "perspective.hpp"
#include "renderer.hpp"

int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective cam(640, 480);  // image resolution
    bool success;
    
    success = scene.Load("/Users/psantos/VI-RT/VI-RT/VI-RT/Scene/tinyobjloader/models/cornell_box.obj");
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }        
    std::cout << "Scene Load: SUCCESS!! :-)\n";
    scene.printSummary();
    std::cout << std::endl;
    
    // declare the renderer
    Renderer myRender (&cam, &scene);
    // render
    myRender.Render();
    // save the image
    cam.SaveImage();
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}
