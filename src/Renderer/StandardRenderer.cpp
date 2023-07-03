//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"
#include "perspective.hpp"
#include <random>
#include <ctime>
#include <omp.h>

void StandardRenderer::Render () { // NOLINT(openmp-use-default-none)
    int W,H;  // resolution

    cam->getResolution(&W,&H);
    const int spp=16;

    int maxThreads = omp_get_max_threads();
    omp_set_num_threads(maxThreads);

    // main rendering loop: get primary rays from the camera until done
#pragma omp parallel num_threads(8) default(none) shared(cam, W, H)
    {
        for (int y = 0; y < H; y++) {  // loop over rows

#pragma omp for 
            //std::cout << y << std::endl;
            for (int x = 0; x < W; x++) { // loop over columns
                RGB color(0., 0., 0.);
                for (int ss = 0; ss < spp; ss++) {
                    Ray primary;
                    Intersection isect;
                    bool intersected;
                    float jitterV[2];
                    jitterV[0] = 0; //((float) rand()) / (float) RAND_MAX;
                    jitterV[1] = 0; //((float) rand()) / (float) RAND_MAX;
                    cam->GenerateRay(x, y, &primary, jitterV);
                    intersected = scene->trace(primary, &isect);
                    RGB this_color = shd->shade(intersected, isect, 1);
                    color += this_color;
                }

                color = color / spp;
                img->set(x, y , color);
            }
        }
    }

}

#pragma clang diagnostic pop

