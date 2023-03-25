//
//  main.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include "scene.hpp"
#include "perspective.hpp"
#include "ImagePPM.hpp"
#include "StandardRenderer.hpp"
#include "AmbientShader.hpp"
#include "AmbientLight.hpp"

int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *img;    // Image
    Shader *shd;
    bool success;

    //success = scene.Load("/home/lau/Desktop/Universidade/VI/VI/src/models/cornell_box.obj");
    success = scene.Load("../models/cornell_box.obj");
    //success = scene.Load("../models/cornell_box_back.obj");
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }        
    std::cout << "Scene Load: SUCCESS!! :-)\n";
    scene.printSummary();
    std::cout << std::endl;

    // add an ambient light to the scene
    AmbientLight ambient(RGB(0.9,0.9,0.9));
    scene.lights.push_back(&ambient);
    scene.numLights++;

    // Image resolution
    const int W= 1024;
    const int H= W;

    img = new ImagePPM(W,H);
    
    // Camera parameters
    const Point Eye ={280,275,-330}, At={280,265,0};
    const Vector Up={0,-1,0};
    const float fovW = 3.14f/2.f, fovH = fovW * H/W;
    cam = new Perspective(Eye, At, Up, W, H, fovW, fovH);

    // create the shader
    RGB background(0.05, 0.05, 0.55);
    shd = new AmbientShader(&scene, background);
    // declare the renderer
    StandardRenderer myRender (cam, &scene, img, shd);
    // render
    myRender.Render();

    printf("Rendering done!\n");
    // save the image
    img->Save("../images/MyImage.ppm");
    printf("Image saved!\n");

    std::cout << "That's all, folks!" << std::endl;
    return 0;
}
