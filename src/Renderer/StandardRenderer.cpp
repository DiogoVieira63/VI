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

    // main rendering loop: get primary rays from the camera until done
    for (int y = 0; y < H; y++) {  // loop over rows
        //std::cout << y << std::endl;
        for (int x = 0; x < W; x++) { // loop over columns
            RGB color(0., 0., 0.);
            for (int ss = 0; ss < spp; ss++) {
                Ray primary;
                Intersection isect;
                bool intersected;
                float jitterV[2];
                jitterV[0] = ((float) rand()) / (float) RAND_MAX;
                jitterV[1] = ((float) rand()) / (float) RAND_MAX;
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


//#define OMP_RAND_MAX 0xffffffff
//typedef unsigned long long rand_state;
//
//// multiplier/modulus taken from Knuth Volume 2, Table 1
//static const int multiplier = 314159269;
//static const int addend = 1;
//static const int modulus = 0xffffffff;
//
//rand_state omp_srand() {
//    rand_state state = time(NULL);
//    state ^= (unsigned long long)omp_get_thread_num() << 32;
//    return state;
//}
//
//int omp_rand(rand_state *state) {
//    *state = *state * multiplier + addend;
//    return *state & modulus;
//}
//
//void StandardRenderer::Render () {
//    int W,H; // resolution
//    cam->getResolution(&W,&H);
//
//#pragma omp declare reduction(+: RGB: omp_out += omp_in)
//    RGB color;
//
//    const int spp=16;
//
//#pragma omp parallel for reduction(+:color)
//    for (int y=0; y < H; y++) {  // loop over rows
//        for (int x=0; x< W; x++) { // loop over columns
//            color = RGB(0.,0.,0.);
//
//            for (int ss = 0; ss < spp; ss++) {
//                Ray primary;
//                Intersection isect;
//                bool intersected;
//                float jitterV[2];
//
//                rand_state seed = omp_srand();
//                jitterV[0] = (float)omp_rand(&seed) / OMP_RAND_MAX;
//                jitterV[1] = (float)omp_rand(&seed) / OMP_RAND_MAX;
//
//                cam->GenerateRay(x, y, &primary, jitterV);
//                intersected = scene->trace(primary, &isect);
//                RGB this_color = shd->shade(intersected, isect, 1);
//                color += this_color;
//
//            }
//
//            color = color / spp;
//            img->set(x,y,color);
//        }
//    }
//}

#pragma clang diagnostic pop

