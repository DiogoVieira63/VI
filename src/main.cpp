//
//  main.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//
#include <chrono>
#include<vector>
using namespace std;
using namespace std::chrono;

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
#include "PathTracerShader.hpp"
#include "Environment.h"

int main(int argc, const char * argv[]) {
    Scene scene;
    Camera *cam; // Camera

    int i;
    cout << "Please enter an integer for image format choice: \n\t 1 - PPM \n\t 2 - JPEG \n\t 3 - PFM \n\t 4 - openEXR \n\t\t>>";
    cin >> i;

    ImagePPM *img;    // Image
    Shader *shd;
    bool success;
    auto start = high_resolution_clock::now();

    success = scene.Load("../models/cornell_box_VI.obj");

    //success = scene.Load("../models/cornell_box.obj");
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }        
    std::cout << "Scene Load: SUCCESS!! :-)\n";
    //scene.printSummary();
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
    const Point EyeE ={280,278,-100}, AtE={280,278,250};
    //cam = new Environment(EyeE, AtE, Up, W, H, fovW, fovH);

    // add an ambient light to the scene
    /*
    AmbientLight *ambient = new AmbientLight(RGB(0.3,0.3,0.3));
    scene.lights.push_back(ambient);
    scene.numLights++;
     */

    // add a point light to the scene
    PointLight *pl1 = new PointLight(RGB(0.65,0.65,0.65), Point(288,508,282));
    //scene.lights.push_back(pl1);
    //scene.numLights++;

    // Luz central
    float light = 0.05;
    AreaLight *al1 = new AreaLight(RGB(light,light,light),Point(343.0,547.9,227.0),Point(343.0,547.9,332.0),Point(213.0,547.9,332.0),Vector(0,-1,0));
    AreaLight *al2 = new AreaLight(RGB(light,light,light),Point(213.0,547.9,332.0),Point(213.0,547.9,227.0),Point(343.0,547.9,227.0),Vector(0,-1,0));

    scene.lights.push_back(al1);
    scene.lights.push_back(al2);
    scene.numLights+=2;

    /*
    // Luz canto frente direito
    AreaLight *al3 = new AreaLight(RGB(light,light,light),Point(93.0,547.9,57.0),Point(93.0,547.9,82.0),Point(43.0,547.9,82.0),Vector(0,-1,0));
    AreaLight *al4 = new AreaLight(RGB(light,light,light), Point(43.0,547.9,82.0),Point(43.0,547.9,57.0),Point(93.0,547.9,57.0),Vector(0,-1,0));

    scene.lights.push_back(al3);
    scene.lights.push_back(al4);
    scene.numLights+=2;

    // Luz canto trás esquerda
    AreaLight *al5 = new AreaLight(RGB(light,light,light),Point(493.0,547.9,457.0),Point(493.0,547.9,482.0),Point(443.0,547.9,482.0),Vector(0,-1,0));
    AreaLight *al6 = new AreaLight(RGB(light,light,light), Point(443.0,547.9,482.0),Point(443.0,547.9,457.0),Point(493.0,547.9,457.0),Vector(0,-1,0));

    scene.lights.push_back(al5);
    scene.lights.push_back(al6);
    scene.numLights+=2;

    // Luz canto trás direita
    AreaLight *al7 = new AreaLight(RGB(light,light,light),Point(93.0,547.9,457.0),Point(93.0,547.9,482.0),Point(43.0,547.9,482.0),Vector(0,-1,0));
    AreaLight *al8 = new AreaLight(RGB(light,light,light), Point(43.0,547.9,482.0),Point(43.0,547.9,457.0),Point(93.0,547.9,457.0),Vector(0,-1,0));

    scene.lights.push_back(al7);
    scene.lights.push_back(al8);
    scene.numLights+=2;

    // Luz canto frente esquerda
    AreaLight *al9= new AreaLight(RGB(light,light,light),Point(493.0,547.9,57.0),Point(493.0,547.9,82.0),Point(443.0,547.9,82.0),Vector(0,-1,0));
    AreaLight *al10 = new AreaLight(RGB(light,light,light), Point(443.0,547.9,82.0),Point(443.0,547.9,57.0),Point(493.0,547.9,57.0),Vector(0,-1,0));

    scene.lights.push_back(al9);
    scene.lights.push_back(al10);
    scene.numLights+=2;

*/
    // create the shader
    RGB background(0.05, 0.05, 0.35);

    //shd = new WhittedShader(&scene, background);
    //shd = new AmbientShader(&scene, background);
    //shd = new DistributedShader(&scene,background);
    shd = new PathTracerShader(&scene,background);
    // declare the renderer
    StandardRenderer myRender (cam, &scene, img, shd);

    // render
    myRender.Render();

    printf("Rendering done!\n");

    switch (i) {
        case 1:
            img->Save("../images/MyImage.ppm");
            printf("Image saved!\n");
            break;
        case 2:
            img->SaveJPG("../images/MyImage.jpg");
            printf("Image saved!\n");
            break;
        case 3:
            img->SavePFM("../images/MyImage.pfm");
            printf("Image saved!\n");
            break;
        case 4:
            img->SavePFM("../images/MyImage.exr");
            printf("Image saved!\n");
            break;
    }

    // save the image
    //img->Save("../images/MyImage.ppm");
    //printf("Image saved!\n");



    // Get ending timepoint
    auto stop = high_resolution_clock::now();

    // Get duration. Substart timepoints to
    // get duration. To cast it to proper unit
    // use duration cast method
    auto duration = duration_cast<microseconds>(stop - start);

    auto duration_seconds = duration.count() / 1000000.0;
    cout << "Time taken: "
         << duration_seconds << " seconds." << endl;

    std::cout << "That's all, folks!" << std::endl;
    return 0;
}
