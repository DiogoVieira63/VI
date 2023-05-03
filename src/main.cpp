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
#include "PointLight.hpp"
#include "WhittedShader.hpp"
#include "DistributedShader.hpp"

int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *img;    // Image
    Shader *shd;
    bool success;

    //success = scene.Load("/home/lau/Desktop/Universidade/VI/VI/src/models/cornell_box.obj");
    success = scene.Load("../models/cornell_box_VI.obj");

    //success = scene.Load("../models/cornell_box_back.obj");
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }        
    std::cout << "Scene Load: SUCCESS!! :-)\n";
    scene.printSummary();
    std::cout << std::endl;

    // Image resolution
    const int W= 1024;
    const int H= W;

    img = new ImagePPM(W,H);
    
    // Camera parameters
    const Point Eye ={280,275,-330}, At={280,265,0};
    const Vector Up={0,-1,0};
    const float fovW = 3.14f/2.f, fovH = fovW * H/W;
    cam = new Perspective(Eye, At, Up, W, H, fovW, fovH);

    // add an ambient light to the scene
    AmbientLight *ambient = new AmbientLight(RGB(0.3,0.3,0.3));
    scene.lights.push_back(ambient);
    scene.numLights++;

    // add a point light to the scene
    PointLight *pl1 = new PointLight(RGB(0.65,0.65,0.65), Point(288,508,282));
    //scene.lights.push_back(pl1);
    //scene.numLights++;


    // Luz central
    AreaLight *al1 = new AreaLight(RGB(1,1,1),Point(343.0,547.9,227.0),Point(343.0,547.9,332.0),Point(213.0,547.9,332.0),Vector(0,-1,0));
    AreaLight *al2 = new AreaLight(RGB(1,1,1), Point(213.0,547.9,332.0),Point(213.0,547.9,227.0),Point(343.0,547.9,227.0),Vector(0,-1,0));

    scene.lights.push_back(al1);
    scene.lights.push_back(al2);
    scene.numLights+=2;

    // Luz canto frente direito
    AreaLight *al3 = new AreaLight(RGB(0.70,0.70,0.70),Point(93.0,547.9,57.0),Point(93.0,547.9,82.0),Point(43.0,547.9,82.0),Vector(0,-1,0));
    AreaLight *al4 = new AreaLight(RGB(0.70,0.70,0.70), Point(43.0,547.9,82.0),Point(43.0,547.9,57.0),Point(93.0,547.9,57.0),Vector(0,-1,0));

    scene.lights.push_back(al3);
    scene.lights.push_back(al4);
    scene.numLights+=2;

    // Luz canto trás esquerda
    AreaLight *al5 = new AreaLight(RGB(0.70,0.70,0.70),Point(493.0,547.9,457.0),Point(493.0,547.9,482.0),Point(443.0,547.9,482.0),Vector(0,-1,0));
    AreaLight *al6 = new AreaLight(RGB(0.70,0.70,0.70), Point(443.0,547.9,482.0),Point(443.0,547.9,457.0),Point(493.0,547.9,457.0),Vector(0,-1,0));

    scene.lights.push_back(al5);
    scene.lights.push_back(al6);
    scene.numLights+=2;

    // Luz canto trás direita
    AreaLight *al7 = new AreaLight(RGB(0.70,0.70,0.70),Point(93.0,547.9,457.0),Point(93.0,547.9,482.0),Point(43.0,547.9,482.0),Vector(0,-1,0));
    AreaLight *al8 = new AreaLight(RGB(0.70,0.70,0.70), Point(43.0,547.9,482.0),Point(43.0,547.9,457.0),Point(93.0,547.9,457.0),Vector(0,-1,0));

    scene.lights.push_back(al7);
    scene.lights.push_back(al8);
    scene.numLights+=2;

    // Luz canto frente esquerda
    AreaLight *al9= new AreaLight(RGB(0.70,0.70,0.70),Point(493.0,547.9,57.0),Point(493.0,547.9,82.0),Point(443.0,547.9,82.0),Vector(0,-1,0));
    AreaLight *al10 = new AreaLight(RGB(0.70,0.70,0.70), Point(443.0,547.9,82.0),Point(443.0,547.9,57.0),Point(493.0,547.9,57.0),Vector(0,-1,0));

    scene.lights.push_back(al9);
    scene.lights.push_back(al10);
    scene.numLights+=2;

    // create the shader
    RGB background(0.05, 0.05, 0.55);

    //shd = new WhittedShader(&scene, background);
    //shd = new AmbientShader(&scene, background);
    shd = new DistributedShader(&scene,background);

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
