//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"

void StandardRenderer::Render () {
    int W=0,H=0;  // resolution
    int x,y;

    cam->getResolution(&W,&H);
    
    // main rendering loop: get primary rays from the camera until done
    for (y=0 ; y< H ; y++) {  // loop over rows
        for (x=0 ; x< W ; x++) { // loop over columns
            Ray primary;
            Intersection isect;
            bool intersected;
            RGB color;
            // Generate Ray (camera)
            cam->GenerateRay(x,y,&primary);
            printf("generated\n");
            // trace ray (scene)
            intersected = scene->trace(primary,&isect);
            printf("intersect\n");
            // shade this intersection (shader)
            color = this->shd->shade(intersected,isect);
            printf("shade\n");
            // write the result into the image frame buffer (image)
            img->set(x,y,color);
            
        } // loop over columns
    }   // loop over rows
}
