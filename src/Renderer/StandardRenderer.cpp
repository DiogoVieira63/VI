//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"
#include <random>

void StandardRenderer::Render () { // NOLINT(openmp-use-default-none)
    int W,H;  // resolution

    cam->getResolution(&W,&H);
    const int spp=1;

    // main rendering loop: get primary rays from the camera until done
    for (int y=0; y < H; y++) {  // loop over rows
        std::cout <<  y << std::endl;
        for (int x=0; x< W; x++) { // loop over columns
            RGB color(0.,0.,0.);
            for (int ss=0; ss < spp;ss++) {
                Ray primary;
                Intersection isect;
                bool intersected;
                float jitterV[2];
                jitterV[0] = ((float)rand()) / (float)RAND_MAX;
                jitterV[1] = ((float)rand()) / (float)RAND_MAX;
                cam->GenerateRay(x, y, &primary, jitterV);
                intersected = scene->trace(primary, &isect);
                RGB this_color = shd->shade (intersected, isect,1);
                color += this_color;

                // Generate Ray (camera)
                // trace ray (scene)

                //printf("trace\n");
                // shade this intersection (shader)
                //printf("shade\n");
                // write the result into the image frame buffer (image)

            }


            color = color / spp;
            //printf("color: %f %f %f\n",color.R,color.G,color.B);
            img->set(x,y,color);

        }
        // loop over columns
    }   // loop over rows
}

#pragma clang diagnostic pop