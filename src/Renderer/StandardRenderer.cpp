//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"
#include <random>

void StandardRenderer::Render () {
    int W=0,H=0;  // resolution
    int x,y,ss;

    cam->getResolution(&W,&H);
    const int spp=16;


    // main rendering loop: get primary rays from the camera until done
    for (y=0 ; y<  H ; y++) {  // loop over rows
        for (x=0; x< W ; x++) { // loop over columns
            Ray primary;
            Intersection isect;
            bool intersected;
            RGB color(0.,0.,0.);
            for (ss=0; ss < spp;ss++) {
                float jitterV[2];
                jitterV[0] = ((float)rand()) / (float)RAND_MAX;
                jitterV[1] = ((float)rand()) / (float)RAND_MAX;
                cam->GenerateRay(x, y, &primary, jitterV);
                intersected = scene->trace(primary, &isect);
                RGB this_color = shd->shade (intersected, isect);
                color += this_color;

                // Generate Ray (camera)
                // trace ray (scene)

                //printf("trace\n");
                // shade this intersection (shader)
                //printf("shade\n");
                // write the result into the image frame buffer (image)

            }
            color = color / spp;
            img->set(x,y,color);
            
        } // loop over columns
    }   // loop over rows
}
